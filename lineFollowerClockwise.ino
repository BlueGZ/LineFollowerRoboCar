//Variables for Ultrasonic pins & left & right IR LED
#include <NewPing.h>
#define trigPin 13
#define echoPin 12
#define maxDistance 200
NewPing sonar(trigPin, echoPin, maxDistance);
float duration, distance;
int LIR,RIR;

//Declaring i/p & o/p pins
void setup() 
{
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  Serial.begin(9600);
}

//Basic Movement Controls Declaration Start
void forward()
{
  digitalWrite(5, HIGH);
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  digitalWrite(8, HIGH);
}

void right()
{
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  digitalWrite(8, HIGH);
}

void left()
{
  digitalWrite(5, HIGH);
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
}

void clockwise(){
  digitalWrite(5, HIGH);
  digitalWrite(6, LOW);
  digitalWrite(7, HIGH);
  digitalWrite(8, LOW);
}

void anticlockwise(){
  digitalWrite(5, LOW);
  digitalWrite(6, HIGH);
  digitalWrite(7, LOW);
  digitalWrite(8, HIGH);
}

void Stop()
{
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
}
//Basic Movement Controls Declaration End


void loop(){
  distance = sonar.ping_cm();
  Serial.print("Distance = ");
  if (distance>=400 || distance<=2){
    Serial.print("Out of Range");
  }
  else{
    Serial.print(distance);
    Serial.println(" cm");
  }
  delay(500);
  if (distance<30){
    Stop();
  }
  else{
    IR();
  }
}


//Code for Line Follower
int count = 0;
void IR(){
  
  LIR = digitalRead(A0);
  RIR = digitalRead(A1);
  
  if(LIR==1 && RIR==1)                //When buggy is on the black line
  forward();
  
  if(LIR==0 && RIR==1)                //When black line is under the left IR LED
  left();
 
  if(LIR==1 && RIR==0)                //When black line is under the right IR LED
  right();

  //Near Junction or black line(park area) when both IR LEDs are above black lines
  if(LIR==0 && RIR==0){
    count++;
    //First, Sixth & Seventh time the buggy moves forward ignoring the black line
    if (count==1 || count==6 || count==7){
      do{
        LIR = digitalRead(A0);
        RIR = digitalRead(A1);
        forward();
      }while(LIR==0 && RIR==0);
    }

    //The first junction, we move left to stay on the track (Loop Starts)
    if (count==2){
      do{
        LIR = digitalRead(A0);
        RIR = digitalRead(A1);
        right();
        delay(680);
      }while(LIR==0 && RIR==0);
    }
  
    //The second junction, we move right (to complete 1 loop)
    if (count==3){
      do{
        LIR = digitalRead(A0);
        RIR = digitalRead(A1);
        left();
      }while(LIR==0 && RIR==0);
    }

    //The third junction we move left (towards parking)
    if (count==5){
      do{
        LIR = digitalRead(A0);
        RIR = digitalRead(A1);
        right();
        delay(600);
      }while(LIR==0 && RIR==0);
    }

    //We will park where we started
    if (count==8){
      Stop();
      delay(300000);
    }
    
  }
}
