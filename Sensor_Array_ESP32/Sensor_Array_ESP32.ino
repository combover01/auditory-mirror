/*
Code for using an array of HCSR04 ultrasonic sensors to triangulate the position and velocity of an object.
For GTCMT Hackathon Spring 2024
Renny Hyde 240303
*/
#include <HCSR04.h>

struct sensor {
  HCSR04 *object;
  float x;
  float y;
};
// use sensorX->object->dist() to get z

typedef struct sensor ultrasonic;

float d = 15;   //sensor spacing
float thresh = 40;   //furthest sensable distance
float dt = 1;    //sampling period of sensors in ms (not advised to go below 25ms)
const int xyWindow = 3;   //size of window in positional averaging filter
// float x_prev = -1;
// float y_prev = -1;
// float z_prev = -1;
float x = 0;
float y = 0;
float z = 0;
float dx;
float dy;
float dz;
float xwindow[xyWindow];
float ywindow[xyWindow];
float xavg = 0;
float yavg = 0;
float xavg_prev;
float yavg_prev;
float zavg = 0;
float z_prev;

ultrasonic s1;
ultrasonic s2;
ultrasonic s3;
ultrasonic s4;
ultrasonic s5;
ultrasonic s6;
ultrasonic s7;
ultrasonic s8;
ultrasonic s9;
ultrasonic s10;
ultrasonic s11;
ultrasonic s12;
ultrasonic s13;
ultrasonic s14;
ultrasonic s15;
ultrasonic s16;
ultrasonic s17;
ultrasonic s18;
HCSR04 hc1(2, 34); //trig pin, echo pin
HCSR04 hc2(2, 35);
HCSR04 hc3(2, 32);
HCSR04 hc4(2, 33);
HCSR04 hc5(2, 25);
HCSR04 hc6(2, 26);
HCSR04 hc7(2, 27);
HCSR04 hc8(2, 14);
HCSR04 hc9(2, 12);
HCSR04 hc10(2, 23);
HCSR04 hc11(2, 22);
HCSR04 hc12(2, 21);
HCSR04 hc13(2, 19);
HCSR04 hc14(2, 18);
HCSR04 hc15(2, 5);
HCSR04 hc16(2, 17);
HCSR04 hc17(2, 16);
HCSR04 hc18(2, 4);
ultrasonic *sensors[] = {&s1, &s2, &s3, &s4, &s5, &s6, &s7, &s8, &s9, &s10, &s11, &s12, &s13, &s14, &s15, &s16, &s17, &s18};


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  // INITIALIZE SENSORS
  // Each sensor must be passed an HCSR04 object (initialized outside of function) and a static x and y position
  s1.object = &hc1; //set
  s1.x = 1;
  s1.y = 64.5;
  s2.object = &hc2; //set
  s2.x = 23.098;
  s2.y = 62.23;
  s3.object = &hc3; //set
  s3.x = 43.688;
  s3.y = 65.135;
  s4.object = &hc4; //set
  s4.x = 66.2305;
  s4.y = 64.5;
  s5.object = &hc5; //set
  s5.x = 1;
  s5.y = 43.18;
  s6.object = &hc6; //set
  s6.x = 22.59;
  s6.y = 43.18;
  s7.object = &hc7; //set
  s7.x = 44.18;
  s7.y = 44.815;
  s8.object = &hc8; //set
  s8.x = 64.77;
  s8.y = 44.18;
  s9.object = &hc9; //set
  s9.x = 1;
  s9.y = 22.86;
  s10.object = &hc10; //set
  s10.x = 23.225;
  s10.y = 23.225;
  s11.object = &hc11; //set
  s11.x = 44.815;
  s11.y = 23.86;
  s12.object = &hc4; //set
  s12.x = 66.405;
  s12.y = 23.86;
  s13.object = &hc13; //set
  s13.x = 1;
  s13.y = 1;
  s14.object = &hc14; //set
  s14.x = 23.59;
  s14.y = 1;
  s15.object = &hc15; //set
  s15.x = 44.815;
  s15.y = 1;
  s16.object = &hc16; //set
  s16.x = 66.405;
  s16.y = 1;
  s17.object = &hc17; //set
  s17.x = 1;
  s17.y = 83.55;
  s18.object = &hc18; //set
  s18.x = 65.405;
  s18.y = 83.185;
  //initialize rolling average to zero
  for (int i =0; i < xyWindow; i++) {
    xwindow[i] = 0;
    ywindow[i] = 0;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  //Calculate position
  int numSensors = sizeof(sensors)/sizeof(sensors[0]);  //confirmed working
  float zsum = 0;
  zsum = 0;
  x = 0;
  y = 0;
  z = 0;

  float distances[numSensors];
  for (int i = 0; i < numSensors; i++) {  //initialize distances to zero
    distances[i] = 0;
  }
  for (int i=0; i < numSensors; i++) {  //store the distance values from each sensor so they don't change between calculations
    distances[i] = sensors[i]->object->dist();
    if (distances[i] > thresh) {
      distances[i] = 0;
    }
    zsum = zsum + distances[i];
  }
  //z sum has been computed to use in weighted average
  for (int i = 0; i < numSensors; i++) {  //compute weighted average of x and y positions by contribution to the z-sum (less contribution = higher weight)
    if (zsum == 0) {  //if none of the sensors are triggered, set x and y to ZERO
      x = 0;
      y = 0;
    } else if (distances[i] == zsum) {  //if the object is directly over one sensor (all contribution to zsum is from one sensor)
      x = sensors[i]->x;
      y = sensors[i]->y;
    }
    else if (distances[i] != 0) { //normal cases, compute weighted sum
      int s1x = sensors[i]->x;
      x = x + sensors[i]->x * (1 - (distances[i] / zsum));
      y = y + sensors[i]->y * (1 - (distances[i] / zsum));  
    }
  }
  //x and y positions have been computed
  //Compute x and y averages
  xavg_prev = xavg;
  yavg_prev = yavg;
  xavg = ((xavg * xyWindow) - xwindow[0] + x) / xyWindow;
  yavg = ((yavg * xyWindow) - ywindow[0] + y) / xyWindow;
  //Pop oldest value and move all values to the left
  for (int i = 0; i < xyWindow-1; i++) {
    xwindow[i] = xwindow[i + 1];
    ywindow[i] = ywindow[i + 1];
  }
  //Update buffer
  xwindow[xyWindow-1] = x;
  ywindow[xyWindow-1] = y;
  //Compute z position from x and y (not x and y averages, this seems pretty robust but it could be better to use averages in the future)
  //Compute the distance from each sensor to the calculated x and y, then use pythagorean theorem to find z
  //Take average of all calculated z values
  z_prev = z;
  int zCounter = 0;
  float xydistance;
  for (int i =0; i < numSensors; i++) {
    xydistance = sqrt( pow((x - sensors[i]->x), 2) + pow((y - sensors[i]->y), 2) );
    float ztemp = pow(distances[i], 2) - pow(xydistance, 2);
    if (ztemp > 0) {  //if ztemp is negative, this sensor does not contribute to the z measurement
      z += sqrt(ztemp);
      zCounter++;
    }
  }
  z = z / zCounter;
  //If xyz have not yet been stored
  //These are not used, so I will comment them out
  // if (x_prev == -1 && y_prev == -1 && z_prev == -1) {
  //   x_prev = x;
  //   y_prev = y;
  //   z_prev = z;
  // }
  //these are using the averages, but could use the instantaneous position if you wanted (although it is very discontinuous)
  dx = ((xavg - xavg_prev) / dt) * 1000;
  dy = ((yavg - yavg_prev) / dt) * 1000;
  dz = ((z - z_prev) / dt) * 1000;
  //store current
  // x_prev = x;
  // y_prev = y;
  // z_prev = z;
  Serial.print("x");
  Serial.println(xavg);
  Serial.print("y");
  Serial.println(yavg);
  Serial.print("z");
  Serial.println(z);
  Serial.print("H");
  Serial.println(dx);
  Serial.print("V");
  Serial.println(dy);
  Serial.print("F");
  Serial.println(dz);
  Serial.print("b");
  Serial.println(body);
  //Serial.print("~")

  delay(dt);   
}
