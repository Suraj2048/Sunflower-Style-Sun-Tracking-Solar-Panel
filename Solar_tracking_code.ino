#include <Servo.h>

Servo horizontal; // Horizontal servo
int servoh = 180;
int servohLimitHigh = 175;
int servohLimitLow = 5;

Servo vertical; // Vertical servo
int servov = 0;
int servovLimitHigh = 60;
int servovLimitLow = 0;

// LDR pin connections
int ldrlt = A1; // LDR top left - BOTTOM LEFT
int ldrrt = A2; // LDR top right - BOTTOM RIGHT
int ldrld = A3; // LDR down left - TOP LEFT
int ldrrd = A4; // LDR down right - TOP RIGHT
int ldrmt = A5; // Middle LDR

const int button1 = 9;
const int button2 = 10;
const int motorA = 7;
const int motorB = 8;
int buttonStateA = 0;
int buttonStateB = 0;

int pos = 0;
int pos2 = 0;
int oldvalue;
int oldvalue2;

void setup() {
    horizontal.attach(5);
    vertical.attach(6);
    horizontal.write(180);
    vertical.write(0);

    pinMode(motorA, OUTPUT);
    pinMode(motorB, OUTPUT);
    pinMode(button1, INPUT);
    pinMode(button2, INPUT);  // Fixed duplicate pinMode issue

    delay(2500);
}

void loop() {
    int ldrStatus = analogRead(ldrmt);

    if (ldrStatus > 30) {
        buttonStateA = digitalRead(button1);
        if (buttonStateA == LOW) {
            digitalWrite(motorA, HIGH); // COUNTER clockwise
            digitalWrite(motorB, LOW);
        } else {
            digitalWrite(motorA, LOW);
            digitalWrite(motorB, LOW);
        }

        int lt = analogRead(ldrlt); // Top left
        int rt = analogRead(ldrrt); // Top right
        int ld = analogRead(ldrld); // Down left
        int rd = analogRead(ldrrd); // Down right
        int dtime = 10, tol = 90;   // Difference time, tolerance

        int avt = (lt + rt) / 2; // Average value top
        int avd = (ld + rd) / 2; // Average value down
        int avl = (lt + ld) / 2; // Average value left
        int avr = (rt + rd) / 2; // Average value right

        int dvert = avt - avd;   // Difference of up and down
        int dhoriz = avl - avr;  // Difference of left and right

        if (abs(dvert) > tol) { // If difference is out of tolerance
            if (avt > avd) {
                servov++;
                if (servov > servovLimitHigh) servov = servovLimitHigh;
            } else if (avt < avd) {
                servov--;
                if (servov < servovLimitLow) servov = servovLimitLow;
            }
            vertical.write(servov);
        }

        if (abs(dhoriz) > tol) { // If difference is out of tolerance
            if (avl > avr) {
                servoh--;
                if (servoh < servohLimitLow) servoh = servohLimitLow;
            } else if (avl < avr) {
                servoh++;
                if (servoh > servohLimitHigh) servoh = servohLimitHigh;
            }
            horizontal.write(servoh);
        }
        
        delay(dtime);
    } else { // If light is not detected
        oldvalue = horizontal.read();
        oldvalue2 = vertical.read();

        for (pos = oldvalue; pos <= 180; pos++) { 
            horizontal.write(pos);
            delay(15);
        }

        for (pos2 = oldvalue2; pos2 >= 0; pos2--) { 
            vertical.write(pos2);
            delay(15);
        }

        buttonStateB = digitalRead(button2);
        if (buttonStateB == LOW) {
            digitalWrite(motorA, LOW);  // Clockwise rotation
            digitalWrite(motorB, HIGH);
        } else {
            digitalWrite(motorA, LOW);
            digitalWrite(motorB, LOW);
        }
    }
}
