//#include <_Teensy.h>

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioInputUSB            usb1;           //xy=211.36666870117188,320.3666687011719
AudioOutputI2S  i2s1;          //xy=542.36669921875,330.3666687011719
AudioAnalyzeRMS rms1;
AudioAnalyzeRMS rms2;

AudioConnection          patchCord1(usb1, 0, i2s1, 0);
AudioConnection          patchCord2(usb1, 0, rms1, 0);
AudioConnection          patchCord3(usb1, 1, i2s1, 1);
AudioConnection          patchCord4(usb1, 1, rms2, 0);


AudioControlSGTL5000     sgtl5000_1;     //xy=380.3666687011719,521.3666687011719
// GUItool: end automatically generated code
// Use these with the Teensy Audio Shield
#define SDCARD_CS_PIN 10
#define SDCARD_MOSI_PIN 7
#define SDCARD_SCK_PIN 14

const int ledPin = 13;

float maxRmsAmp = 0.0;

void setup() {
  // put your setup code here, to run once:
  AudioMemory(12);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.6);

  sgtl5000_1.autoVolumeDisable();
  sgtl5000_1.audioProcessorDisable();
  sgtl5000_1.unmuteLineout();
  
  pinMode(ledPin, OUTPUT);
  
  digitalWrite(ledPin, HIGH);   // set the LED on

}

void loop() {
  // put your main code here, to run repeatedly:
   // read the PC's volume setting
  float vol = usb1.volume();

  // scale to a nice range (not too loud)
  // and adjust the audio shield output volume
  if (vol > 0) {
    // scale 0 = 1.0 range to:
    //  0.3 = almost silent
    //  0.8 = really loud
    vol = 0.3 + vol * 0.5;
  }
 // use the scaled volume setting.  Delete this for fixed volume.
  sgtl5000_1.volume(vol);
  sgtl5000_1.lineOutLevel(31);

  float curRmsAmp = rms1.read();
  if (curRmsAmp > maxRmsAmp){
    maxRmsAmp = curRmsAmp;
  }
  if (curRmsAmp > (0.5 * maxRmsAmp)){
    digitalWrite(ledPin, HIGH);   // set the LED on
  }
  else{
    digitalWrite(ledPin, LOW);   // set the LED on
  }


  delay(100);

  
}
