#include <Wire.h>                     //I2C communication with master
#include <SPI.h>                      //SPI for SDFAT
#include <SdFat.h>                    //SDFAT to read from SD card
#define USE_LONG_FILE_NAMES 1         //Enable Long File Names for SDFAT
#define I2C_ADDR 0x55
/*
   Control codes
   0. Change song/get length
   1. Pause/play
   2. Get songs
   3. Get number of songs
*/
#define chipSelect 8                  //SD chip select pin
#define bufferSize 256                //Size of audio sample buffer, in bytes
SdFat SD;                             //SD object
SdFile audioFile;                     //SD File object for WAV file
uint8_t buffer[bufferSize];           //Buffer to play hold audio samples
volatile uint8_t code;                //Buffer to hold I2C control code
volatile uint8_t song = 0;            //Buffer to hold playing song number
volatile uint16_t bufferLocation = 0; //Variable to store index of audio buffer
uint16_t numSongs;                    //Number of available songs to send to master
volatile bool songChanged = false;    //Flag to keep track of song changes
volatile uint8_t songLocation = 1;    //Index for which song is selected
volatile uint32_t length;             //Buffer to hold length of the song

void setup() {
  Wire.begin(I2C_ADDR);               //Initialize I2C communication
  Wire.onReceive(ISR_onReceive);      //I2C receive ISR definition
  Wire.onRequest(ISR_onRequest);      //I2C request ISR definition
  SD.begin(chipSelect, SPI_FULL_SPEED);  //Initialize SD card with full speed
  SdFile root;                        //Creates SD file object
  root.open("/ROOT", O_RDONLY);       //Initiallizes SD file object with /root directory
  SdFile entry;                       //Creates SD file object
  entry.openNext(&root, O_RDONLY);    //Initiallizes SD file object with "root" file
  char filename[65];                  //Buffer for filename
  for (numSongs = 0; entry.isFile(); numSongs++) {  //Loops to a desired file number
    if (entry.isOpen()) entry.close();//Closes existing file to free memory
    entry.openNext(&root, O_RDONLY);  //Opens next file
    entry.getName(filename, sizeof(filename));  //Puts name of current file into buffer
    if (String(filename).startsWith("._")) numSongs--;  //Discounts hidden files
  }
  DDRD = 0xFF;                        //Set DAC pins as output
  cli();                              //Clear global interrupts
  TCCR1A = 0;                         //Normal mode, no PWM
  TCCR1B = 0;                         //Stop the timer (will start it later)
  TCNT1 = 0;                          //Clear the timer count
  OCR1A = 1000 - 1;                   //1000 ticks of timer1 = 62.5 ms
  TCCR1B |= (1 << WGM12);             //CTC mode
  TCCR1B |= (1 << CS10);              //Prescaler 1 (no prescaler)
  sei();                              //Set the global interrupt flag
}
void loop() {
  if (songChanged) {                      //Changes song when flag true
    songChanged = false;                  //Reset changed flag
    cli();                                //Clear interrupts
    if (audioFile.isOpen()) audioFile.close();  //Close file, if existing
    SdFile root;                          //Create temporary root index file
    root.open("/ROOT", O_RDONLY);         //Assign 'root' to root index
    for (uint8_t i = 0; i <= song; i++) { //Sort through songs to find desired song
      if (audioFile.isOpen()) audioFile.close();  //Closes existing file to free memory
      audioFile.openNext(&root, O_RDONLY);//Opens next file
      char filename[65];                  //Buffer for filename
      audioFile.getName(filename, sizeof(filename));  //Puts name of file into buffer
      if (String(filename).startsWith("._")) i--; //Filter invalid files
    }
    root.close();                               //Close index
    audioFile.seekSet(44);                      //Skip header
    audioFile.read(buffer, bufferSize);         //Put first chunk into buffer
    TIMSK1 |= (1 << OCIE1A);                    //"Press" play
    sei();                                      //Set interrupts again
    if (!audioFile.isOpen()) {                  //If file is invalid, don't play
      TIMSK1 &= ~(1 << OCIE1A);                 //If file is not valid stop playback
      length = 0;                               //Tell master to reset as well (0=invalid)
    }
  }
  if (bufferLocation == bufferSize - 1) {       //Reload audio buffer
    bufferLocation = 0;                         //Initiallize buffer index
    audioFile.read(buffer, bufferSize);         //Place chunk into buffer
  }
  if (!audioFile.available())                   //Check for end of song (handle by master)
    TIMSK1 &= ~(1 << OCIE1A);                   //Stop playback
  PORTD = buffer[bufferLocation];               //Update DAC
}

ISR(TIMER1_COMPA_vect) {
  bufferLocation++;                             //Go to next location in audio buffer
}

void ISR_onRequest() {                          //Request routine (not a true ISR)
  if (code == 0) {                              //Code 0 = song was changed; sends length
    length = (audioFile.fileSize() - 44) / 16000;   //Length = song/bitrate
    Wire.write(length >> 8);                    //Sends high byte of length to master
    Wire.write(length);                         //Sends low byte of length to master
  }
  else if (code == 2) {                         //Code 2 = request song names
    bool playbackState = (TIMSK1 >> OCIE1A) & 1;//Get playback state
    TIMSK1 &= ~(1 << OCIE1A);                   //Pause playback
    SdFile root;                                //Creates SD file object
    root.open("/ROOT", O_RDONLY);               //Initiallizes file object with /root directory
    SdFile entry;                               //Creates SD file object
    char filename[65];                          //Buffer (local) to store filename
    for (uint8_t i = 0; i < songLocation; i++) {//Loops through to certain song number
      if (entry.isOpen()) entry.close();        //Closes previous iteration to free memory
      entry.openNext(&root, O_RDONLY);          //Opens next file
      entry.getName(filename, sizeof(filename));//Places filename into buffer
      if (String(filename).startsWith("._")) i--;//Skips hidden/invalid files
    }
    songLocation++;                             //Increments index
    if (!entry.isOpen()) {                      //Resets when no more files are available
      songLocation = 1;                         //Resets index
      Wire.write(3);                            //Send end of text ASCII character
    }
    else Wire.write(filename);                  //Send filename over I2C to master
    root.close();                               //Close to save memory
    entry.close();                              //Close to save memory
    TIMSK1 |= (playbackState << OCIE1A);        //Restore playback state
  } else if (code == 3) {                       //Code 3 means send number of available songs
    numSongs -= songLocation;                   //Calculate number of available songs
    Wire.write(numSongs >> 8);                  //Send high byte of available songs
    Wire.write(numSongs);                       //Send low byte of available songs
  }
}
void ISR_onReceive(uint8_t bytes_received) {    //Receive routine (true ISR)
  code = Wire.read();                           //First byte received is the command code
  if (!code) {                                  //Code of 0 means change song
    songChanged = true;                         //Set song changed flag
    song = Wire.read();                         //Read the song index sent by the master
  } else if (code == 1) TIMSK1 ^= (1 << OCIE1A);//1 means toggle playback, so toggle OCIE1A
}
