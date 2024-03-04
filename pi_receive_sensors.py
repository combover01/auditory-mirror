import serial
import time

port = ''
serial.EIGHTBITS
esp = serial.Serial (port)
esp.baudrate = 9600
connected = true

readvar = ''

#processing loop
while connected:
    #first bit of data
    dataIn = esp.read(1)
    if dataIn == "x":
        #x-position
        x = esp.read_until()    #reads until /n
        send_to_pd("/x_pos", x)
    elif dataIn == "y":
        #y-position
        #x-position
        y = esp.read_until()    #reads until /n
        send_to_pd("/y_pos", y)
    elif dataIn == "z":
        #z-position
        z = esp.read_until()    #reads until /n
        send_to_pd("/z_pos", z)
    elif dataIn == "H":
        #x velocity
        H = esp.read_until()    #reads until /n
        send_to_pd("/x_vel", x)
    elif dataIn == "H":
        #y velocity
        V = esp.read_until()    #reads until /n
        send_to_pd("/y_vel", V)
    elif dataIn == "F":
        #z velocity
        F = esp.read_until()    #reads until /n
        send_to_pd("/z_vel", F)
    elif dataIn == "b":
        #body signal
        b = esp.read_until()    #reads until /n
        send_to_pd("/body", b)
    elif dataIn == "~":
        connected = False
    time.sleep(0.015)   #sleep time should be the same as the sampling period of the sensors
    
    

def send_to_pd(prefix, value):
    print(prefix + ' ' + value)