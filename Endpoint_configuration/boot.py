import binascii
import pycom
import socket
import time
import machine
from network import LoRa
import struct
import ustruct
import ubinascii
from machine import Pin, ADC, UART

# Colors
off = 0x000000
red = 0xff0000
green = 0x00ff00
blue = 0x0000ff
pink = 0xff69b4

# Turn off hearbeat LED
pycom.heartbeat(False)

#Turn off Wifi
pycom.wifi_on_boot(False) #Untested

#constants and I/O object making
adc = ADC()
batt = adc.channel(attn=1, pin='P16')
#pin_in = Pin('G17', mode=Pin.IN, pull=Pin.PULL_UP)
#Wakeup configuration
pin_in = Pin('P13', mode=Pin.IN, pull= Pin.PULL_DOWN)
machine.pin_sleep_wakeup(['P13'], machine.WAKEUP_ANY_HIGH, True)

# Initialize LoRaWAN radio to Europe and max allowed tx_power according to ETSI standard (expecting antenna gain = cable loss on transmission side)
lora = LoRa(mode=LoRa.LORAWAN, region=LoRa.EU868)

uart1 = UART(1) #Usage of default RX = P4 and TX = P3 Used for UART communication with Arduino Nano. 
uart1.init(9600, bits=8, parity=None, stop=1)
uart2 = UART(2)
uart2.init(baudrate=9600, bits=8, parity=None, stop=1, pins=('P20','P21')) #P20: Tx, P21: Rx Used for datareception from GPS module

##-------------------------------Interrupt---------------------------------------------##
def interrupt_handler(arg):
    print("Interrupt is called")
    uart1.write("OK")
    print("OK written to Ardunio")
    go_to_loop()

pin_stop_button = Pin('G17', mode=Pin.IN, pull=Pin.PULL_UP)
pin_stop_button.callback(Pin.IRQ_FALLING | Pin.IRQ_RISING, interrupt_handler)

##------------------------------------------------------------------------------------------##

##This function is using UART1 to read what is delivered by the GPS module. The module deliveres degree coordinates from the GPGGA datagram which in slot 2 and 4 contains lat and lon og the position. 
##The function is configured as a recursive function which reads the buffer, if the critical data slot is not filled it will call itself again. This will go on for 10 times as this is the maximum limit for recursion dept set by microPython
def get_gps_buffer(buffer_type, critical_data, recursion_max):
    options_of_data = ["GPGGA", 'GPGSV']
    gps_buffer = str(uart2.read()).split("$")
    splitted_buffer = ""
    for i in range(len(gps_buffer)):
        splitted_buffer = gps_buffer[i].split(",")
        if(splitted_buffer[0] == options_of_data[buffer_type]):
            if(splitted_buffer[critical_data] != ''):
                return splitted_buffer, recursion_max # splitted_buffer[i]: i = (1 -> UTC time, 2 -> Latitude, 3 -> N or S, 4 -> Longitude, 5 -> E or W, 9 -> Antenna altitude above/below mean sea level (geoid))   for rest see: http://aprs.gids.nl/nmea/#gga
            else:
                print("Critical data not present in gps_buffer")
    if(splitted_buffer == ""):
        print("Unable to find GPS data. Retries in 1 second")

    time.sleep(1) #Sleeps for 1 seconds, then retry to fill up buffer. Only needed if GPGAA's message number critical_data slot is not present.
    if (recursion_max == 0):
        return ['122637.00', '5610.06764', '5610.06764', '01011.43309', '01011.43309', '10.00000', '10.00000', '10.00000', '10.00000', '10.00000', '', '', '', '*65\\r\\n'], recursion_max # Default LAT and LON set to 10(somewhere in Nigeria)
    new_recursion = recursion_max - 1
    return get_gps_buffer(buffer_type, critical_data, new_recursion)

def go_to_loop():
    while True:
        pycom.rgbled(green)
        time.sleep(0.1)
        pycom.rgbled(blue)
        time.sleep(2)
        lora.nvram_save()
        deepsleep_time = 3600000 #Max transmission duty cycle according to ETSI = 1%, TOA for config(SF=12, CR=4/5, payload = 12 Bytes, preamble=8) = 1.155s => wait must equal  116 seconds to satisfy ETSI pr channel  (https://www.loratools.nl/#/airtime)
        machine.deepsleep(deepsleep_time) #Sleeps for 1 hour


if machine.reset_cause() == machine.DEEPSLEEP_RESET: #Use saved configuration if awoken
    lora.nvram_restore()
    pycom.rgbled(red) #Red when package sending is started
    s = socket.socket(socket.AF_LORA, socket.SOCK_RAW)
    s.setsockopt(socket.SOL_LORA, socket.SO_DR, 0)
    s.setsockopt(socket.SOL_LORA, socket.SO_CONFIRMED, True)
    s.setblocking(True)
    ##GPS section##
    gps_buffer, waittime = get_gps_buffer(0,2, 10)
    pycom.rgbled(pink)
    time.sleep(waittime)
    print(gps_buffer[2])
    print(gps_buffer[4])
    lat = float(gps_buffer[2][0:2]) + float(gps_buffer[2][2:len(gps_buffer[2])])/60 #Conversion from NMES GPGGA version of Latitude degree to Latitude comma coordinate
    lon = float(gps_buffer[4][0:3]) + float(gps_buffer[4][3:len(gps_buffer[4])])/60 #Conversion from NMES GPGGA version of Longitude degree to Longitude comma coordinate
    #lat = gps_buffer[2]
    #lon = gps_buffer[4]
    
    print(" lat del1: {} \n lat del2: {} \n lon del1: {} \n lon del2: {}".format(
        float(gps_buffer[2][0:2]), float(gps_buffer[2][2:len(gps_buffer[2])])/60,
        float(gps_buffer[4][0:3]), float(gps_buffer[4][3:len(gps_buffer[4])])/60
    ))

    uart1.write("OK")
    print("OK written to Ardunio")
    ##Analog readings##
    batt = batt.value()

    ##Transmission section##
    pycom.rgbled(blue) #Switching to blue when GPS is done.
    payload = bytes(struct.pack("fff", lat, lon, batt)) #Serverside will receive a 12 byte payload on the form lat = payload[0:3] lon = payload[4:7] , battery = payload[8:11]
    count = s.send(payload)
    print("{} \n {}".format(lat,lon))
    print('Sent %s bytes' % count)
    go_to_loop()
else: #Should only run one time, and activates the device up against the join server with the given keys.

    # OTAA is used in this configuration
    dev_eui = lora.mac()
    app_key = binascii.unhexlify('c0f14c7040f7a0b976a61d8569e0e5c4')

    lora.join(activation=LoRa.OTAA, auth=(dev_eui, app_key), timeout=0, dr=0)  #Join the network
    pycom.rgbled(red)

    while not lora.has_joined(): # Loop until joined
        print('Not joined yet...')
        pycom.rgbled(red)
        time.sleep(0.1)
        pycom.rgbled(off)
        time.sleep(2)

    print('Joined')
    lora.nvram_save()
    go_to_loop()

# create an (ABP) authentication params
#dev_addr = struct.unpack(">l", ubinascii.unhexlify('00F644DE'))[0]
#nwk_swkey = ubinascii.unhexlify('43FF7A55907D6C0050C57971D66F1E6D')
#app_swkey = ubinascii.unhexlify('A2349333BB885FFE126673AE3EE19B15')

# Join the network
#lora.join(activation=LoRa.ABP, auth=(dev_addr, nwk_swkey, app_swkey), timeout=0, dr=0)
