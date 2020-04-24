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

# Turn off hearbeat LED
pycom.heartbeat(False)

#Turn off Wifi
pycom.wifi_on_boot(False) #Untested

#constants and I/O object making
ms_to_s = 1000
adc = ADC()
batt = adc.channel(attn=1, pin='P16')
#pin_in = Pin('G17', mode=Pin.IN, pull=Pin.PULL_UP)
pin_in = Pin('P13', mode=Pin.IN, pull= Pin.PULL_DOWN)
uart1 = UART(1) #Usage of default RX = P4 and TX = P3
uart1.init(9600, bits=8, parity=None, stop=1)
uart2 = UART(2)
uart2.init(baudrate=9600, bits=8, parity=None, stop=1, pins=('P20','P21')) #P20: Tx, P21: Rx

def get_gps_buffer(buffer_type, critical_data, recursion_max):
    options_of_data = ["GPGGA", 'GPGSV']
    gps_buffer = str(uart2.read()).split("$")
    splitted_buffer = ""
    for i in range(len(gps_buffer)):
        splitted_buffer = gps_buffer[i].split(",")
        if(splitted_buffer[0] == options_of_data[buffer_type]):
            if(splitted_buffer[critical_data] != ''): 
                return splitted_buffer # splitted_buffer[i]: i = (1 -> UTC time, 2 -> Latitude, 3 -> N or S, 4 -> Longitude, 5 -> E or W, 9 -> Antenna altitude above/below mean sea level (geoid))   for rest see: http://aprs.gids.nl/nmea/#gga
            else:
                print("Critical data not present in gps_buffer")
    if(splitted_buffer == ""):
        print("Unable to find GPS data. Retries in 1 second") 
           
    time.sleep(1) #Sleeps for 2 seconds, then retry to fill up buffer then retries if GPGAA is not present. 
    if (recursion_max == 0):
        return ['122637.00', '10.00000', '10.00000', '10.00000', '10.00000', '10.00000', '10.00000', '10.00000', '10.00000', '10.00000', '', '', '', '*65\\r\\n']
    new_recursion = recursion_max - 1
    return get_gps_buffer(buffer_type, critical_data, new_recursion)

def get_lat_lon(specific_buffer, lat_lon):
    if(lat_lon == "lat"): #Tested with 'GPGAA', conversion is number 1 and 2 is as it is and after 2 to end is divided by 60.
        lat_lon = float(specific_buffer[2][0:2]) + float(specific_buffer[2][2:len(specific_buffer[2])])/60

    elif(lat_lon == "lon"):  #Tested with 'GPGAA', conversion is number 1 to 3 is as it is and after 3 to end is divided by 60. 
        lat_lon = float(specific_buffer[4][0:3]) + float(specific_buffer[4][3:len(specific_buffer[4])])/60

    else: #Default to appear on 
        lat_lon = "wrong parameter: {}".format(lat_lon)

    return lat_lon


# Initialize LoRaWAN radio to Europe and max allowed tx_power according to ETSI standard (expecting antenna gain = cable loss on transmission side)
lora = LoRa(mode=LoRa.LORAWAN, region=LoRa.EU868)
#machine.pin_sleep_wakeup(['P10'], machine.WAKEUP_ALL_LOW, True)
machine.pin_sleep_wakeup(['P13'], machine.WAKEUP_ANY_HIGH, True)

if machine.reset_cause() == machine.DEEPSLEEP_RESET: #Use saved configuration if awoken
    uart1.write("OK")
    print("OK written to Ardunio")
    lora.nvram_restore()

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
# create an (ABP) authentication params
#dev_addr = struct.unpack(">l", ubinascii.unhexlify('00F644DE'))[0]
#nwk_swkey = ubinascii.unhexlify('43FF7A55907D6C0050C57971D66F1E6D')
#app_swkey = ubinascii.unhexlify('A2349333BB885FFE126673AE3EE19B15')

# Join the network
#lora.join(activation=LoRa.ABP, auth=(dev_addr, nwk_swkey, app_swkey), timeout=0, dr=0)

pycom.rgbled(red) #Red when package sending is started
s = socket.socket(socket.AF_LORA, socket.SOCK_RAW)
s.setsockopt(socket.SOL_LORA, socket.SO_DR, 0)
s.setsockopt(socket.SOL_LORA, socket.SO_CONFIRMED, True)
s.setblocking(True)

while True:
    ##GPS section##
    gps_buffer = get_gps_buffer(0,2, 10)
    lat = get_lat_lon(gps_buffer, "lat")
    lon = get_lat_lon(gps_buffer, "lon")
    
    ##Analog readings##
    batt = batt.value()

    ##Transmission section##
    pycom.rgbled(blue) #Switching to blue when GPS is done.
    payload = bytes(struct.pack("fff", lat, lon, batt)) #Serverside will receive a 12 byte payload on the form lat = payload[0:3] lon = payload[4:7] , battery = payload[8:11]
    count = s.send(payload) 
    print('Sent %s bytes' % count)

    pycom.rgbled(green)
    time.sleep(0.1)
    pycom.rgbled(blue)
    lora.nvram_save()
    wait_time = 3600000
    machine.deepsleep(wait_time) #Sleeps for 1 hour
    
    #Max transmission duty cycle according to ETSI = 1%, TOA for config(SF=12, CR=4/5, payload = 12 Bytes, preamble=8) = 1.155s => wait must equal  116 seconds to satisfy ETSI pr channel  (https://www.loratools.nl/#/airtime)

