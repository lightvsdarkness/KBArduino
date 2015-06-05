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
int grad, pos;
boolean camera = false;
boolean perimeter = false;
//ULTRASOUND
int percycle;
int timesadvanced;
const int degree_array[13] = { 85, 88, 91, 94, 97, 100, 103, 104, 107, 110, 111, 112, 113 };
//const int degree_array[13] = { 55, 60, 67, 72, 76, 84, 90, 97, 104, 112, 119, 127, 132 };
//const int degree_array[13] = { 65, 65, 65, 65, 70, 85, 90, 95, 100, 105, 110, 115, 125 };
//const int degree_array[13] = { 5, 15, 30, 45, 60, 75, 90, 105, 120, 135, 150, 160, 170 };
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
          analogWrite(EA,180);//0-255
          analogWrite(EB,180); //0-255
          digitalWrite(EN2,LOW);
          digitalWrite(EN3,HIGH);
          digitalWrite(EN4,HIGH);
          digitalWrite(EN5,LOW);     
        }        
void turnR(void)      		//turnR
        {        
          analogWrite(EA,180);//0-255
          analogWrite(EB,180); //0-255
          digitalWrite(EN2,HIGH);
          digitalWrite(EN3,LOW);
          digitalWrite(EN4,LOW);
          digitalWrite(EN5,HIGH);
          
        }
//WORKING
void servo_cam_turn50_to_80(Servo servo, int amountofgrad)
{
  int servonow = 50;
  for (grad = servonow; grad >= amountofgrad; grad+=1)
  {
    servo.write(servonow);
    delay(10);
  }
}
//void servo_cam_turn50_to_80(Servo servo, int amountofgrad)
//{
//  int servonow = 50;
//  for (grad = servonow; grad >= amountofgrad; grad+=1)
//  {
//    servo.write(servonow);
//    delay(10);
//  }
//}
//void servo_cam_turn80_to_110(Servo servo, int amountofgrad)
//{
//  int servonow = 80;
//  for (grad = servonow; grad <= amountofgrad; grad+=1)
//  {
//    servo.write(servonow);
//    delay(10);
//  }
//}
//For digital servo
//void servo_9000(Servo servo, int amountofgrad)
//{
//  int servonow=servo.read();
//  if (servonow < amountofgrad) {
//  for (grad = servonow; grad < amountofgrad; grad+=1)
//  {
//    servo.write(servonow);
//    delay(15);
//  }
//  }
//  else {
//      for (grad = servonow; grad > amountofgrad; grad-=1)
//      {
//        servo.write(servonow);
//        delay(15);
//      }
//  }
//}
// 130 left, 30 right
void grab_stuff (Servo servo_left, Servo servo_right) {
  int servonow_left=servo_left.read();
  int servonow_right=servo_right.read();
  if (servonow_left < 10) {
      for(pos = servonow_left; pos < 10; pos += 1)
      {
        servo_left.write(pos);
        delay(10);
      }
  }
  else if (servonow_left > 10) {
        for(pos = servonow_left; pos > 10; pos -= 1)
        {
          servo_left.write(pos);
          delay(10);
        }
  }
  if (servonow_right < 175) {
    for(pos = servonow_right; pos < 175; pos += 1)
    {
      servo_right.write(pos);
      delay(10);
    } 
  }
  else if (servonow_right > 175) {
      for(pos = servonow_left; pos > 175; pos -= 1)
      {
        servo_right.write(pos);
        delay(10);
      }
  }
}
void release_stuff (Servo servo_left, Servo servo_right){
  int servonow_left=servo_left.read();
  int servonow_right=servo_right.read();
  if (servonow_left < 170) {
      for(pos = servonow_left; pos < 170; pos += 1)
    {
      servo_left.write(pos);
      delay(10);
    }
  }
  else if (servonow_left > 170) {
        for(pos = servonow_left; pos > 170; pos -= 1)
        {
          servo_left.write(pos);
          delay(10);
        }
  }
  servo_right.writeMicroseconds(100);
  delay(600);
  // if (servonow_right < 10) {
  //   for(pos = servonow_right; pos < 10; pos += 1)
  // {
  //   servo_right.write(pos);
  //   delay(10);
  // } 
  // }
  // else if (servonow_right > 10) {
  //     for(pos = servonow_left; pos > 10; pos -= 1)
  //     {
  //       servo_right.write(pos);
  //       delay(10);
  //     }
  // }
}

void Perimeter () {
//  if(Serial.available()) {
//  modemkey = Serial.read();
//  if (modemkey = 'e')
//  {return;}
  int per_dist =  ultrasoundmode();
          if (timesadvanced> 4) {
            back();
            delay(200);
            ostanovka();
            turnL();
            delay(350);
            ostanovka();
            advance();
            delay(330);
            ostanovka();
          }
          if (per_dist < 0) {
            back();
            delay(200);
            ostanovka();
          } else if (per_dist<=3) {
            back();
            delay(70);
            ostanovka();
            turnL();
            delay(250);
            ostanovka();
            advance();
            delay(340);
            ostanovka();
          }  else if (per_dist>=9) {
            back();
            delay(70);
            ostanovka();
            turnR();
            delay(250);
            ostanovka();
            advance();
            delay(340);
            ostanovka();
          } else {
            timesadvanced += 1;
            advance();
            delay(400);
            ostanovka();
          }
  //}
}
//Для объезда препятствий
int ultrasoundmode() {
  int servoposition = 0;
  int maxarray_index = 0;
  int registeredmaxdistance = 0;
  //Переменная, показывающая в каком направлении больше свободного пространства, от -1 до 13
  //(-1 если впереди и по бокам нет места)
  //Пока не нужен
  //int received_distance_array[13];
  //
  int currentdistance = 0;
  int alloweddsitance = 20;
  //servoX.write(90);
  // Крутим серву, измеряя расстояния и занося данные в массив
  for (servoposition = 0; servoposition <= 12; servoposition += 1) {
	servoX.write(degree_array[servoposition]);
    delay(120);
    //received_distance_array[servoposition] = Dist.getDistanceCentimeter();
    currentdistance = Dist.getDistanceCentimeter();
    delay(100);
    if (registeredmaxdistance < currentdistance) {
      maxarray_index = servoposition;
      registeredmaxdistance = currentdistance;
    }
  }
  // Если расстояние меньше минимально допустимого, присваиваем -1, т.е. задний ход
  if (registeredmaxdistance < 12)
  {
    maxarray_index = -1;
  }
  servoX.write(95);
  return maxarray_index;
}

void setup() {
  //echo first, trig second
  Dist.begin(9,8);
  //SERVO SETUP
  //FORMER ATTACH AT SETUP
  //servoX.attach(11);
//  servoY6.attach(6);
//  servoXleft_10.attach(10);
//  servoXright_11.attach(4);
  //FORMER CENTER POSITIONING AT SETUP
  //servoX.write(95);
  //servoY6.write(90);
//  servoXleft_10.write(60);
//  servoXright_11.write(90);
  //Двигатели
  pinMode(EN2,OUTPUT);
  pinMode(EN3,OUTPUT);
  pinMode(EN4,OUTPUT);
  pinMode(EN5,OUTPUT);
  //Мигаем пару раз огоньками, если они есть
  //Объявляем переменные
  //pinMode(LED1,OUTPUT);
  //pinMode(LED2,OUTPUT);
   //for(kkl=0;kkl<2;vservopos + 1) {
    //digitalWrite(LED1,HIGH);
    //digitalWrite(LED2,HIGH);
    //delay(500);
    //digitalWrite(LED1,LOW);
    //digitalWrite(LED2,LOW);
    //delay(500);
  //}
Serial.begin(9600);  //Serial связан с модемом
  modemkey=0;
}


void loop() {
  //
//  distance = Dist.getDistanceCentimeter();
//  if(distance<=4 & distance>1) {
//    back();
//    delay(90);
//    ostanovka();
//    distance=0;
//  }
//  grab_stuff(servoXleft_10, servoXright_11, true);
//  delay(5000);
//  manipulator_left(servoXleft_10, false, 50);
//  manipulator_right(servoXright_11, false, 120);
//  delay(5000);
  //Perimeter routine

  if(Serial.available()) {
    timesadvanced = 0;
    modemkey = Serial.read();
    switch(modemkey) {
      case 'a':
         currentspeed = midspeed;
         advance();
         //servoX.write(90);
         modemkey=0;
         break;   
       case 'b':
         //servoX.write(150);
         back();
         modemkey=0;
         break;     
       case 'c':
         turnL();
         modemkey=0;
         break;
       case 'd':
        turnR();
        modemkey=0;
         break; 
       case 'e':
        ostanovka();
        modemkey=0;
         break;  
       case 'j': 
         currentspeed = maxspeed;
         advance();
         modemkey=0;
         break;
       case 'l': 
         //servo_right();
         modemkey=0;
         break;
       case 'k':
        servoY6.attach(6);
        delay(20);
        //servo_cam_turn50_to_80(servoY6, 50);
        servoY6.detach();
        modemkey=0;
        break;
       case 'i':  //MAYBE NOT WORKING?
         servoY6.attach(6);
         delay(20);
         //servo_cam_turn90_to_50(servoY6, 50);
         servoY6.writeMicroseconds(150);
         delay(200);
         servoY6.detach();
         modemkey=0;
         break;
       case 'm':
        servoY6.attach(6);
        delay(20);
        //servo_cam_turn110_to_80(servoY6, 30);
        servoY6.writeMicroseconds(1000);
        delay(200);
        servoY6.detach();
        modemkey=0;
        break;
       case 'n':
         servoXright_11.attach(4);
         release_stuff(servoXleft_10, servoXright_11);
         servoXleft_10.detach();
         servoXright_11.detach();
         modemkey=0;
         break;
       case 'g':
         servoXleft_10.attach(10);
         servoXright_11.attach(4);
         grab_stuff(servoXleft_10, servoXright_11);
         servoXright_11.detach();
         modemkey=0;
         break;
       case 'x':
         perimeter = true;
          if (perimeter) {
            servoX.attach(11);
            delay(15);
  //        distance = Dist.getDistanceCentimeter();
  //        if(distance<=4 & distance>1) {
  //          back();
  //          delay(90);
  //          ostanovka();
  //          distance=0;
  //        }
            timesadvanced = 0;
            for (percycle = 0; percycle <= 4; percycle += 1) {
              Perimeter();
            }
            perimeter = false;
            servoX.detach();
          }
       modemkey=0;
      break;
      }
  }
 }
