#include <Servo.h>
#include <DistanceSRF04.h>
DistanceSRF04 Dist;
int distance;
int EN2 = 14;  
int EN3 = 15;
int EN4 = 16;
int EN5 = 17;
//P2-1 и P2-2 плате - можно подключить 
//int LED1 = 6; 
//int LED2 = 7;
//
int EA = 3;
int EB = 5;
char modemkey;
//TJ-1 (10 выход), TJ-2 - для серв
//SERVO
//Ultrasound servo (110 - super left, 60 - super right)
Servo servoX;
//Camera servo
Servo servoY6;
//Left manipulator
Servo servoXleft_10;
//Right manipulator
Servo servoXright_11;
//
int grad, pos, pin, amountofgrad;
boolean camera = false;
boolean perimeter = false;
//ULTRASOUND
const int degree_array[13] = { 85, 88, 91, 94, 97, 100, 103, 104, 107, 110, 111, 112, 113 };
//const int degree_array[13] = { 55, 60, 67, 72, 76, 84, 90, 97, 104, 112, 119, 127, 132 };
//const int degree_array[13] = { 65, 65, 65, 65, 70, 85, 90, 95, 100, 105, 110, 115, 125 };
//const int degree_array[13] = { 0, 15, 30, 45, 60, 75, 90, 105, 120, 135, 150, 160, 170 };
//const int degree_array[13] = { 5, 30, 45, 60, 75, 90, 105, 120, 135, 150, 165, 180, 200 };
//SPEED
int minspeed = 160;
int midspeed = 190;
int maxspeed = 250;
int currentspeed = maxspeed;
///////////////////////////////////////

void ostanovka(void)      //stop
        {
          analogWrite(EA,0);
          analogWrite(EB,0); 
          digitalWrite(EN2,LOW);   
          digitalWrite(EN3,LOW);
          digitalWrite(EN4,LOW);   
          digitalWrite(EN5,LOW);       
        }
void advance(void)      //advance
        { 
          analogWrite(EA,currentspeed);//0-255
          analogWrite(EB,currentspeed);//0-255      
          digitalWrite(EN2,LOW);
          digitalWrite(EN3,HIGH);
          digitalWrite(EN4,LOW);
          digitalWrite(EN5,HIGH);
        }
void back(void)      //back
        {        
          analogWrite(EA,255);//0-255
          analogWrite(EB,255); //0-255 
          digitalWrite(EN2,HIGH);
          digitalWrite(EN3,LOW);
          digitalWrite(EN4,HIGH);
          digitalWrite(EN5,LOW);            
        }
void turnL(void)      //turnL
        {        
          analogWrite(EA,200);//0-255
          analogWrite(EB,200); //0-255
          digitalWrite(EN2,LOW);
          digitalWrite(EN3,HIGH);
          digitalWrite(EN4,HIGH);
          digitalWrite(EN5,LOW);     
        }        
void turnR(void)      		//turnR
        {        
          analogWrite(EA,200);//0-255
          analogWrite(EB,200); //0-255
          digitalWrite(EN2,HIGH);
          digitalWrite(EN3,LOW);
          digitalWrite(EN4,LOW);
          digitalWrite(EN5,HIGH);
        }
//Для объезда препятствий
int ultrasoundmode() {
  int servoposition = 0;
  int maxarray_index = 0;
  int registeredmaxdistance = 0;
  int currentdistance = 0;
  int alloweddsitance = 20;

  for (servoposition = 0; servoposition <= 12; servoposition += 1) {
    servoX.write(degree_array[servoposition]);
    delay(120);
    currentdistance = Dist.getDistanceCentimeter();
    delay(100);
    if (registeredmaxdistance > currentdistance) {
      maxarray_index = servoposition;
      registeredmaxdistance = currentdistance;
    }
  }
  // Если расстояние меньше минимально допустимого, присваиваем -1, т.е. задний ход
  if (registeredmaxdistance < 3)
  {
    maxarray_index = -1;
  }
  servoX.write(95);
  return maxarray_index;
}

void setup() {
  //echo first, trig second
  Dist.begin(9,8);
  //Servo setup
  servoX.attach(11);
  //Двигатели
  pinMode(EN2,OUTPUT);
  pinMode(EN3,OUTPUT);
  pinMode(EN4,OUTPUT);
  pinMode(EN5,OUTPUT);
}

void loop() {
      int per_dist =  ultrasoundmode();
      if (per_dist<0) {

      } else if (per_dist<=4) {
      //if (per_dist<=3) {
//        back();
//        delay(100);
//        ostanovka();
        turnL();
        delay(200);
        ostanovka();
        advance();
        delay(100);
        ostanovka();
      }  else if (per_dist>=10) {
//        back();
//        delay(100);
//        ostanovka();
        turnR();
        delay(200);
        ostanovka();
        advance();
        delay(100);
        ostanovka();
      } else {
        advance();
        delay(250);
        ostanovka();
      } 
 }
 switch () {
   case 'a':
   
   break;
   case 'b':
   break;
   case 'c':
   break;
   case 'd':
   break;
}

