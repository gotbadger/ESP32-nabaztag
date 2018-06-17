/// https://github.com/arkhipenko/TaskScheduler/blob/master/examples/Scheduler_example20_StatusRequest_LTS_WDT_Timeout_Object/Scheduler_example20_StatusRequest_LTS_WDT_Timeout_Object.ino#L4


#define _TASK_SLEEP_ON_IDLE_RUN // Enable 1 ms SLEEP_IDLE powerdowns between tasks if no callback methods were invoked during the pass 
#define _TASK_STATUS_REQUEST    // Compile with support for StatusRequest functionality - triggering tasks on status change events in addition to time only
#define _TASK_WDT_IDS           // Compile with support for wdt control points and task ids
#define _TASK_LTS_POINTER       // Compile with support for local task storage pointer
#define _TASK_PRIORITY          // Support for layered scheduling priority
// #define _TASK_MICRO_RES         // Support for microsecond resolution
#define _TASK_STD_FUNCTION      // Support for std::function (ESP8266 and ESP32 ONLY)
#define _TASK_DEBUG             // Make all methods and variables public for debug purposes
#define _TASK_INLINE       // Make all methods "inline" - needed to support some multi-tab, multi-file implementations
#define _TASK_TIMEOUT           // Support for overall task timeout 
#include <TaskScheduler.h>
Scheduler ts;
#define ARRAY_SIZE(x) sizeof(x)/sizeof(x[0])
#define MOTOR_FORWARD 1
#define MOTOR_BACK 2
#define MOTOR_STOP 0
#define MOTOR_B 0
#define MOTOR_A 1
int choreography_1[][2] = { 
  { MOTOR_FORWARD, 3000},
  { MOTOR_STOP, 1000},
  { MOTOR_BACK, 6000},
  { MOTOR_FORWARD, 3000},
};



//////////////////////////////////// button
int button = 25;
bool buttonState = false;
void check_button();
void debounce();

Task t_sample(100, TASK_FOREVER, &check_button, &ts, true);

void debounce() {
  Serial.println("debounced");
  t_sample.setCallback(&check_button);
  t_sample.delay(100);
}

void check_button(){
  Serial.print(".");
  buttonState = digitalRead(button);
  if(buttonState){
    Serial.println("button pressed");
    mov_schdule();
    t_sample.setCallback(&debounce);
    t_sample.delay(1000);
    Serial.println("continue");
  }
}

/////////////////////////




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

////// choreography

int i_mov = 0;
bool mov_end(){
  //finish choreography and stop movement
  motor_move(MOTOR_B, MOTOR_STOP);
  i_mov = 0;
  return true;
}

void mov_callbackack(){
  Task &t = ts.currentTask();
  int motor = t.getId();
  //set direction
  motor_move(motor, choreography_1[i_mov][0]);
  //set time till next choreography movement
  t.delay(choreography_1[i_mov][1]);
  i_mov++;
}
bool noop() {
  return true;
}

void mov_schdule(){
 if(i_mov != 0){
  Serial.println("already running"); 
  return;
 }
 Serial.print("running choreography_1:");
 Serial.println(ARRAY_SIZE(choreography_1));
 Task *t1 = new Task(TASK_MILLISECOND, ARRAY_SIZE(choreography_1), &mov_callbackack, &ts, false, &noop, &mov_end);
 Task *t2 = new Task(TASK_MILLISECOND, ARRAY_SIZE(choreography_1), &mov_callbackack, &ts, false, &noop, &mov_end);
 t1->setId(MOTOR_A);
 t2->setId(MOTOR_B);
 t1->restartDelayed();
 t2->restartDelayed();
}
/////




void loop() {
  ts.execute();
}
//


