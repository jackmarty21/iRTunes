#include <IRremote.h>
#include <NewTone.h>

int RECV_PIN = 11;
int LOOP_PIN = 12;

IRrecv irrecv(RECV_PIN);

decode_results results;

int i = 0;
int elapsedTime;
int intervalCounter;
int loopSize = 0;
int intervalMeasure;
int loopStartTime;
int testTime;
int cDuration;
int dDuration; 
int eDuration;
int gDuration;
int loopEndTime;
float c = 65.41;
float d = 73.42;
float e = 82.41;
float g = 98;

//float loopArray[100];
bool musicLoop = false;

typedef struct
 {
     int frequency;
     int beforeInterval;
     int duration;
 }  note_struct;

note_struct notes[100];


void setup()
{
  Serial.begin(9600);
  notes[0].frequency = -1;
  // In case the interrupt driver crashes on setup, give a clue
  // to the user what's going on.

  pinMode(LOOP_PIN, OUTPUT);

  digitalWrite(LOOP_PIN, LOW);


  //Serial.println("Enabling IRin");
  irrecv.enableIRIn(); // Start the receiver
  //Serial.println("Enabled IRin");
}

void loop() {
  
  if ((notes[0].frequency > -1) && (musicLoop == false)) {
    for (int j=0; j<loopSize; j++) {
      if (notes[j].beforeInterval > -1) {
        delay(notes[j].beforeInterval);
        Serial.write(notes[j].frequency);
        delay(300);
      }
    }
    //delay(300);
  }
  //IR Signal
  if (irrecv.decode(&results)) {
    //Serial.println(results.value, HEX); 
    

    //The Loop Start
    if ((results.value == 0xFFE21D) && (musicLoop == false)) {
      //Serial.println("here");
      musicLoop = true;
      digitalWrite(LOOP_PIN, HIGH);
      loopStartTime = millis();
      
    }
    //The Loop End
    else if ((results.value == 0xFFE21D) && (musicLoop == true)) {
      //Serial.println("Loop Ended");
      loopSize = i;
      loopEndTime = millis();
      digitalWrite(LOOP_PIN, LOW);
      musicLoop = false;
      i = 0;
    }
    
    //clear loop
    if ((results.value == 0xFF629D)) {
      musicLoop = false;
      digitalWrite(LOOP_PIN, LOW);
      delay(100);
      digitalWrite(LOOP_PIN, HIGH);
      delay(100);
      digitalWrite(LOOP_PIN, LOW);
      delay(100);
      digitalWrite(LOOP_PIN, HIGH);
      delay(100);
      digitalWrite(LOOP_PIN, LOW);
      delay(100);
      notes[0].frequency = -1;
      loopSize = 0;
      i=0;
      intervalCounter = 0;
    }
    if (musicLoop == true) {
        
      //For Tones
      //C
      if (results.value == 0xFF6897) {
        notes[i].frequency = 0;

        //add beforeInterval
        if (i == 0) {
          testTime = millis();
          intervalMeasure = testTime - loopStartTime;
          notes[i].beforeInterval = intervalMeasure;
        }
        else {
          testTime = millis();
          elapsedTime = testTime - loopStartTime;
          intervalMeasure = elapsedTime - intervalCounter;
          notes[i].beforeInterval = intervalMeasure;
        }
        
        intervalCounter += notes[i].beforeInterval + 300;
        i++;
      }
      //C#
      else if (results.value == 0xFF9867) {
        notes[i].frequency = 1;

        //add beforeInterval
        if (i == 0) {
          testTime = millis();
          intervalMeasure = testTime - loopStartTime;
          notes[i].beforeInterval = intervalMeasure;
        }
        else {
          testTime = millis();
          elapsedTime = testTime - loopStartTime;
          intervalMeasure = elapsedTime - intervalCounter;
          notes[i].beforeInterval = intervalMeasure;
        }
        intervalCounter += notes[i].beforeInterval + 300;
        i++;
      }
      //D
      else if (results.value == 0xFFB04F) {
        //Serial.println("Tone added to Loop");
        notes[i].frequency = 2;

        //add beforeInterval
        if (i == 0) {
          testTime = millis();
          intervalMeasure = testTime - loopStartTime;
          notes[i].beforeInterval = intervalMeasure;
        }
        else {
          testTime = millis();
          elapsedTime = testTime - loopStartTime;
          intervalMeasure = elapsedTime - intervalCounter;
          notes[i].beforeInterval = intervalMeasure;
        }
        intervalCounter += notes[i].beforeInterval + 300;
        i++;
      }
      //D#
      else if (results.value == 0xFF30CF) {
        //Serial.println("Tone added to Loop");
        notes[i].frequency = 3;

        //add beforeInterval
        if (i == 0) {
          testTime = millis();
          intervalMeasure = testTime - loopStartTime;
          notes[i].beforeInterval = intervalMeasure;
        }
        else {
          testTime = millis();
          elapsedTime = testTime - loopStartTime;
          intervalMeasure = elapsedTime - intervalCounter;
          notes[i].beforeInterval = intervalMeasure;
        }
        intervalCounter += notes[i].beforeInterval + 300;
        i++;
      }
      //E
      else if (results.value == 0xFF18E7) {
        //Serial.println("Tone added to Loop");
        notes[i].frequency = 4;

        //add beforeInterval
        if (i == 0) {
          testTime = millis();
          intervalMeasure = testTime - loopStartTime;
          notes[i].beforeInterval = intervalMeasure;
        }
        else {
          testTime = millis();
          elapsedTime = testTime - loopStartTime;
          intervalMeasure = elapsedTime - intervalCounter;
          notes[i].beforeInterval = intervalMeasure;
        }
        intervalCounter += notes[i].beforeInterval + 300;
        i++;
      }
      //F
      else if (results.value == 0xFF7A85) {
        //Serial.println("Tone added to Loop");
        notes[i].frequency = 5;

        //add beforeInterval
        if (i == 0) {
          testTime = millis();
          intervalMeasure = testTime - loopStartTime;
          notes[i].beforeInterval = intervalMeasure;
        }
        else {
          testTime = millis();
          elapsedTime = testTime - loopStartTime;
          intervalMeasure = elapsedTime - intervalCounter;
          notes[i].beforeInterval = intervalMeasure;
        }
        intervalCounter += notes[i].beforeInterval + 300;
        i++;
      }
      //F#
      else if (results.value == 0xFF10EF) {
        //Serial.println("Tone added to Loop");
        notes[i].frequency = 6;

        //add beforeInterval
        if (i == 0) {
          testTime = millis();
          intervalMeasure = testTime - loopStartTime;
          notes[i].beforeInterval = intervalMeasure;
        }
        else {
          testTime = millis();
          elapsedTime = testTime - loopStartTime;
          intervalMeasure = elapsedTime - intervalCounter;
          notes[i].beforeInterval = intervalMeasure;
        }
        intervalCounter += notes[i].beforeInterval + 300;
        i++;
      }
      //G
      else if (results.value == 0xFF38C7) {
        //Serial.println("Tone added to Loop");
        notes[i].frequency = 7;

        //add beforeInterval
        if (i == 0) {
          testTime = millis();
          intervalMeasure = testTime - loopStartTime;
          notes[i].beforeInterval = intervalMeasure;
        }
        else {
          testTime = millis();
          elapsedTime = testTime - loopStartTime;
          intervalMeasure = elapsedTime - intervalCounter;
          notes[i].beforeInterval = intervalMeasure;
        }
        intervalCounter += notes[i].beforeInterval + 300;
        i++;
      }
      //G#
      else if (results.value == 0xFF5AA5) {
        //Serial.println("Tone added to Loop");
        notes[i].frequency = 8;

        //add beforeInterval
        if (i == 0) {
          testTime = millis();
          intervalMeasure = testTime - loopStartTime;
          notes[i].beforeInterval = intervalMeasure;
        }
        else {
          testTime = millis();
          elapsedTime = testTime - loopStartTime;
          intervalMeasure = elapsedTime - intervalCounter;
          notes[i].beforeInterval = intervalMeasure;
        }
        intervalCounter += notes[i].beforeInterval + 300;
        i++;
      }
      //A
      else if (results.value == 0xFF42BD) {
        //Serial.println("Tone added to Loop");
        notes[i].frequency = 9;

        //add beforeInterval
        if (i == 0) {
          testTime = millis();
          intervalMeasure = testTime - loopStartTime;
          notes[i].beforeInterval = intervalMeasure;
        }
        else {
          testTime = millis();
          elapsedTime = testTime - loopStartTime;
          intervalMeasure = elapsedTime - intervalCounter;
          notes[i].beforeInterval = intervalMeasure;
        }
        intervalCounter += notes[i].beforeInterval + 300;
        i++;
      }
      //A#
      else if (results.value == 0xFF4AB5) {
        //Serial.println("Tone added to Loop");
        notes[i].frequency = 10;

        //add beforeInterval
        if (i == 0) {
          testTime = millis();
          intervalMeasure = testTime - loopStartTime;
          notes[i].beforeInterval = intervalMeasure;
        }
        else {
          testTime = millis();
          elapsedTime = testTime - loopStartTime;
          intervalMeasure = elapsedTime - intervalCounter;
          notes[i].beforeInterval = intervalMeasure;
        }
        intervalCounter += notes[i].beforeInterval + 300;
        i++;
      }
      //B
      else if (results.value == 0xFF52AD) {
        //Serial.println("Tone added to Loop");
        notes[i].frequency = 11;

        //add beforeInterval
        if (i == 0) {
          testTime = millis();
          intervalMeasure = testTime - loopStartTime;
          notes[i].beforeInterval = intervalMeasure;
        }
        else {
          testTime = millis();
          elapsedTime = testTime - loopStartTime;
          intervalMeasure = elapsedTime - intervalCounter;
          notes[i].beforeInterval = intervalMeasure;
        }
        intervalCounter += notes[i].beforeInterval + 300;
        i++;
      }
    }
    
    //For Tones

    //C
    if (results.value == 0xFF6897) { 
      Serial.write(0);  
    }
    //C#
    else if (results.value == 0xFF9867) {
      Serial.write(1); 
    }
    //D
    else if (results.value == 0xFFB04F) {
      Serial.write(2); 
    }
    //D#
    else if (results.value == 0xFF30CF) {
      Serial.write(3); 
    }
    //E
    else if (results.value == 0xFF18E7) {
      Serial.write(4);
    }
    //F
    else if (results.value == 0xFF7A85) {
      Serial.write(5);
    }
    //F#
    else if (results.value == 0xFF10EF) {
      Serial.write(6);
    }
    //G
    else if (results.value == 0xFF38C7) {
      Serial.write(7);
    }
    //G#
    else if (results.value == 0xFF5AA5) {
      Serial.write(8);
    }
    //A
    else if (results.value == 0xFF42BD) {
      Serial.write(9);
    }
    //A#
    else if (results.value == 0xFF4AB5) {
      Serial.write(10);
    }
    //B
    else if (results.value == 0xFF52AD) {
      Serial.write(11);
    }
    
    irrecv.resume(); // Receive the next value
  }
}
