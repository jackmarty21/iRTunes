#include <FatReader.h>
#include <SdReader.h>
#include <avr/pgmspace.h>
#include "WaveUtil.h"
#include "WaveHC.h"
//#include <IRremote.h>
//
//int RECV_PIN = 11;
//
//IRrecv irrecv(RECV_PIN);
//
//decode_results results;


SdReader card;    // This object holds the information for the card
FatVolume vol;    // This holds the information for the partition on the card
FatReader root;   // This holds the information for the filesystem on the card
FatReader f;      // This holds the information for the file we're play

WaveHC wave;      // This is the only wave (audio) object, since we will only play one at a time

//#define DEBOUNCE 5  // button debouncer

// here is where we define the buttons that we'll use. button "1" is the first, button "6" is the 6th, etc
byte buttons[] = {14, 15, 16, 17, 18, 19};
// This handy macro lets us determine how big the array up above is, by checking the size
#define NUMBUTTONS sizeof(buttons)
// we will track if a button is just pressed, just released, or 'pressed' (the current state
volatile byte pressed[NUMBUTTONS], justpressed[NUMBUTTONS], justreleased[NUMBUTTONS];

//char mystr[3];
int writeInfo;

// this handy function will return the number of bytes currently free in RAM, great for debugging!   
int freeRam(void)
{
  extern int  __bss_end; 
  extern int  *__brkval; 
  int free_memory; 
  if((int)__brkval == 0) {
    free_memory = ((int)&free_memory) - ((int)&__bss_end); 
  }
  else {
    free_memory = ((int)&free_memory) - ((int)__brkval); 
  }
  return free_memory; 
} 

void sdErrorCheck(void)
{
  if (!card.errorCode()) return;
  putstring("\n\rSD I/O error: ");
  Serial.print(card.errorCode(), HEX);
  putstring(", ");
  Serial.println(card.errorData(), HEX);
  while(1);
}

void setup() {
  byte i;
  
  // set up serial port
  Serial.begin(9600);
  putstring_nl("WaveHC with ");
  Serial.print(NUMBUTTONS, DEC);
  putstring_nl("buttons");
  
  putstring("Free RAM: ");       // This can help with debugging, running out of RAM is bad
  Serial.println(freeRam());      // if this is under 150 bytes it may spell trouble!
  
  // Set the output pins for the DAC control. This pins are defined in the library
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, INPUT);
 
  // pin13 LED
  pinMode(13, OUTPUT);
  pinMode(13, INPUT);
  pinMode(13, INPUT);
  pinMode(13, INPUT);
  pinMode(13, INPUT);
  pinMode(13, INPUT);
 
  // Make input & enable pull-up resistors on switch pins
  for (i=0; i< NUMBUTTONS; i++) {
    pinMode(buttons[i], INPUT);
    digitalWrite(buttons[i], HIGH);
  }
  
  //  if (!card.init(true)) { //play with 4 MHz spi if 8MHz isn't working for you
  if (!card.init()) {         //play with 8 MHz spi (default faster!)  
    putstring_nl("Card init. failed!");  // Something went wrong, lets print out why
    sdErrorCheck();
    while(1);                            // then 'halt' - do nothing!
  }
  
  // enable optimize read - some cards may timeout. Disable if you're having problems
  card.partialBlockRead(true);
 
// Now we will look for a FAT partition!
  uint8_t part;
  for (part = 0; part < 5; part++) {     // we have up to 5 slots to look in
    if (vol.init(card, part)) 
      break;                             // we found one, lets bail
  }
  if (part == 5) {                       // if we ended up not finding one  :(
    putstring_nl("No valid FAT partition!");
    sdErrorCheck();      // Something went wrong, lets print out why
    while(1);                            // then 'halt' - do nothing!
  }
  
  // Lets tell the user about what we found
  putstring("Using partition ");
  Serial.print(part, DEC);
  putstring(", type is FAT");
  Serial.println(vol.fatType(),DEC);     // FAT16 or FAT32?
  
  // Try to open the root directory
  if (!root.openRoot(vol)) {
    putstring_nl("Can't open root dir!"); // Something went wrong,
    while(1);                             // then 'halt' - do nothing!
  }
  
  // Whew! We got past the tough parts.
  putstring_nl("Ready!");
  
  TCCR2A = 0;
  TCCR2B = 1<<CS22 | 1<<CS21 | 1<<CS20;

  //Timer2 Overflow Interrupt Enable
  TIMSK2 |= 1<<TOIE2;


}

SIGNAL(TIMER2_OVF_vect) {
  check_switches();
}

void check_switches()
{
  static byte previousstate[NUMBUTTONS];
  static byte currentstate[NUMBUTTONS];
  byte index;

  for (index = 0; index < NUMBUTTONS; index++) {
    currentstate[index] = digitalRead(buttons[index]);   // read the button
    
    /*     
    Serial.print(index, DEC);
    Serial.print(": cstate=");
    Serial.print(currentstate[index], DEC);
    Serial.print(", pstate=");
    Serial.print(previousstate[index], DEC);
    Serial.print(", press=");
    */
    
    if (currentstate[index] == previousstate[index]) {
      if ((pressed[index] == LOW) && (currentstate[index] == LOW)) {
          // just pressed
          justpressed[index] = 1;
      }
      else if ((pressed[index] == HIGH) && (currentstate[index] == HIGH)) {
          // just released
          justreleased[index] = 1;
      }
      pressed[index] = !currentstate[index];  // remember, digital HIGH means NOT pressed
    }
    //Serial.println(pressed[index], DEC);
    previousstate[index] = currentstate[index];   // keep a running tally of the buttons
  }
}

void loop() {
  
  writeInfo = Serial.read();
  if (writeInfo > -1) {
    Serial.println(writeInfo);
    if (writeInfo == 0){
      Serial.println("Playing C1P.WAV");
      playcomplete("C1P.WAV");
    }
    else if (writeInfo == 1) {
      Serial.println("Playing C#1P.WAV");
      playcomplete("C#1P.WAV");
    }
    else if (writeInfo == 2) {
      Serial.println("Playing D1P.WAV");
      playcomplete("D1P.WAV");
    }
    else if (writeInfo == 3) {
      Serial.println("Playing D#1P.WAV");
      playcomplete("D#1P.WAV");
    }
    else if (writeInfo == 4) {
      Serial.println("Playing E1P.WAV");
      playcomplete("E1P.WAV");
    }
    else if (writeInfo == 5) {
      Serial.println("Playing F1P.WAV");
      playcomplete("F1P.WAV");
    }
    else if (writeInfo == 6) {
      Serial.println("Playing F#1P.WAV");
      playcomplete("F#1P.WAV");
    }
    else if (writeInfo == 7) {
      Serial.println("Playing G1P.WAV");
      playcomplete("G1P.WAV");
    }
    else if (writeInfo == 8) {
      Serial.println("Playing G#1P.WAV");
      playcomplete("G#1P.WAV");
    }
    else if (writeInfo == 9) {
      Serial.println("Playing A1P.WAV");
      playcomplete("A1P.WAV");
    }
    else if (writeInfo == 10) {
      Serial.println("Playing A#1P.WAV");
      playcomplete("A#1P.WAV");
    }
    else if (writeInfo == 11) {
      Serial.println("Playing B1P.WAV");
      playcomplete("B1P.WAV");
    }
    else if (writeInfo == 12){
      Serial.println("Playing C2P.WAV");
      playcomplete("C2P.WAV");
    }
    else if (writeInfo == 13) {
      Serial.println("Playing C#2P.WAV");
      playcomplete("C#2P.WAV");
    }
    else if (writeInfo == 14) {
      Serial.println("Playing D2P.WAV");
      playcomplete("D2P.WAV");
    }
    else if (writeInfo == 15) {
      Serial.println("Playing D#2P.WAV");
      playcomplete("D#2P.WAV");
    }
    else if (writeInfo == 16) {
      Serial.println("Playing E2P.WAV");
      playcomplete("E2P.WAV");
    }
    else if (writeInfo == 17) {
      Serial.println("Playing F2P.WAV");
      playcomplete("F2P.WAV");
    }
    else if (writeInfo == 18) {
      Serial.println("Playing F#2P.WAV");
      playcomplete("F#2P.WAV");
    }
    else if (writeInfo == 19) {
      Serial.println("Playing G2P.WAV");
      playcomplete("G2P.WAV");
    }
    else if (writeInfo == 20) {
      Serial.println("Playing G#2P.WAV");
      playcomplete("G#2P.WAV");
    }
    else if (writeInfo == 21) {
      Serial.println("Playing A2P.WAV");
      playcomplete("A2P.WAV");
    }
    else if (writeInfo == 22) {
      Serial.println("Playing A#2P.WAV");
      playcomplete("A#2P.WAV");
    }
    else if (writeInfo == 23) {
      Serial.println("Playing B2P.WAV");
      playcomplete("B2P.WAV");
    }
    else if (writeInfo == 24){
      Serial.println("Playing C3P.WAV");
      playcomplete("C3P.WAV");
    }
    else if (writeInfo == 25) {
      Serial.println("Playing C#3P.WAV");
      playcomplete("C#3P.WAV");
    }
    else if (writeInfo == 26) {
      Serial.println("Playing D3P.WAV");
      playcomplete("D3P.WAV");
    }
    else if (writeInfo == 27) {
      Serial.println("Playing D#3P.WAV");
      playcomplete("D#3P.WAV");
    }
    else if (writeInfo == 28) {
      Serial.println("Playing E3P.WAV");
      playcomplete("E3P.WAV");
    }

    // Memory Issues
//    else if (writeInfo == 29) {
//      Serial.println("Playing F3P.WAV");
//      playcomplete("F3P.WAV");
//    }
//    else if (writeInfo == 30) {
//      Serial.println("Playing F#3P.WAV");
//      playcomplete("F#3P.WAV");
//    }
//    else if (writeInfo == 31) {
//      Serial.println("Playing G3P.WAV");
//      playcomplete("G3P.WAV");
//    }
//    else if (writeInfo == 32) {
//      Serial.println("Playing G#3P.WAV");
//      playcomplete("G#3P.WAV");
//    }
//    else if (writeInfo == 33) {
//      Serial.println("Playing A3P.WAV");
//      playcomplete("A3P.WAV");
//    }
//    else if (writeInfo == 34) {
//      Serial.println("Playing A#3P.WAV");
//      playcomplete("A#3P.WAV");
//    }
//    else if (writeInfo == 35) {
//      Serial.println("Playing B3P.WAV");
//      playcomplete("B3P.WAV");
//    }
  }
  
}



// Plays a full file from beginning to end with no pause.
void playcomplete(char *name) {
  // call our helper to find and play this name
  playfile(name);
//  while (wave.isplaying) {
//  // do nothing while its playing
//  }
  // now its done playing
}

void playfile(char *name) {
  // see if the wave object is currently doing something
  if (wave.isplaying) {// already playing something, so stop it!
    wave.stop(); // stop it
  }
  // look in the root directory and open the file
  if (!f.open(root, name)) {
    putstring("Couldn't open file "); Serial.print(name); return;
  }
  // OK read the file and turn it into a wave object
  if (!wave.create(f)) {
    putstring_nl("Not a valid WAV"); return;
  }
  
  // ok time to play! start playback
  wave.play();
}
