# Write your code here :-)
from gpiozero import LED
from gpiozero import Button
import time
import socket
import argpase


UDP_IP = "localhost"

led = LED(17)
button = Button(2)

while True:
    button.wait_for_press()
    print("You Pushed me")

    led.toggle()
    time.sleep(0.5)

