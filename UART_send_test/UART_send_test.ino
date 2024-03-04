#include <HardwareSerial.h>

void setup() {
  // put your setup code here, to run once:
  HardwareSerial SerialPort(2);
  SerialPort.begin (9600, SERIAL_8N1, 16, 17)
}

void loop() {
  // put your main code here, to run repeatedly:
  SerialPort.println("x1.00");
  SerialPort.println("y2.00");
  SerialPort.println("V3.00");
  SerialPort.println("b4.00");
  SerialPort.println("~");
  delay(15);
}
