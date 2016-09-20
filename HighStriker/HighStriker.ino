/* High Striker Code
    Last Update: Knotfest 2016 (9/19/2016)

    Sean Cusack for Sheet Metal Alchemist
*/

#include <Encoder.h>

Encoder knobLeft(2, 3);
long timer = 0;
long delayTimer = 0;
long oldLeft = -999;
long newLeft = -999;
int flameLevel = 0;
int flameFlag = 0;
const int poof1 = 8;
const int poof2 = 9;
const int poof3 = 10;
const int poof4 = 11;
const int poof5 = 12;


int travelPos = 300;        //MAKE THIS NUMBER HIGHER TO COUNT TO A LONGER ARM THROW
int deadZone = 5;           //THIS DEFINES THE ERROR IN THE ROTARY ENCODER. INCREASING IT ALLOWS FOR MORE WIGGLE IN THE STRIKE PLATE BEFORE READING
int counterDelay = 1000;    //THE AMOUNT OF TIME IN uS THAT THE COMPUTER WAITS BETWEEN COUNTS. IF THIS GETS SMALLER, THE COUNTS TO ROTATE WILL APPEAR TO INCREASE, AND IT WILL INCREASE THE DIFFERENTIATION BETWEEN LEVEL HITS
long staleEnc = 1000000;    //THE AMOUNT OF TIME UNTIL THE COUNTER RESETS TO COMPENSATE FOR DRIFT
int thresh1 = 300;          //THE TIMER VAL TO HIT A LEVEL 1 FIREFX (MAKE THIS LOWER TO MAKE GAME HARDER)
int thresh2 = 100;          //THE TIMER VAL TO HIT A LEVEL 2 FIREFX (MAKE THIS LOWER TO MAKE GAME HARDER)
int thresh3 = 65;           //THE TIMER VAL TO HIT A LEVEL 3 FIREFX (MAKE THIS LOWER TO MAKE GAME HARDER)
int thresh4 = 58;           //THE TIMER VAL TO HIT A LEVEL 4 FIREFX (MAKE THIS LOWER TO MAKE GAME HARDER)
int thresh5 = 52;           //THE TIMER VAL TO HIT A LEVEL 5 FIREFX (MAKE THIS LOWER TO MAKE GAME HARDER)
int settlingBreak = 4000;   //THE TIME FROM AFTER THE FIRE EFFECTS GO OFF UNTIL THE DEVICE IS ARMED AGAIN. THE TIME FOR THE STRIKE PLATE TO SETTLE
int poofLag = 250;          //THE TIME BETWEEN ACTIVATING THE NEXT POOFER
int poofOn = 2000;          //THE TIME BETWEEN

void setup() {
  Serial.begin(9600);
  pinMode(poof1, OUTPUT);
  pinMode(poof2, OUTPUT);
  pinMode(poof3, OUTPUT);
  pinMode(poof4, OUTPUT);
  pinMode(poof5, OUTPUT);
}

void loop() {
  oldLeft = newLeft;
  newLeft = knobLeft.read();

  //THIS ACCOUNTS FOR DRIFT OVER TIME
  if (abs(newLeft - oldLeft) < deadZone) {
    delayTimer++;
    if (delayTimer > staleEnc) {
      Serial.println("Resetting Stale Timer");
      resetEnc();
      delayTimer = 0;
    }
  }

  //THIS IS THE CONDITION WHICH TRIGGERS THE FIREFX
  if (newLeft > travelPos && flameFlag == 0) {
    flameFlag = 1;
    Serial.print("Time to travelPos: ");
    Serial.print(timer);
    Serial.println();
    flameDet(timer);
    resetEnc();
  }

  //THIS DOES THE PHYSICAL COUNTING OF THE TICKS, AND ADDS TO "TIMER"
  if (newLeft != oldLeft && newLeft > deadZone && newLeft > oldLeft) {
    // Serial.print("Left = ");
    // Serial.print(newLeft);
    // Serial.println();
    timer++;
    delayMicroseconds(counterDelay);
  }

  // if a character is sent from the serial monitor,
  // reset everything
  if (Serial.available()) {
    Serial.read();
    Serial.println("Resetting to zero");
    resetEnc();
  }
}

void resetEnc() {
  knobLeft.write(0);
  newLeft = 0;
  oldLeft = 0;
  timer = 0;
}

void flameDet(int rotSpeed) {
  Serial.println(rotSpeed);
  int onFlag = 0;

  if (rotSpeed < thresh5 && onFlag == 0) {
    flameOn(5);
    onFlag = 1;
  }
  if (rotSpeed < thresh4 && onFlag == 0) {
    flameOn(4);
    onFlag = 1;
  }
  if (rotSpeed < thresh3 && onFlag == 0) {
    flameOn(3);
    onFlag = 1;
  }
  if (rotSpeed < thresh2 && onFlag == 0) {
    flameOn(2);
    onFlag = 1;
  }
  if (rotSpeed < thresh1 && onFlag == 0) {
    flameOn(1);
    onFlag = 1;
  } else {
    flameOn(0);
    onFlag = 1;
  }
}

void flameOn(int level) {
  switch (level) {
    case 1:
      Serial.println("Flame Level 1");
      digitalWrite(poof1, HIGH);
      delay(poofOn);
      flameOff();
      break;
    case 2:
      Serial.println("Flame Level 2");
      digitalWrite(poof1, HIGH);
      delay(poofLag);
      digitalWrite(poof2, HIGH);
      delay(poofOn);
      flameOff();
      break;
    case 3:
      Serial.println("Flame Level 3");
      digitalWrite(poof1, HIGH);
      delay(poofLag);
      digitalWrite(poof2, HIGH);
      delay(poofLag);
      digitalWrite(poof3, HIGH);
      delay(poofOn);
      flameOff();
      break;
    case 4:
      Serial.println("Flame Level 4");
      digitalWrite(poof1, HIGH);
      delay(poofLag);
      digitalWrite(poof2, HIGH);
      delay(poofLag);
      digitalWrite(poof3, HIGH);
      delay(poofLag);
      digitalWrite(poof4, HIGH);
      delay(poofOn);
      flameOff();
      break;
    case 5:
      Serial.println("Flame Level 5");
      digitalWrite(poof1, HIGH);
      delay(poofLag);
      digitalWrite(poof2, HIGH);
      delay(poofLag);
      digitalWrite(poof3, HIGH);
      delay(poofLag);
      digitalWrite(poof4, HIGH);
      delay(poofLag);
      digitalWrite(poof5, HIGH);
      delay(poofOn);
      flameOff();

      poofsOn();
      delay(poofLag);
      flameOff();
      poofsOn();
      delay(poofLag);
      flameOff();
      poofsOn();
      delay(poofLag);
      flameOff();
      poofsOn();
      delay(poofOn);
      flameOff();

      poofsOn();
      delay(poofLag);
      flameOff();
      poofsOn();
      delay(poofLag);
      flameOff();
      poofsOn();
      delay(poofLag);
      flameOff();
      poofsOn();
      delay(poofOn);
      flameOff();

      break;
    default:
      Serial.println("Flames off");
      break;
  }
  flameFlag = 0;
  delay(settlingBreak);
  resetEnc();
}

void flameOff() {
  digitalWrite(poof1, LOW);
  digitalWrite(poof2, LOW);
  digitalWrite(poof3, LOW);
  digitalWrite(poof4, LOW);
  digitalWrite(poof5, LOW);
}

void poofsOn() {
  digitalWrite(poof1, HIGH);
  digitalWrite(poof2, HIGH);
  digitalWrite(poof3, HIGH);
  digitalWrite(poof4, HIGH);
  digitalWrite(poof5, HIGH);
}

