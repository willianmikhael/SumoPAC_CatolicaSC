// Definições dos pinos dos motores
#define START_MOTOR_1 3
#define LEFT_MOTOR_PIN_1 5
#define LEFT_MOTOR_PIN_2 4

#define START_MOTOR_2 6
#define RIGHT_MOTOR_PIN_1 7
#define RIGHT_MOTOR_PIN_2 8

#define S1 A1
#define S2 A2
#define TRESHOLD 600

#define sensor A3
 
// <!---------------------------------------   Function Motor Control  -------------------------------------------!>

void motors(int left, int right) {  //0 until 255    0 until -255
  // <#!----- Motor Left  -----!#> //
  if (left >= 0) {
    digitalWrite(LEFT_MOTOR_PIN_1, HIGH);
    digitalWrite(LEFT_MOTOR_PIN_2, LOW);
  } else {
    digitalWrite(LEFT_MOTOR_PIN_1, LOW);
    digitalWrite(LEFT_MOTOR_PIN_2, HIGH);
    left = left * (-1);
  }
  analogWrite(START_MOTOR_1, left);

  //<#!----- Motor Right  -----!#> //
  if (right >= 0) {
    digitalWrite(RIGHT_MOTOR_PIN_1, HIGH);
    digitalWrite(RIGHT_MOTOR_PIN_2, LOW);
  } else {
    digitalWrite(RIGHT_MOTOR_PIN_1, LOW); 
    digitalWrite(RIGHT_MOTOR_PIN_2, HIGH);
    right = right * (-1);
  }
  analogWrite(START_MOTOR_2, right);
}

int sharp(){
  float volts= analogRead(sensor)*0.0048828125;
  int distance = 26*pow(volts, -1);

 return distance;

}
// <!------------------------------------------------------------------------------------------------------------!>

void setup() {



  Serial.begin(9600); // inicia a comunicação serial


  pinMode(sensor, INPUT);
  pinMode(S1, INPUT);
  pinMode(S2, INPUT);

  pinMode(START_MOTOR_1, OUTPUT);
  pinMode(LEFT_MOTOR_PIN_1, OUTPUT);
  pinMode(LEFT_MOTOR_PIN_2, OUTPUT);

  pinMode(START_MOTOR_2, OUTPUT);
  pinMode(RIGHT_MOTOR_PIN_1, OUTPUT);
  pinMode(RIGHT_MOTOR_PIN_2, OUTPUT);
}

void loop() {

  int sensor1 = analogRead(S1);
  int sensor2 = analogRead(S1);

  if(sensor1 <= TRESHOLD){
    for(int i=0;i < 30000;i++){
    motors(-80,-150);
    }
  } 
  else if(sensor2 <= TRESHOLD){
    for(int j=0;j < 30000;j++){
    motors(-150,-80);
    }
  } else if(sensor1 && sensor2 <= TRESHOLD){
    for(int j=0;j < 30000;j++){
    motors(-120,-120);
  }
  for(int j=0;j < 2000;j++){
    motors(-150,-80);
    }
  } else if(sharp() <= 50){
    motors(120,120);
  }
  else  motors(120,120);


  
}