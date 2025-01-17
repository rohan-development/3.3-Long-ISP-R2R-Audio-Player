const unsigned char note [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x7f, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xe0,
  0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00,
  0x01, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x01, 0xff,
  0xff, 0xff, 0xe0, 0x00, 0x00, 0x01, 0xff, 0xff, 0xf8, 0x60, 0x00, 0x00, 0x01, 0xff, 0xff, 0x80,
  0x60, 0x00, 0x00, 0x01, 0xff, 0xf0, 0x00, 0x60, 0x00, 0x00, 0x01, 0xff, 0x00, 0x00, 0x60, 0x00,
  0x00, 0x01, 0xe0, 0x00, 0x00, 0x60, 0x00, 0x00, 0x01, 0xc0, 0x00, 0x00, 0x60, 0x00, 0x00, 0x01,
  0x80, 0x00, 0x00, 0x60, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x60, 0x00, 0x00, 0x01, 0x80, 0x00,
  0x00, 0x60, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x60, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x60,
  0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x60, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x60, 0x00, 0x00,
  0x01, 0x80, 0x00, 0x00, 0x60, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x60, 0x00, 0x00, 0x01, 0x80,
  0x00, 0x00, 0x60, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x60, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00,
  0x60, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x60, 0x00, 0x00, 0x01, 0x80, 0x00, 0x3f, 0x60, 0x00,
  0x00, 0x01, 0x80, 0x00, 0xff, 0xe0, 0x00, 0x00, 0x01, 0x80, 0x03, 0xff, 0xe0, 0x00, 0x00, 0x01,
  0x80, 0x07, 0xff, 0xe0, 0x00, 0x00, 0x01, 0x80, 0x07, 0xff, 0xe0, 0x00, 0x00, 0x01, 0x80, 0x0f,
  0xff, 0xc0, 0x00, 0x00, 0x01, 0x80, 0x0f, 0xff, 0xc0, 0x00, 0x00, 0xfd, 0x80, 0x0f, 0xff, 0x80,
  0x00, 0x03, 0xff, 0x80, 0x07, 0xff, 0x00, 0x00, 0x07, 0xff, 0x80, 0x03, 0xfc, 0x00, 0x00, 0x0f,
  0xff, 0x80, 0x00, 0xe0, 0x00, 0x00, 0x1f, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0x80,
  0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xfe, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x1f, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};                                            //Music note graphic in program memory

#include <Wire.h>                             //I2C library for communication with player MCU
#include <Adafruit_GFX.h>                     //Core graphics library
#include <Adafruit_ST7735.h>                  //ST7735 (TFT) library
#include <SPI.h>                              //SPI Library for TFT display
#define PLAYER 0x55                           //I2C address of player (slave)
#define PIN_A 2                               //Pin "A" of rotary encoder
#define PIN_B 1                               //Pin "B" of rotary encoder
#define TFT_CS    6                           //Chip select of TFT
#define TFT_RST   5                           //Reset pin of TFT
#define TFT_DC    4                           //Data/Command pin of TFT
char songNames[25][33] {};                    //Buffer to hold song names
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST); //TFT object
volatile bool progBar = false;                //Flag to update display progress bar
bool displaying = false;                      //Flag to keep track of displaying status
bool reload = false;                          //Flag for when to reload song index
volatile bool sw = false;                     //Rotary encoder pressed flag
volatile bool pre0 = false;                   //Previous state of rotary encoder switch
bool subtracted = false;                      //Keeps track of whether the index was subtracted
int8_t count = 0;                             //Variable to keep track of current display index
uint16_t numSongs;                            //Number of songs available
volatile bool pinA, pinB;                     //Bools to store states of A and B
uint8_t preA;                                 //Previous pin A state
volatile bool triggered = false;              //Flag for a change in rotary encoder position
volatile uint8_t selectedSong = 0;            //Buffer to store selected song
uint8_t songLocation = 0;                     //Song index
uint16_t songLength = 0;                      //Length of song
volatile uint16_t songProgress = 0;           //Progress through a song

void setup() {
  Wire.begin();
  tft.initR(INITR_BLACKTAB);                  //Initialize TFT panel
  tft.fillScreen(ST77XX_BLACK);               //Fill screen with black
  tft.setTextColor(ST77XX_WHITE);             //Set colour to white
  tft.setTextSize(1);                         //Set text size to 1
  Wire.beginTransmission(PLAYER);             //Start a transmission to the player MCU
  Wire.write(3);                              //Puts a 3 into the code buffer of slave (# of songs)
  Wire.endTransmission();                     //Ends transmission and writes the byte
  Wire.requestFrom(PLAYER, 2);                //Request 2 bytes from slave (# of songs = 2 bytes)
  while (!Wire.available());                  //Wait for bytes to become available
  numSongs = Wire.read() << 8;                //Receive high byte
  numSongs |= Wire.read();                    //Receive low byte
  if (numSongs < 25) getSongs(numSongs);      //When there are les than 25 songs receive all names
  else {                                      //When there are 25 or more than 25, get 24
    getSongs(24);                             //Get 24 song names
    numSongs = 25;                            //Add a 25th row
    strcpy(songNames[24], "Load More...");    //In the 25th row, but a "Load More..." button
  }
  cli();                                      //Clear global interrupts
  TCCR1A = 0;                                 //Normal mode, no PWM
  TCCR1B = 0;                                 //Stop the timer (will start it later)
  TCNT1 = 0;                                  //Clear the timer count
  OCR1A = 62500 - 1;                          //62500 ticks of timer1 = 1/256 second
  TCCR1B |= (1 << WGM12);                     //CTC mode
  TCCR1B |= (1 << CS12);                      //Prescaler 256
  PCICR |= (1 << PCIE2);                      //Enable PCINT for PORTD
  PCMSK2 |= (1 << PCINT16) | (1 << PCINT17);  //Mask for 0 and 1 (pins 0 and 1)
  sei();                                      //Set the global interrupt flag
  attachInterrupt(digitalPinToInterrupt(2), ISR_rotary, CHANGE);  //Interrupt for encoder
  updateScreen();                             //Update the tft screen
  pinA = digitalRead(PIN_A);                  //Get the state of pin A of encoder
  pinB = digitalRead(PIN_B);                  //Get the state of pin B of encoder
  preA = pinA;                                //Put a value into preA
}

void loop() {
  rotaryUpdate();                             //Update the count (if triggered is true)
  if (sw) {                                   //When rotarty encoder select is pressed...
    sw = false;                               //Reset flag
    if (displaying) {                         //If on the "playing" screen, select toggles playback
      Wire.beginTransmission(PLAYER);         //Start a transmission to the slave MCU
      Wire.write(1);                          //Code of 1 means toggle playback
      Wire.endTransmission();                 //End the transmission/send bytes
      TIMSK1 ^= (1 << OCIE1A);                //Toggle the status of the progress bar
    }
    else if (count != 24) {                   //When not on the load more songs line, load selected
      songProgress = 0;                       //Reset progress bar
      cli();                                  //Clear interrupts
      TIMSK1 |= (1 << OCIE1A);                //Start progress bar
      sei();                                  //Start global interrupts again
      Wire.beginTransmission(PLAYER);         //Begin I2C transmission to player MCU
      Wire.write(0);                          //Code of 0 means change song
      Wire.write(count + (24 * (songLocation / 25))); //Send desired song (including the page)
      Wire.endTransmission();                 //End the transmission/send bytes
      Wire.requestFrom(PLAYER, 2);            //Request 2 bytes from the player MCU
      while (!Wire.available());              //Wait for bytes to become available
      songLength = (Wire.read() << 8);        //Receive high byte of length
      songLength |= Wire.read();              //Receive low byte of length
      tft.fillScreen(ST77XX_BLACK);           //Clear the screen
      tft.drawBitmap(39, 30, note, 50, 50, ST77XX_WHITE);   //Draw the music note graphic
      tft.setCursor(14, 120);                 //Go to 14, 120 on the screen
      tft.print(songNames[count]);            //Print the name of the current song playing
      displaying = true;                      //Enable the "displaying" flag
    }
    else reload = true;                       //If it was on the load more button, set the flag
  }
  if (progBar && displaying) {                //When on music note and update flag is set...
    progBar = false;                          //Reset progBar flag
    tft.fillRect(10, 90, 100, 8, ST77XX_BLACK); //Clear the number of elapsed seconds area
    tft.setCursor(14, 90);                    //Go to elapsed seconds area
    tft.print(songProgress);                  //Print elapsed seconds
    tft.setCursor(95, 90);                    //Go to total seconds area
    tft.print(songLength);                    //Print total seconds
    uint8_t mapBuf = map(songProgress, 0, songLength, 0, 100);      //Buffer for mapped progress
    tft.fillRect(14 + mapBuf, 100, 100 - mapBuf, 15, ST77XX_WHITE); //Draw progBar background
    tft.fillRect(14, 100, mapBuf, 15, ST77XX_GREEN);                //Draw progress bar
  }
  if (reload) {                               //If load more button was pressed
    reload = false;                           //Reset flag
    count = 0;                                //Set selector position to 0
    Wire.beginTransmission(PLAYER);           //Start a transmission to the player MCU
    Wire.write(3);                            //Write a code of 3 (# of songs)
    Wire.endTransmission();                   //End transmission/send bytes
    Wire.requestFrom(PLAYER, 2);              //Request 2 bytes from the player
    while (!Wire.available());                //Wait for bytes to become available
    numSongs = Wire.read() << 8;              //Receive high byte
    numSongs |= Wire.read();                  //Receive low byte
    if (numSongs < 25) getSongs(numSongs);    //If number of songs is less than 25, receive all names
    else {                                    //If number of songs is 25 or more...
      getSongs(24);                           //Get 24 songs
      numSongs = 25;                          //Add a 25th row
      strcpy(songNames[24], "Load More...");  //Put a "Load More..." button on row 25
    }
    updateScreen();                           //Update the screen with new songs
  }
}
void getSongs(uint8_t maxSong) {              //Function to get "maxSong" number of song names
  memset(songNames, '\0', sizeof(songNames));   //Clear array
  Wire.beginTransmission(PLAYER);             //Start a transmission to the player MCU
  Wire.write(2);                              //2 means send songs
  Wire.endTransmission();                     //End transmission/send bytes
  uint8_t row;                                //Row index
  for (row = 0; row < maxSong; row++) {       //Loop maxSong number of times
    Wire.requestFrom(PLAYER, 32);             //Request 32 bytes (characters)
    for (uint8_t charac = 0; Wire.available(); charac++)  //Receive however many bytes available
      songNames[row][charac] = Wire.read();   //Receive 1 byte (character) at a time
    songNames[row][19] = '\0';                //Put null terminator into location 19 (display length)
    if (songNames[row][0] == 3) break;        //If received "end of text character", stop the loop
  }
  if (songNames[row][0] != 3)                 //For all valid song names...
    songLocation += maxSong;                  //Update song index
  else songLocation = 0;                      //If invalid, reset the song index to loop back
}
void ISR_rotary() {                           //Rotary encoder update "change" ISR
  triggered = true;                           //Activate 'changed' flag
  pinA = digitalRead(PIN_A);                  //Store state of pin 'A'
  pinB = digitalRead(PIN_B);                  //Store state of pin 'B'

}
ISR(TIMER1_COMPA_vect) {                      //Progress bar update ISR
  progBar = true;                             //Set progress bar update flag
  if (songProgress < songLength)              //If song is playing...
    songProgress++;                           //Add a second to the progress
}
ISR(PCINT2_vect) {                            //Pin change interrupt for PORTD
  if (PIND & (1 << PD0)) {                    //If the encoder switch was high...
    sw = true;                                //Set switch pressed flag
    pre0 = true;                              //Set status flag to detect falling edge
  }
  else if (pre0) pre0 = false;                //Reset status flag on falling edge
  else {                                      //If change was detected on encoder...
    triggered = true;                         //Set encoder changed flag
    pinA = digitalRead(PIN_A);                //Store state of pin A
    pinB = digitalRead(PIN_B);                //Store state of pin B
  }
}
void updateScreen() {                         //Function to update the screen
  tft.fillScreen(ST77XX_BLACK);               //Clear LCD when the displayed string changes
  for (uint8_t i = 0; i < numSongs; i++) {    //Display all valid songs
    tft.setCursor(10, 10 * (i + 1));          //Set cursor to next line
    if ((count + i) < numSongs)               //If the song is valid...
      tft.print(songNames[count + i]);        //Print the song
  }
}
void rotaryUpdate() {                         //Function to detect rotary encoder direction
  if (triggered) {                            //When change flag has tripped...
    triggered = false;                        //Reset change flag
    displaying = false;
    if (pinA != preA)                         //Determine direction (intermediary step)
      preA = pinA;                            //Store previous state
    else {  
      if ((pinA == pinB)) {                   //Determine direction
        if (!subtracted) count++;             //Increment time (direction A)
        else subtracted = false;              //Clear subtracted flag if added
      }
      else {                                  //Other direction
        count--;                              //Decrement time (direction B)
        subtracted = true;                    //Set subtracted flag
      }
      if (count < 0) count = 0;               //Bottom bookend
      if (count >= numSongs) count = numSongs - 1;  //Top bookend
      updateScreen();                         //Update the screen
    }
  }
}
