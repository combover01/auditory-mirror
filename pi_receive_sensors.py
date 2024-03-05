import serial
import time

def send_to_pd(prefix, value):
    print(prefix + ' ' + value.decode(encoding='ascii').strip())

port = '/dev/ttyAMA0'
# serial.EIGHTBITS
esp = serial.Serial(port)
print(esp.name)
esp.baudrate = 9600
connected = True

#processing loop
while connected:
#     dataIn = esp.read()
#     print(dataIn)
    #first bit of data
    #print("read")
    dataIn = esp.read(1)
    #print(dataIn)
    if dataIn == b'x':
        #x-position
        x = esp.read_until()    #reads until /n
        #x = x.decode(encoding='ascii')
        send_to_pd("/x_pos", x)
    elif dataIn == b'y':
        #y-position
        y = esp.read_until()    #reads until /n
        send_to_pd("/y_pos", y)
    elif dataIn == b'z':
        #z-position
        z = esp.read_until()    #reads until /n
        send_to_pd("/z_pos", z)
    elif dataIn == b'H':
        #x velocity
        H = esp.read_until()    #reads until /n
        send_to_pd("/x_vel", x)
    elif dataIn == b'V':
        #y velocity
        V = esp.read_until()    #reads until /n
        send_to_pd("/y_vel", V)
    elif dataIn == b'F':
        #z velocity
        F = esp.read_until()    #reads until /n
        send_to_pd("/z_vel", F)
    elif dataIn == b'b':
        #body signal
        b = esp.read_until()    #reads until /n
        send_to_pd("/body", b)
    elif dataIn == b'~':
        print("~")
        connected = False
        break
    time.sleep(0.015)   #sleep time should be the same as the sampling period of the sensors


