#include <TaskScheduler.h>

#define MOTOR_FORWARD 1
#define MOTOR_BACK 2
#define MOTOR_STOP 0
#define MOTOR_B 0
#define MOTOR_A 1
int movement[][2] = { 
  { MOTOR_FORWARD, 3000},
  { MOTOR_STOP, 1000},
  { MOTOR_BACK, 6000},
  { MOTOR_FORWARD, 3000},
  { MOTOR_STOP, 1000},
};
int button = 25;
bool buttonState = false;

int motor_pins[][2] = { {13,12},
                        {14,27}};


void setupMotors() {
  for (int i =0; i < sizeof(motor_pins); i++) {
    for (int j =0; j < sizeof(motor_pins[i]); j++) {
      pinMode(motor_pins[i][j], OUTPUT);
      digitalWrite(motor_pins[i][j], LOW);
    }
  }
}
void setup() {
  setupMotors();    
    // put your setup code here, to run once:
    Serial.begin(115200);
    Serial.println("\nstarting\n");
}

void motor_move(int motor, int dir){
  
  digitalWrite(motor_pins[motor][0], LOW);
  digitalWrite(motor_pins[motor][1], LOW);
  switch (dir) {
    case MOTOR_BACK:
      digitalWrite(motor_pins[motor][0], HIGH);
      digitalWrite(motor_pins[motor][1], LOW);
      break;
    case MOTOR_FORWARD:
      digitalWrite(motor_pins[motor][0], LOW);
      digitalWrite(motor_pins[motor][1], HIGH);
      break;
    default:
      break;
  }
}

void play(int motor, int section[]){
  Serial.print(".");
  motor_move(motor, section[0]);
  delay(section[1]);
}

void loop() {
  if(buttonState){
    Serial.print("\nbutton pressed [");
    for (int i =0; i < 5; i++) {
      play(MOTOR_B, movement[i]);
    }
    Serial.print("] movement done");
    buttonState = false; 
  }
  buttonState = digitalRead(button);
  delay(50);
}
//


