import binascii
from pycom import heartbeat, wifi_on_boot, rgbled
import socket
from time import sleep
import machine
from network import LoRa
import struct
import ubinascii
from machine import Pin, ADC, UART

##-------------------------------COLORS-----------------------------------------------------##
off = 0x000000
red = 0xff0000
green = 0x00ff00
blue = 0x0000ff
pink = 0xff69b4
##------------------------------------------------------------------------------------------##
##-----------------------Pycom default overwritten------------------------------------------##
heartbeat(False)
#Turn off Wifi
wifi_on_boot(False) #Untested
##---------------------------------LoRa-----------------------------------------------------##
# Initialize LoRaWAN radio to Europe and max allowed tx_power 
# according to ETSI standard (expecting antenna gain = cable loss on transmission side)
lora = LoRa(mode=LoRa.LORAWAN, region=LoRa.EU868)
##------------------------------------------------------------------------------------------##
##-------------------------------PIN_setup--------------------------------------------------##
##-------------------------------UART-------------------------------------------------------##
uart1 = UART(1) #Usage of default RX = P4 and TX = P3. UART communication with Arduino Nano. 
uart1.init(9600, bits=8, parity=None, stop=1)
uart2 = UART(2)
uart2.init(baudrate=9600, bits=8, parity=None, 
        stop=1, pins=('P20','P21')) #P20: Tx, P21: Rx. Datareception from GPS module

##------------------------------------ADC---------------------------------------------------##
adc = ADC()
batt = adc.channel(attn=1, pin='P16')
##------------------------------------------------------------------------------------------##
##----------------------------Wakeup configuration------------------------------------------##
pin_in = Pin('P13', mode=Pin.IN, pull= Pin.PULL_DOWN)
machine.pin_sleep_wakeup(['P13'], machine.WAKEUP_ANY_HIGH, True)
#Wakeup configuration
##------------------------------------------------------------------------------------------##
##------------------------------------------------------------------------------------------##

class UI():
    def interrupt_handler(self, arg):
        print("Interrupt is called")
        uart1.write("OK")
        print("OK written to Arduino")
        afdsdCtrl2().go_to_sleep(3600000)

    def indicate(self, rgbHex):
        rgbled(rgbHex)
##-------------------------------Interrupt--------------------------------------------------##
pin_stop_button = Pin('G17', mode=Pin.IN, pull=Pin.PULL_UP)
pin_stop_button.callback(Pin.IRQ_FALLING | Pin.IRQ_RISING, UI().interrupt_handler)
##------------------------------------------------------------------------------------------##

class Uart_IF():
    def write_ok_to_arduino(self):
        uart1.write("OK")
    def get_gps_buffer(self):
        raw_gps_buffer = str(uart2.read())
        telegram_splitted_gps_buffer = raw_gps_buffer.split("$") #Telegrams starts with "$"
        return telegram_splitted_gps_buffer

class GPS_module():
    def get_current_pos(self, recursions):
        
        #Base case
        if (recursions == 0):
            #Conversion from NMES GPGGA version of Latitude degree to Latitude comma coordinate
            default_lat = '5610.06764'
            default_lon = '01011.43309'
            lat = float(default_lat[0:2]) + float(default_lat[2:len(default_lat)])/60
            #Conversion from NMES GPGGA version of Longitude degree to Longitude comma coordinate
            lon = float(default_lon[0:3]) + float(default_lon[3:len(default_lon)])/60 
            return lat, lon, recursions

        read_uart = Uart_IF().get_gps_buffer
        gps_buffer = read_uart()
        splitted_buffer = ""
        for i in range(len(gps_buffer)):
            splitted_buffer = gps_buffer[i].split(",")
            if(splitted_buffer[0] == "GPGGA"):
                if(splitted_buffer[2] != ''): 
                    raw_lat = str(splitted_buffer[2])
                    raw_lon = str(splitted_buffer[4])

                    #Conversion from NMES GPGGA version of Latitude degree to Latitude comma coordinate
                    lat = float(raw_lat[0:2]) + float(raw_lat[2:len(raw_lat)])/60
                    #Conversion from NMES GPGGA version of Longitude degree to Longitude comma coordinate
                    lon = float(raw_lon[0:3]) + float(raw_lon[3:len(raw_lon)])/60 
                    
                    return lat, lon, recursions
                else:
                    print("Critical data not present in gps_buffer")

        if(splitted_buffer == ""):
            print("GPS module not functional") 
            
        sleep(1) #Sleeps for 1 seconds, then retry to fill up buffer then retries if GPGAA is not present. 
        new_recursion = recursions - 1
        return self.get_current_pos(new_recursion)

class LoRa_IF():
    indicate = UI().indicate
    
    def set_socket_configuration(self):
        s = socket.socket(socket.AF_LORA, socket.SOCK_RAW)
        s.setsockopt(socket.SOL_LORA, socket.SO_DR, 0)
        s.setsockopt(socket.SOL_LORA, socket.SO_CONFIRMED, True)
        s.setblocking(True)
        return s

    def join_sequence(self, dev_eui, app_key):

        lora.join(activation=LoRa.OTAA, auth=(dev_eui, app_key), timeout=0, dr=0)  #Join the network
        self.indicate(red)

        while not lora.has_joined(): # Loop until joined
            print('Not joined yet...')
            self.indicate(red)
            sleep(0.1)
            self.indicate(off)
            sleep(2)

        print('Joined')
        lora.nvram_save()
        afdsdCtrl2().go_to_sleep(3600000)

    def send_packet(self, s, lat, lon, batt) :
	#Serverside will receive a 12 byte payload on the form
        #lat = payload[0:3] lon = payload[4:7] , battery = payload[8:11]
        payload = bytes(struct.pack("fff", lat, lon, batt)) 
        count = s.send(payload)
        print('Sent %s bytes' % count)

class afdsdCtrl2():
    get_current_pos= GPS_module().get_current_pos
    indicate = UI().indicate
    write_ok_to_arduino = Uart_IF().write_ok_to_arduino
    lora_if = LoRa_IF()

    def go_to_sleep(self, sleeptime):
        while True:
            self.indicate(green)
            sleep(0.1)
            self.indicate(blue)
            sleep(2)
            lora.nvram_save()
             
            machine.deepsleep(sleeptime) #Sleeps for 1 hour

    def deepsleep_wakeup_sequence(self, second_to_gps_try):
        lora.nvram_restore()
        self.indicate(red) #Red when package sending is started
        s = self.lora_if.set_socket_configuration()

        ##GPS section##
        lat, lon, waittime = self.get_current_pos(second_to_gps_try)
        print(waittime)
        self.indicate(pink)
        sleep(0.1 )#float(waittime)) #0.1 required else too fast for rgbled

        ##Analog readings##
        batt = 4095 #test

        ##Transmission section##
        self.indicate(blue) #Switching to blue when GPS is done.
        self.lora_if.send_packet(s, lat, lon, batt)

        self.write_ok_to_arduino()
        print("OK written to Ardunio")
        print("{} \n {}".format(lat,lon))
        
        self.go_to_sleep(1000)

#Use saved configuration if awoken
if machine.reset_cause() == machine.DEEPSLEEP_RESET: 
    afdsdCtrl2().deepsleep_wakeup_sequence(10) #Input to gps tries 1 second pr try
else: #Should only run one time, and activates the device up against the join server with the given keys.
    # OTAA is used in this configuration
    dev_eui = lora.mac()
    app_key = binascii.unhexlify('c0f14c7040f7a0b976a61d8569e0e5c4')
    LoRa_IF().join_sequence(dev_eui, app_key)
