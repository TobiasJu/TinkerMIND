// logo animation sketch by TinkerMIND

#include <Adafruit_NeoPixel.h>

#define led_pin D6
const int section_1 = 40;
const int section_2 = 37;
const int section_3 = 40;
const int led_count = section_1 + section_2 + section_3;
const int start_brightness = 70;
const int target_brightness = 150;
bool first_run = true;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(led_count, led_pin, NEO_GRB + NEO_KHZ400);

const uint32_t sun = strip.Color(255, 0, 0); // red
const uint32_t moon = strip.Color(255, 255, 255); // white
const uint32_t stars = strip.Color(0, 50, 200); // bluish

// this runs only once
void setup() {
  Serial.begin(115200);
  strip.begin();
  strip.setBrightness(start_brightness);
  strip.show();
  delay(5000);
  // debug prints
  Serial.print("loop counter");
  Serial.print(",");
  Serial.print("red");
  Serial.print(",");
  Serial.println("white");
}

// this runs forever
void loop() {
  // boot animation
  if (first_run) {
    colorWipeSection(1, sun, 40);
    colorWipeSection(2, moon, 40); 
    colorWipeSection(3, stars, 40); 
    first_run = false;
    delay(2000);
    // slowly increase brightness
    for (int i=start_brightness; i<target_brightness; i++) { strip.setBrightness(i); strip.show(); delay(50); }
  }
  breathe(40, 100);
}

// simple function for setting the whole sting to one color
void setColorForSection(int start, int stop, uint32_t color) {
  for(uint16_t i=0; i<(section_1 + section_2 + section_3); i++) {
    if (i >= start && i <= stop) {
      strip.setPixelColor(i, color);
    }
  }
  strip.show();
}

// render breate effect
void breathe(uint8_t min_brightness, uint8_t max_brightness) {
  //for (int i=min_brightness; i<max_brightness; i++) { strip.setBrightness(i); strip.show(); delay(50); }
  //for (int i=max_brightness; i>min_brightness; i--) { strip.setBrightness(i); strip.show(); delay(50); }
  bool second_run = false;
  for(int i=min_brightness; i<255; i++) {
    // make the transition longer
    if (second_run) {
      i--;
      second_run = false;
    } else {
      second_run = true;
    }
    // debug print
    Serial.println(i);
    Serial.print(",");
    Serial.print(int(i*255/255));
    Serial.print(",");
    int white = int(255-i+min_brightness*255/255);
    int red = int(i*255/255);
    setColorForSection(0, section_1, strip.Color(red, 0, 0)); 
    setColorForSection(section_1, (section_1 + section_2), strip.Color(white, white, white));
    Serial.println(red);
    delay(1); // this has to be here, or the code will crash
  }
  second_run = false;
  for(int i=255; i>min_brightness; i--) {
    // make the transition longer
    if (second_run) {
      i++;
      second_run = false;
    } else {
      second_run = true;
    }
    // debug print
    Serial.println(i);
    Serial.print(",");
    Serial.print(int(i*255/255));
    Serial.print(",");
    int red = int(i*255/255);
    int white = int(255-i+min_brightness*255/255);
    setColorForSection(0, section_1, strip.Color(red, 0, 0));
    setColorForSection(section_1, (section_1 + section_2), strip.Color(white, white, white));
    Serial.println(red);
    delay(1); // this has to be here, or the code will crash
  }
}

// Fill the dots one after the other with a different color for each segment
void colorWipeSection(int sec_number, uint32_t c, uint8_t wait) {
  int counter = 0;
  for(uint16_t i=0; i<(section_1 + section_2 + section_3); i++) {
    if (sec_number == 1 && counter < section_1 ) {
      strip.setPixelColor(i, c);
      delay(wait);
      counter++;
    } else if (sec_number == 2 && counter > section_1 && counter < (section_1 + section_2)) {
      strip.setPixelColor(i, c);
      delay(wait);
      counter++;
    } else if (sec_number == 3 && counter >= (section_1 + section_2)) {
      strip.setPixelColor(i, c);
      delay(wait);
      counter++;
    } else {
      counter++;
    }
    strip.show();
  }
}
