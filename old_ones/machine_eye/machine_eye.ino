#include <AFMotor.h>
#include <Servo.h>
//Моторы
AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);
AF_DCMotor myMotors[4] = { motor1, motor2, motor3, motor4 };
const int maxspeed = 255;
//Ультразвук
const int Trig = 52;
const int Echo = 50;
//Серва
// Массив для хранения углов поворота сервопривода (шаг 15 градусов)
const int vservo_array[13] = { 0, 15, 30, 45, 60, 75, 90, 105, 120, 135, 150, 165, 180 };
Servo vservo;
int vservopos = 0;
//Переменная, показывающая в каком направлении больше свободного пространства, от -1 до 13
//(-1 если впереди и по бокам нет места)
int maxarray_index = 0;
//
int maxarray_distance = 0;
int alloweddsitance = 20;
int i;
///////////////////////////////////////
void accel(int i) {
  for (j = 250; j < 255; j++) {
    myMotors[i].setSpeed(j);
    delay(5);
  }
}
//Для объезда препятствий
int ultrasoundmode() {
  vservo.write(90);
  maxarray_index = 0;
  maxarray_distance = 0;

  // Крутим серву, измеряя расстояния и занося данные в массив
  for (vservopos = 0; vservopos < 13; vservopos = vservopos + 1) {
  vservo.write(vservo_array[vservopos]);
    vservoarray[vservopos] = HC_SR04();

    if (maxarray_distance > vservo_array[vservopos]) {
      maxarray_index = vservopos ;
      maxarray_distance = vservo_array[vservopos];
    }

    // Выводим данные для отладки
    Serial.print(vservopos);
    Serial.print(" ");
    Serial.println(vservo_array[vservopos]);
  }
  // Если расстояние меньше минимально допустимого, присваиваем -1, т.е. задний ход
  if (maxarray_distance < 20)
{
  maxarray_index = -1;
}
return maxarray_index;
}


/* Функция определения расстояния с дальномера */
int HC_SR04() {
  digitalWrite(Trig, HIGH); // Подаем сигнал на выход микроконтроллера
  delayMicroseconds(10); // Удерживаем 10 микросекунд
  digitalWrite(Trig, LOW); // Затем убираем
  time_us = pulseIn(Echo, HIGH); // Замеряем длину импульса
  distance_sm = time_us / 58; // Пересчитываем в сантиметры
  return vdistance_sm; // Возвращаем значение
  delay(20);
}

void setup() {
  //Serial
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("motor1 test!");

  //Мотор
  unsigned int time_us = 0;
  unsigned int distance_sm = 0;
  //turn on motor1
  //motor1.setSpeed(200);
  //motor1.run(RELEASE);

  //Ультразвуковой
  pinMode(Trig, OUTPUT);
  pinMode(Echo, INPUT);

  //Серва
  // Выбираем пин к которому подключен сервопривод
  vservo.attach(9); // или 10, если воткнули в крайний разъём
  // Поворачиваем сервопривод в положение 90 градусов при каждом включении
  vservo.write(90);
}


void loop() {
  maxarray_index = vultrasoundmode();
  switch (maxarray_index) {
    case '-1':
      obstacleBackward();
      break;
    case '1':
      runForward();
      break;



      /*	Serial.print("tock");
      	runBackward();*/

      Serial.print("tech");
      motor1.run(RELEASE);
  }
  delay(10);
}

void runForward() {
  uint8_t i;
  uint8_t j;
  for (i = 0; i < 4; i++) {
    myMotors[i].run(FORWARD);
    accel(i);
  }
  delay(10);
}

/*void fullForward() {
	uint8_t i;
	motor1.run(FORWARD);
	for (i = 250; i<255; i++) {
		motor1.setSpeed(i);
		delay(1);
	}

	for (i = 255; i != 250; i--) {
		motor1.setSpeed(i);
		delay(1);
	}
}

void runBackward() {
	uint8_t i;
	motor1.run(BACKWARD);
	for (i = 250; i<255; i++) {
		motor1.setSpeed(i);
		delay(1);
	}
}
*/
void obstacleBackward() {
  uint8_t i;
  uint8_t j;
  for (i = 0; i < 4; i++) {
    myMotors[i].run(BACKWARD);
    accel(i);
  }
  delay(10);
}


