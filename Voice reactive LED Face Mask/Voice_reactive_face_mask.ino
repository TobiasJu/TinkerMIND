//this code is for this mask: https://youtu.be/K4tvNI47nhw

#include <Adafruit_NeoPixel.h>
#include <math.h>
#define micPin A0


// custom anomations feel free to add as many as you want
uint8_t mouth_0[4][8] = {
    {0,0,0,0,0,0,0,0},
    {6,6,6,6,6,6,6,6},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0}
};

uint8_t mouth_4[4][8] = {
    {0,6,6,6,6,6,6,0},
    {6,0,0,0,0,0,0,6},
    {6,0,0,0,0,0,0,6},
    {0,6,6,6,6,6,6,0}
};

uint8_t mouth_3[4][8] = {
    {0,0,6,6,6,6,0,0},
    {0,6,0,0,0,0,6,0},
    {0,6,0,0,0,0,6,0},
    {0,0,6,6,6,6,0,0},
};

uint8_t mouth_2[4][8] = {
    {0,6,6,6,6,6,6,0},
    {6,0,0,0,0,0,0,6},
    {0,6,6,6,6,6,6,0},
    {0,0,0,0,0,0,0,0}
};

uint8_t mouth_1[4][8] = {
    {0,0,0,6,6,0,0,0},
    {6,6,6,0,0,6,6,6},
    {0,0,0,6,6,0,0,0},
    {0,0,0,0,0,0,0,0}
};

// some custom parameters, depending on your microphone and controller
const uint16_t samples = 512;
const uint16_t offset = 540;
const uint16_t gain = 1;
uint16_t palette[8] = {};
const int ledPin = 2;
const int ledCount = 32;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(ledCount, ledPin, NEO_RGB + NEO_KHZ400);


// this function sets the leds to the right color
void drawImage(uint8_t image_addr[][8]){
    int pixel_nr = 0;
    for(int x = 0; x<4; x++){
        for(int y = 0; y<8; y++){
            int index = image_addr[x][y];
            // int index = 0;
            strip.setPixelColor(pixel_nr, palette[index]);
            pixel_nr += 1;
        }
    }
    strip.show();
}


// this functions processes the raw input value of the microphone
uint16_t smooth(){
  float vol = 0;
  int nvol = 0;
  int previous_peak = -1;
  float vol_sum = 0;
  for (int i = 0; i<samples; i++){
      int analog_value = analogRead(micPin) - offset;
      // Serial.println(analog_value);
      if (analog_value <= 0){
        continue;
      }
      vol_sum += analog_value * gain;
  }
  vol = vol_sum/samples;
  return vol;
}


void setup() {
    strip.begin();

    // i wanted to add push button to switch the color/animations but i did not, lol
    palette[0] = strip.Color(200,0,0);
    palette[1] = strip.Color(100,0,0);
    palette[2] = strip.Color(0,255,0);
    palette[3] = strip.Color(0,0,255);
    palette[4] = strip.Color(0,255,255);
    palette[5] = strip.Color(5,0,5);
    palette[6] = strip.Color(100,100,0);
    palette[7] = strip.Color(255,255,255);


    // debug output
    Serial.begin(115200);
    delay(1000);
    Serial.println("raw,abs,nvol");
}


void loop() {
  uint16_t vol = smooth();
  Serial.println(vol);

 if(vol < 10){
      drawImage(mouth_0);
  } else if(vol < 20){
      drawImage(mouth_1);
  } else if(vol < 40){
      drawImage(mouth_2);
  } else if(vol < 50){
      drawImage(mouth_3);
  } else {
      drawImage(mouth_4);
  }
}
