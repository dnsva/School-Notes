
//LIBRARIES
#include <Adafruit_SH110X.h>
#include <Adafruit_NeoPixel.h>
#include <RotaryEncoder.h>
#include <Wire.h>
#include <Keyboard.h>
#include <string>

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_NEOPIXEL, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);
Adafruit_SH1106G display = Adafruit_SH1106G(128, 64, &SPI1, OLED_DC, OLED_RST, OLED_CS);
RotaryEncoder encoder(PIN_ROTA, PIN_ROTB, RotaryEncoder::LatchMode::FOUR3);

void checkPosition() {  encoder.tick(); }

int encoder_pos = 0; //encoder position state

void setup(){ //set everything up
  Keyboard.begin();
  Serial.begin(115200);
  delay(500); 

  pixels.begin(); //start pixels
  pixels.setBrightness(255);
  pixels.show();

  display.begin(0, true); //start OLED
  display.display();
  
  for (uint8_t i=0; i<=12; i++) {
    pinMode(i, INPUT_PULLUP); //set key to input
  }

  pinMode(PIN_ROTA, INPUT_PULLUP); //inputs
  pinMode(PIN_ROTB, INPUT_PULLUP); 
  attachInterrupt(digitalPinToInterrupt(PIN_ROTA), checkPosition, CHANGE); //interrupts
  attachInterrupt(digitalPinToInterrupt(PIN_ROTB), checkPosition, CHANGE);   

  display.setTextSize(1);
  display.setTextWrap(false);
  display.setTextColor(SH110X_WHITE, SH110X_BLACK); // white text, black background

  pinMode(PIN_SPEAKER_ENABLE, OUTPUT); //enable speaker
  digitalWrite(PIN_SPEAKER_ENABLE, HIGH);
  pinMode(PIN_SPEAKER, OUTPUT); 
  digitalWrite(PIN_SPEAKER, LOW);
}

uint8_t j = 0;
bool i2c_found[128] = {false};
    
void loop() {

  //DISPLAY
  display.clearDisplay();
  display.setCursor(0,0);
  display.println(" *** Sound Maker *** ");

  encoder.tick(); // check the encoder
  int newPos = encoder.getPosition();
  if (encoder_pos != newPos) {
    Serial.print("Encoder:");
    Serial.print(newPos);
    Serial.print(" Direction:");
    Serial.println((int)(encoder.getDirection()));
    encoder_pos = newPos;
  }

  //Specifically for the octaves:
  encoder_pos *= -1;
  if(encoder_pos < 0) encoder_pos = 0;
  else if(encoder_pos > 12) encoder_pos %= 13;
  
  display.setCursor(0, 8);
  display.print("Rotary encoder: ");
  display.print(encoder_pos);

  //Scanning
  if ((j & 0x3F) == 0) {
    Serial.println("Scanning I2C: ");
    Serial.print("Found I2C address 0x");
    for (uint8_t address = 0; address <= 0x7F; address++) {
      Wire.beginTransmission(address);
      i2c_found[address] = (Wire.endTransmission () == 0);
      if (i2c_found[address]) {
        Serial.print("0x");
        Serial.print(address, HEX);
        Serial.print(", ");
      }
    }
    Serial.println();
  }
  display.setCursor(0, 16);
  display.print("I2C Scan: ");
  for (uint8_t address=0; address <= 0x7F; address++) {
    if (!i2c_found[address]) continue;
    display.print("0x");
    display.print(address, HEX);
    display.print(" ");
  }
  
  display.setCursor(0, 24);
  if (!digitalRead(PIN_SWITCH)){ //if encoder pressed
    pixels.setBrightness(150);
  }

  for(int i=0; i<12; i++) { //check for if button pressed & change color
    if(i == 9 || i == 10){ //newline or scale (special)
      pixels.setPixelColor(i, 255, 0, 0);
    }else{
      pixels.setPixelColor(i, (encoder_pos*100)%255, 100, 250);
    }
  }
  for (int i=1; i<=12; i++) {
    if (!digitalRead(i)) { //switch pressed
      Serial.print("Switch "); Serial.println(i);
      pixels.setPixelColor(i-1, 0xFFFFFF);  //make white
      display.setCursor(((i-1) % 3)*48, 32 + ((i-1)/3)*8); //move the text into a 3x4 grid
    }
  }

  String encoder_pos_str = String(encoder_pos);

  if(!digitalRead(1)){ //switch pressed - C
    display.print("C");
    Keyboard.print("C");
    display.print(encoder_pos);
    Keyboard.print(encoder_pos_str);
    Keyboard.print(" "); 
    tone(PIN_SPEAKER, 16.35*(encoder_pos+6), 300); //C
  }
  if(!digitalRead(2)){ //switch pressed - D
    display.print("D");
    Keyboard.print("D");
    display.print(encoder_pos);
    Keyboard.print(encoder_pos_str);
    Keyboard.print(" "); 
    tone(PIN_SPEAKER, 18.35*(encoder_pos+6), 300); //D
  }
  if(!digitalRead(3)){ //switch pressed - E
    display.print("E");
    Keyboard.print("E");
    display.print(encoder_pos);
    Keyboard.print(encoder_pos_str);
    Keyboard.print(" "); 
    tone(PIN_SPEAKER, 20.60*(encoder_pos+6), 300); //E
  }
  if(!digitalRead(4)){ //switch pressed - F
    display.print("F");
    Keyboard.print("F");
    display.print(encoder_pos);
    Keyboard.print(encoder_pos_str);
    Keyboard.print(" "); 
    tone(PIN_SPEAKER,  21.83*(encoder_pos+6), 300); //F
  }
  if(!digitalRead(5)){ //switch pressed - G
    display.print("G");
    Keyboard.print("G");
    display.print(encoder_pos);
    Keyboard.print(encoder_pos_str);
    Keyboard.print(" "); 
    tone(PIN_SPEAKER, 24.50*(encoder_pos+6), 300);  //G
  }
  if(!digitalRead(6)){ //switch pressed - A
    display.print("A");
    Keyboard.print("A");
    display.print(encoder_pos);
    Keyboard.print(encoder_pos_str);
    Keyboard.print(" "); 
    tone(PIN_SPEAKER, 27.50*(encoder_pos+6), 300);  //A
  }
  if(!digitalRead(7)){ //switch pressed - B
    display.print("B");
    Keyboard.print("B");
    display.print(encoder_pos);
    Keyboard.print(encoder_pos_str);
    Keyboard.print(" "); 
    tone(PIN_SPEAKER, 30.87*(encoder_pos+6), 300);  //B
  }
  if(!digitalRead(10)){ //switch pressed - NEWLINE
    display.print("NL");
    Keyboard.print("\n");
  }
  if(!digitalRead(11)){ //switch pressed - PLAY ALL NOTES
    tone(PIN_SPEAKER, 16.35*(encoder_pos+6), 200);  //C
    delay(200);
    tone(PIN_SPEAKER, 18.35*(encoder_pos+6), 200);  //D
    delay(200);
    tone(PIN_SPEAKER, 20.60*(encoder_pos+6), 200);  //E
    delay(200);
    tone(PIN_SPEAKER, 21.83*(encoder_pos+6), 200);  //F
    delay(200);
    tone(PIN_SPEAKER, 24.50*(encoder_pos+6), 200);  //G
    delay(200);
    tone(PIN_SPEAKER, 27.50*(encoder_pos+6), 200);  //A
    delay(200);
    tone(PIN_SPEAKER, 30.87*(encoder_pos+6), 200);  //B
    delay(200);
    display.print("SCALE");
    Keyboard.print("C");
    Keyboard.print(encoder_pos_str);
    Keyboard.print(" "); 
    Keyboard.print("D");
    Keyboard.print(encoder_pos_str);
    Keyboard.print(" ");
    Keyboard.print("E");
    Keyboard.print(encoder_pos_str);
    Keyboard.print(" ");
    Keyboard.print("F");
    Keyboard.print(encoder_pos_str);
    Keyboard.print(" ");
    Keyboard.print("G");
    Keyboard.print(encoder_pos_str);
    Keyboard.print(" ");
    Keyboard.print("A");
    Keyboard.print(encoder_pos_str);
    Keyboard.print(" ");
    Keyboard.print("B");
    Keyboard.print(encoder_pos_str);
    Keyboard.print(" ");
  }

  pixels.show(); //show neopixels
  j++; //increment "swirl"

  display.display(); //display oled

  delay(100); //delay is not a bad idea
}
