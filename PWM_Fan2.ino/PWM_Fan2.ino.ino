#include "LowPower.h"

// Defined
#define FAN_OFF 0
#define FAN_LOW 1
#define FAN_MID 2
#define FAN_HIGH 3

// Hardware pins
const int sw1 = 2;
const int sw2 = 3;
const int pwmPin1 = 9;
const int pwmPin2 = 10;

// Functions Prototypes
void checkingInput();

void setup() {
  pinMode(sw1, INPUT_PULLUP);
  pinMode(sw2, INPUT_PULLUP);
  pinMode(pwmPin1, OUTPUT);
  pinMode(pwmPin2, OUTPUT);
  Serial.begin(9600);
}

void loop() { 
  checkingInput();
}

/* checkingInput
 *   checking input from buttons to increase and decrease 
 *   pwm duty cycles. Meanwhile, debouncing the button inputs.
 */
void checkingInput(){
  static int fan_state = 0;
  static int previousSW1 = HIGH;
  static int previousSW2 = HIGH;
  volatile int pwmValue = 0;

  const int interval = 50;
  static unsigned long previousMillis = 0;
  unsigned long currentMillis = millis();

  // checking input every 50 ms
  if (currentMillis - previousMillis >= interval) {

    // update previous time.
    previousMillis = currentMillis;

    // Checking the falling edge of input from switch 1 and switch 2
    //  If so, increment fan state.
    //  On the other hand, decrement fan state.
    if((digitalRead(sw1) == LOW) && (previousSW1 == HIGH)){
      if(fan_state >= FAN_HIGH){
        fan_state = FAN_HIGH;  
      }
      else{
        fan_state++;  
      }
    }
    else if((digitalRead(sw2) == LOW) && (previousSW2 == HIGH)){
      if(fan_state <= FAN_OFF){
        fan_state = FAN_OFF;
      }
      else{
        fan_state--;  
      }
    }

    Serial.println(fan_state);

    // Select pwm value for the fans.
    switch(fan_state){
          case FAN_OFF:  {pwmValue =   0; break;} 
          case FAN_LOW:  {pwmValue = 100; break;} 
          case FAN_MID:  {pwmValue = 175; break;} 
          case FAN_HIGH: {pwmValue = 255; break;}
          default:       {pwmValue =   0; break;}
    }

    // Assign fans' pwm
    analogWrite(pwmPin1, pwmValue);
    analogWrite(pwmPin2, pwmValue);

    // update switches state
    previousSW1 = digitalRead(sw1);
    previousSW2 = digitalRead(sw2);

  }
}

