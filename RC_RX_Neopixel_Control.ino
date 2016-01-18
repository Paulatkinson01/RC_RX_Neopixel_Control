#include <Adafruit_NeoPixel.h>
#define PIXEL_PIN 6                 // Pin connected to pixels
#define PIXEL_COUNT 16              // Number of Pixels
const int numReadings = 10;
int rxIn = 7;                       // Use a 10K pullup resistor
int readings[numReadings];          // the readings from the analog input
int readIndex = 0;                  // the index of the current reading
int total = 0;                      // the running total
int average = 0;
unsigned long duration;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);


void setup()
{
  strip.begin();
  strip.show();
  pinMode(rxIn, INPUT);
  Serial.begin(9600);
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }
}


// Main loop

void loop()
{

  total = total - readings[readIndex];
  readings[readIndex] = pulseIn(rxIn, HIGH);
  total = total + readings[readIndex];
  readIndex = readIndex + 1;
  
  if (readIndex >= numReadings) {    
    readIndex = 0;
  }

  average = total / numReadings;
  Serial.println(average);

  //duration = pulseIn(rxIn, HIGH);
  
  if(average < 1100){ // PWM lower than 1100 use this mode, else the function that matches below.
    colorWipe(strip.Color(255, 0, 0), 50); // Red
    colorWipe(strip.Color(0, 255, 0), 50); // Green
    colorWipe(strip.Color(0, 0, 255), 50); // Blue
    Serial.println("colorWipe");
  }
  else
  if(average > 1100 && average < 1300)
  {
    rainbow(20);
    Serial.println("rainbow");
  }
  else
  if(average > 1300 && average < 1500)
  {
    rainbowCycle(20);
    Serial.println("rainbowCycle");
  }
  else
  if(average > 1500 && average < 1700)
  {
    theaterChaseRainbow(50);
    Serial.println("theaterChaseRainbow");
  }
  else
  if(average > 1700)
  {
    theaterChase(strip.Color(127, 127, 127), 50); // White
    theaterChase(strip.Color(127,   0,   0), 50); // Red
    theaterChase(strip.Color(  0,   0, 127), 50); // Blue
    Serial.println("Function 5");
  }
  
  delay(400);
 
}



// defining functions used in the main loop

void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}


void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();
     
      delay(wait);
     
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
        }
        strip.show();
       
        delay(wait);
       
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, 0);        //turn every third pixel off
        }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if(WheelPos < 170) {
    WheelPos -= 85;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}
