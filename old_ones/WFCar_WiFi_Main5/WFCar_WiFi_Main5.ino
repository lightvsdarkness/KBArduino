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
Servo servoXright_11;
//Nothing
//Servo servo7;
int grad;
int c;
int pos, pin, amountofgrad;
boolean side;
boolean perimeter = false;
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

//void servo_180(Servo servo, int ch)
//{
//  servo.write(0);
//  delay(500);
//  grad = 15;
//  for (c = 0; c < ch; c = c + 1)
//  {
//    servo.write(grad);
//    grad = grad + 20;
//    delay(500);
//  }
//  servo.write(0);
//}

//void servo_left()
//{
//
//  int servotemp=servoX.read();//获取舵机目前的角度值
//  servotemp-=2;//舵机运动1度
//  if(servotemp<170&&servotemp>10) //我定义的舵机旋转角度为10度到170度
//  servoX.write(servotemp);
//  else if (servotemp<=10)  servoX.write(10);
//  else servoX.write(170);
//}
//void servo_right()
//{
//  int servotemp=servoX.read();//获取舵机目前的角度值
//  servotemp+=2;//舵机运动1度
//  if(servotemp<170&&servotemp>10)//我定义的舵机旋转角度为10度到170度
//  servoX.write(servotemp);
//  else if (servotemp<=10)  servoX.write(10);
//  else servoX.write(170);
//}

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
//side 0 - left, side 1 - right
void manipulator_left (Servo myservo, boolean side,  int amountofgrad) {
  int servonow=myservo.read();
  if (side)
    for(pos = servonow; pos <= amountofgrad; pos += 1) // goes from 0 degrees to 180 degrees 
  {                                  // in steps of 1 degree 
    myservo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(15);
  } 

else 
  for(pos = servonow; pos>=amountofgrad; pos-=1)     // goes from 180 degrees to 0 degrees 
  {         
    myservo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
  } 
}
//side 1 - right, side 0 - left
void manipulator_right (Servo myservo, boolean side,  int amountofgrad) {
  int servonow=myservo.read();
  if (!side)
    for(pos = servonow; pos <= amountofgrad; pos += 1) // goes from 0 degrees to 180 degrees 
  {                                  // in steps of 1 degree 
    myservo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(15);
  } 
else 
  for(pos = servonow; pos>=amountofgrad; pos-=1)     // goes from 180 degrees to 0 degrees 
  {         
    myservo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
  } 
}
//void grab_(){
// manipulator_left(servoXleft_10, true, 120);
// manipulator_right(servoXright_11, true, 30);
//}
//void release_(){
//  manipulator_left(servoXleft_10, false, 40);
//  manipulator_right(servoXright_11, false, 130);
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
  servoY6.attach(6);
  servoXleft_10.attach(10);
  servoXright_11.attach(4);
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
  //Мигаем
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
  //servoXleft_10.write(90);
//  servoXright_11.write(100);
//  servoXleft_10.write(60);
  //servoXright_11.read();
//  servoXright_11.write(130);
//  servoXleft_10.write(30);
  //delay(50);
//  distance = Dist.getDistanceCentimeter();
//  if(distance<=4 & distance>1) {
//    back();
//    delay(90);
//    ostanovka();
//    distance=0;
//  }

  if(Serial.available()) {
    modemkey = Serial.read();
       }
    if (modemkey == 'g'){
 manipulator_left(servoXleft_10, true, 120);
 manipulator_right(servoXright_11, true, 30);
    modemkey=0;
    }
    if (modemkey == 'r'){
  manipulator_left(servoXleft_10, false, 40);
  manipulator_right(servoXright_11, false, 130);
    modemkey=0;
    }
   if (modemkey == 'x')
   {
     perimeter = true;
     modemkey=0;
   }

  //Perimeter routine
 if (perimeter) {
      distance = Dist.getDistanceCentimeter();
      if(distance<=4 & distance>1) {
        back();
        delay(90);
        ostanovka();
        distance=0;
      } 
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
        //ostanovka();
        advance();
        delay(300);
      }  else if (per_dist>=8) {
        turnR();
        delay(200);
        //ostanovka();
        advance();
        delay(300);
      } else {
        advance();
        delay(400);

      } 
 }

//  if(Serial.available()) {
//    modemkey = Serial.read();
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
       //servo_left();
       modemkey=0;
       break;
     case 'l': 
       //servo_right();
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
   
    }
  //}
 }
