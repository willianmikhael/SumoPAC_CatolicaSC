// Portas driver motor
#define PININ1 2      //define rotação horário ou anti-horário (para motor1) ex.: HIGH/LOW
#define PININ2 4      //define rotação horário ou anti-horário (para motor1) ex.: LOW/HIGH

#define PININ3 6      //define rotação horário ou anti-horário (para motor2)   ex.: HIGH/LOW
#define PININ4 7      //define rotação horário ou anti-horário (para motor2)   ex.: LOW/HIGH

#define PINENA 3      //Liga ou desliga (motor1)
#define PINENB 5      //Liga ou desliga (motor2)

#define S1 A0         // Portas sensor QTR
#define S2 A1         // Portas sensor QTR

#define sharp A2      //Sensor Sharp (10 a 80 cm)

#define SPEED 100     //Define velocidade
#define FORCE 155     //Define boost para velocidade

#define TRESHOLD 700  // Valor de referencia para cor da linha branca


// <!--------------------------------------------------- inicio Settings motores ---------------------------------------------------!>
class MotorPins {
private:
  int en0, en1, en2;

public:
  MotorPins(int pin0, int pin1, int pin2) : en0(pin0), en1(pin1), en2(pin2) {
    pinMode(en0, OUTPUT);
    pinMode(en1, OUTPUT);
    pinMode(en2, OUTPUT);
  }
     int getEn0() const { return en0; }
     int getEn1() const { return en1; }
     int getEn2() const { return en2; }
};
// <!--------------------------------------------------- fim Settings dos motores ---------------------------------------------------!>

// <!------------------------------------------------- inicio controle dos motores --------------------------------------------------!>
class MotorControl {
private:
  MotorPins pins;  

public:
  MotorControl(MotorPins pins) : pins(pins) {}

  void moveForward(int vel) {
    analogWrite(pins.getEn0(), vel);
    digitalWrite(pins.getEn1(), HIGH);
    digitalWrite(pins.getEn2(), LOW);
  }

  void moveBackward(int vel) {
    analogWrite(pins.getEn0(), vel);
    digitalWrite(pins.getEn1(), LOW);
    digitalWrite(pins.getEn2(), HIGH);
  }

  void stop() {
    analogWrite(pins.getEn0(), 0);
    digitalWrite(pins.getEn1(), LOW);
    digitalWrite(pins.getEn2(), LOW);
  }
};

// <!--------------------------------------------------- fim controle dos motores ----------------------------------------------------!>

// <!---------------------------------------------------- inicio leitura do Sharp ----------------------------------------------------!>
class Sharp {

public:
  Sharp(int sharp_port) {
    Serial.begin(9600);                              //Comunicação serial em 9600 baud rate
    pinMode(sharp_port, INPUT);
  }

  bool sharpRead(int *dist_read) {
    float volts = analogRead(sharp) * 0.0048828125;  //Calcula tensão na entrada AN
    int distance = 26 * pow(volts, -1);              //Equação de acordo com gráfico do datasheet

    if (distance <= *dist_read) {
      return true;
    } else {
      return false;
    }

    delay(700);
  }
};
// <!---------------------------------------------------- fim leitura do Sharp ----------------------------------------------------!>

// <!----------------------------------------- inicio leitura dos sensores de refletância -----------------------------------------!>
class Sensors {
  
  public:
  void readSensors(int *sensors) {
    sensors[0] = analogRead(S1);
    sensors[1] = analogRead(S2);
  }
};
// <!------------------------------------------ fim leitura dos sensores de refletância -------------------------------------------!>

// <!---------------------------------------------------- inicio controle ---------------------------------------------------------!>
class Control {

  public:
  void control(int* sensors,int* dist_read, Sharp& sharpsen, MotorControl& motor1Control, MotorControl& motor2Control) {
    if (sensors[0] <= TRESHOLD) {                       // verifica se o valor do sensor está abaixo do limiar
      motor1Control.moveBackward(100);                  // ativa motor 1
      motor2Control.moveBackward(100);                  // ativa motor 1
    } 
    else if (sensors[1] <= TRESHOLD){
      motor2Control.moveBackward(100);                  // ativa motor 1
      motor1Control.moveBackward(100);                  // ativa motor 2
    }
    else if (sharpsen.sharpRead(*dist_read) == true) {
      motor1Control.moveForward(150);                   // ativa motor 1
      motor1Control.moveForward(150);                   // ativa motor 2
    } 
    else {                                            
      motor1Control.stop();                             // desliga os motores caso não receba input dos sensores
      motor2Control.stop();                             // desliga os motores caso não receba input dos sensores
    }
  }
};
// <!------------------------------------------------------- fim controle --------------------------------------------------------!>

// <!--------------------------------------------------- inicio criando objetos --------------------------------------------------!>
MotorPins motor1Pins(PINENA, PININ1, PININ2);
MotorPins motor2Pins(PINENB, PININ3, PININ4);

MotorControl motor1Control(motor1Pins);
MotorControl motor2Control(motor2Pins);

Sharp sharpsen(sharp);

Sensors ir;

Control control;
// <!--------------------------------------------------- fim criando objetos ---------------------------------------------------!>

void setup() {
}

void loop() {
  int sensors[2];
  int dist_read = 80;
  ir.readSensors(sensors);                                                              // leitura dos sensores ir
  control.control(sensors, dist_read, sharpsen, motor1Control, motor2Control);          // controle dos motores  
}
