#include <elapsedMillis.h>

const int analogIn1   = A0;
const int analogIn2   = A1;
const int analogPot1  = A2;
const int analogPot2  = A3;
const int analogOut1  = 5;
const int analogOut2  = 6;
const int digitalIn1  = 8;
const int digitalIn2  = 9;
const int digitalOut1 = 3;
const int digitalOut2 = 4;
const int switch1Up   = A4;
const int switch1Down = A5;
const int switch2Up   = 7;
const int switch2Down = 2;

bool gate, resetHigh;
unsigned char in, resetIn;

int divider1, counter1;
int divider2, counter2;
int divider3, counter3;
int divider4, counter4;

elapsedMillis sinceGateOn;

void setup() {
  pinMode(digitalIn1, INPUT);
  pinMode(digitalIn2, INPUT);
  pinMode(digitalOut1, OUTPUT);
  pinMode(digitalOut2, OUTPUT);
  pinMode(switch1Up, INPUT_PULLUP);
  pinMode(switch1Down, INPUT_PULLUP);
  pinMode(switch2Up, INPUT_PULLUP);
  pinMode(switch2Down, INPUT_PULLUP);

  analogWrite(analogOut1, 255);
  analogWrite(analogOut2, 255);

  gate = false;
  resetHigh = false;

  resetCounters();
  setupDividers();
}

void out1On() { digitalWrite(digitalOut1, HIGH); }
void out1Off() { digitalWrite(digitalOut1, LOW); }
void out2On() { digitalWrite(digitalOut2, HIGH); }
void out2Off() { digitalWrite(digitalOut2, LOW); }
void out3On() { analogWrite(analogOut1, 0); }
void out3Off() { analogWrite(analogOut1, 255); }
void out4On() { analogWrite(analogOut2, 0); }
void out4Off() { analogWrite(analogOut2, 255); }

void allOutputsLow() {
  out1Off();
  out2Off();
  out3Off();
  out4Off();
}

void resetCounters() {
  counter1 = 1; counter2 = 1;
  counter3 = 1; counter4 = 1;
}

void setupDividers() {
  if(digitalRead(switch1Up) == LOW) {
    divider1 = 1;
    divider2 = 2;
    divider3 = 3;
    divider4 = 4;
  } else if (digitalRead(switch1Down) == LOW) {
    divider1 = 1;
    divider2 = 2;
    divider3 = 4;
    divider4 = 8;
  } else {
    divider1 = 1;
    divider2 = 3;
    divider3 = 5;
    divider4 = 7;
  }
}

void divide() {
  if(counter1 >= divider1) {
    out1On();
    counter1 = 0;
  }

  if(counter2 >= divider2) {
    out2On();
    counter2 = 0;
  }

  if(counter3 >= divider3) {
    out3On();
    counter3 = 0;
  }

  if(counter4 >= divider4) {
    out4On();
    counter4 = 0;
  }

  counter1++; counter2++;
  counter3++; counter4++;
}

void clockDivider() {
  in = digitalRead(digitalIn1);
  resetIn = digitalRead(digitalIn2);

  if(resetIn == HIGH && resetHigh == false) {
    resetHigh = true;
    resetCounters();
  }
  if(resetIn == LOW && resetHigh == true) {
    resetHigh = false;
  }

  if(in == HIGH && gate == false) {
    gate = true;
    setupDividers();
    divide();
  } else if (in == LOW && gate == true) {
    gate = false;
    allOutputsLow();
  }
}

void clockSource() {
  unsigned long gateOnDuration = analogRead(analogPot1);
  unsigned long gateOffDuration = analogRead(analogPot2);
  unsigned long fullCycle = gateOnDuration + gateOffDuration;

  resetIn = digitalRead(digitalIn2);

  if(resetIn == HIGH && resetHigh == false) {
    resetHigh = true;
    sinceGateOn = fullCycle;
    resetCounters();
    allOutputsLow();
  }
  if(resetIn == LOW && resetHigh == true) {
    resetHigh = false;
  }

  if(sinceGateOn >= fullCycle) {
    sinceGateOn = 0;
    divide();
  }
  if(sinceGateOn >= gateOnDuration) {
    allOutputsLow();
    setupDividers();
  }
}

void loop() {
  if(digitalRead(switch2Down) == LOW) {
    clockSource();
  } else {
    clockDivider();
  }
}
