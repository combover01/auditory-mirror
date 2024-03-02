# Write your code here :-)
from gpiozero import LED
from gpiozero import Button
import time

led = LED(17)
button = Button(2)

# while True:
#     button.wait_for_press()
#     print("You Pushed me")

#     led.toggle()
#     time.sleep(0.5)


"""Small example OSC client

This program sends 10 random values between 0.0 and 1.0 to the /filter address,
waiting for 1 seconds between each value.
"""
import argparse
import random
import time
import math

from pythonosc import udp_client
from pythonosc import osc_server
from pythonosc.dispatcher import Dispatcher


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--ip", default="127.0.0.1", help="The ip of the OSC server")
    parser.add_argument("--portOut", type=int, default=5005, help="The port the OSC server is sending to")
    parser.add_argument("--portIn", type=int, default=5006, help="The port the OSC server is listening to")
    args = parser.parse_args()

    client = udp_client.SimpleUDPClient(args.ip, args.portOut)

    dispatcher=Dispatcher()
    dispatcher.map("/filter",print)

    server = osc_server.ThreadingOSCUDPServer(
        (args.ip, args.portIn),dispatcher)
    print("serving on {}".format(server.server_address))
    server.serve_forever()

while True:
    for x in range(10):
        message = random.random()
        print("sending message " + str(message))
        client.send_message("/filter", message)
        led.toggle()
        time.sleep(1)