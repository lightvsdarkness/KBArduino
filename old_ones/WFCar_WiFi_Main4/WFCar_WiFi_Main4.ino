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
int kkl;
char modemkey;

//TJ-1 (10 выход), TJ-2 - для серв
//Ultrasound servo
Servo servoX;
//Camera servo
Servo servoY6;
//Left manipulator
Servo servoXleft_10;
//Right manipulator
Servo servoXright_4;
//Nothing
//Servo servo7;
int grad;
int c;
//
int received_distance_array[13];
const int degree_array[13] = { 0, 15, 30, 45, 60, 75, 90, 105, 120, 135, 150, 160, 170 };
//const int degree_array[13] = { 5, 30, 45, 60, 75, 90, 105, 120, 135, 150, 165, 180, 200 };
//
int servoposition = 0;
//Переменная, показывающая в каком направлении больше свободного пространства, от -1 до 13
//(-1 если впереди и по бокам нет места)
int maxarray_index = 0;
//
int maxdistance;
int alloweddsitance = 20;
int i;
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
        
void advance(void)      //advance advance
        { 
          analogWrite(EA,254);//调节参数可调速度（0-255）
          analogWrite(EB,255);//调节参数可调速度（0-255）      
          digitalWrite(EN2,LOW);
          digitalWrite(EN3,HIGH);
          digitalWrite(EN4,LOW);
          digitalWrite(EN5,HIGH);
        }
void back(void)      //back
        {        
          analogWrite(EA,255);//调节参数可调速度（0-255）
          analogWrite(EB,255); //调节参数可调速度（0-255） 
          digitalWrite(EN2,HIGH);
          digitalWrite(EN3,LOW);
          digitalWrite(EN4,HIGH);
          digitalWrite(EN5,LOW);            
        }
void turnL(void)      //turnL
        {        
          analogWrite(EA,180);//调节参数可调速度（0-255）
          analogWrite(EB,180); //调节参数可调速度（0-255）
          digitalWrite(EN2,LOW);
          digitalWrite(EN3,HIGH);
          digitalWrite(EN4,HIGH);
          digitalWrite(EN5,LOW);     
        }        
void turnR(void)      		//turnR
        {        
          analogWrite(EA,180);//调节参数可调速度（0-255）
          analogWrite(EB,180); //调节参数可调速度（0-255）
          digitalWrite(EN2,HIGH);
          digitalWrite(EN3,LOW);
          digitalWrite(EN4,LOW);
          digitalWrite(EN5,HIGH);
          
        }  

void servo_180(Servo servo, int ch)
{
  servo.write(0);
  delay(500);
  grad = 15;
  for (c = 0; c < ch; c = c + 1)
  {
    servo.write(grad);
    grad = grad + 20;
    delay(500);
  }
  servo.write(0);
}

void servo_left()
{

  int servotemp=servoX.read();//获取舵机目前的角度值
  servotemp-=2;//舵机运动1度
  if(servotemp<170&&servotemp>10) //我定义的舵机旋转角度为10度到170度
  servoX.write(servotemp);
  else if (servotemp<=10)  servoX.write(10);
  else servoX.write(170);
}
void servo_right()
{
  int servotemp=servoX.read();//获取舵机目前的角度值
  servotemp+=2;//舵机运动1度
  if(servotemp<170&&servotemp>10)//我定义的舵机旋转角度为10度到170度
  servoX.write(servotemp);
  else if (servotemp<=10)  servoX.write(10);
  else servoX.write(170);
}

//void servo_up()
//{
//  int servotemp1=servoY.read();//获取舵机目前的角度值
//  servotemp1+=2;//舵机运动1度
//  if(servotemp1<170&&servotemp1>10)//我定义的舵机旋转角度为10度到170度
//  servoY6.write(servotemp1);
//  else if (servotemp1<=10)  servoY6.write(10);
//  else servoY6.write(170);
//}
//void servo_down()
//{
//  int servotemp1=servoY6.read();//获取舵机目前的角度值
//  servotemp1-=2;//舵机运动1度
//  if(servotemp1<170&&servotemp1>10)//我定义的舵机旋转角度为10度到170度
//  servoY6.write(servotemp1);
//  else if (servotemp1<=10)  servoY6.write(10);
//  else servoY6.write(170);
//}


//Для объезда препятствий
int ultrasoundmode() {
  //vservo.write(90);
  servoX.write(170);
  maxarray_index = 0;
  maxdistance = 0;

  // Крутим серву, измеряя расстояния и занося данные в массив
  for (servoposition = 0; servoposition <= 12; servoposition = servoposition + 1) {
	servoX.write(degree_array[servoposition]);
    delay(100);
    received_distance_array[servoposition] = Dist.getDistanceCentimeter();

    if (maxdistance > received_distance_array[servoposition]) {
      maxarray_index = servoposition;
      maxdistance = received_distance_array[servoposition];
    }
//    Serial.print(vservopos);
//    Serial.print(" ");
//    Serial.print("distance:"+received_distance_array[vservopos]);
//    Serial.print(" ");
//    Serial.print("degree:"+degree_array[vservopos]);
  }
  // Если расстояние меньше минимально допустимого, присваиваем -1, т.е. задний ход
  if (maxdistance < 3)
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
  servoX.attach(2);
  //servoY6.attach(11);
  servoXleft_10.attach(10);
  servoXright_4.attach(4);
  servoX.write(95);
  //servoY6.write(90);
  //servoXleft_10.attach(90);
  //servoXright_11.write(90);
  //
  pinMode(EN2,OUTPUT);
  pinMode(EN3,OUTPUT);
  pinMode(EN4,OUTPUT);
  pinMode(EN5,OUTPUT);
  //pinMode(LED1,OUTPUT);
  //pinMode(LED2,OUTPUT);
  //Мигаем пару раз огоньками, если они есть
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
  //if (perimeter){}
  distance = Dist.getDistanceCentimeter();
  if(distance<=5 & distance>1) {
    back();
    delay(100);
    ostanovka();
    distance=0;
  } 
  if(Serial.available()) {
    modemkey = Serial.read();
  switch(modemkey) {
    case 'a':
       advance();
       servoX.write(90);
       modemkey=0;
       break;   
     case 'b':
       servoX.write(150);
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
       servo_left();
       modemkey=0;
       break;
     case 'l': 
       servo_right();
       modemkey=0;
       break;
     case 'k': 
      //servo_up();
      modemkey=0;
      break;
     case 'i':
       //servo_down();
       modemkey=0;
       break;
    case 'n':
       //digitalWrite(LED1,HIGH);
       //digitalWrite(LED2,HIGH);
       modemkey=0;
       break;
    case 'm':
      //digitalWrite(LED1,LOW);
      //digitalWrite(LED2,LOW);
      modemkey=0;
      break;
    case 'x':  //Perimeter routine
      distance = Dist.getDistanceCentimeter();
//      if(distance<=5 & distance>1) {
//        back();
//        delay(100);
//        ostanovka();
//        distance=0;
//      } 
      int per_dist =  ultrasoundmode();
      if (per_dist<=0) {
        back();
        delay(200);
        ostanovka();
        advance();
        delay(200);
      } else if (per_dist<=5) {
        turnL();
        delay(200);
        ostanovka();
        advance();
        delay(300);
      }  else if (per_dist>=8) {
        turnR();
        delay(200);
        ostanovka();
        advance();
        delay(300);
      } else {
        advance();
        delay(400);
        ostanovka();
      }
      modemkey=0;
      break;   
    }
  }
 }
