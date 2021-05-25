/*

Author   : Yash Sharma
Roll No. : 1901CS73
Project  : Bi-directional Person Counter

*/

/******************** Defining Global Variables ********************/
// count stores the number of people inside a building
int count = 0;

int last_count = 0;
float distance, distance_rear, distance_front;
float time_taken;

// declare variables for the Arduino pins to which the sensor is connected
int echo1 = 9, trigger1 = 10; // for front ultrasonic sensor
int echo2 = 7, trigger2 = 6;  // for rear ultrasonic sensor
int led = 2;

/************************* Initial setup *************************/
void setup()
{
 Serial.begin(9600);

  // initializing the pin to which echo terminal is connected as input
  // and trig terminal is connected as output 
  pinMode(echo1, INPUT);
  pinMode(trigger1, OUTPUT); 
  pinMode(echo2, INPUT);
  pinMode(trigger2, OUTPUT);

  // initializing the pin for led as output
  pinMode(led, OUTPUT);
}

/************************* Helper function *************************/
//function to calculate the distance of an object placed in front of the ultrasonic sensor
void calculate_distance(int trigger, int echo)
{
  digitalWrite(trigger, LOW);
  delayMicroseconds(10);        // delay for 10 microseconds
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);        // delay for 10 microseconds
  digitalWrite(trigger, LOW);

  time_taken = pulseIn(echo, HIGH);
  distance = time_taken * 0.017;

  if(distance > 50)
    distance = 50;
}

/**************************** Main Loop ****************************/
void loop()
{
  // if count > 0, led is turned on otherwise off
  if(count > 0) {
    digitalWrite(led,HIGH);
  }
  else {
    digitalWrite(led, LOW);
  }
 
  calculate_distance(trigger1, echo1);
  distance_front = distance; //get distance of front sensor
 
  if(distance < 50) {
    for(int i=0; i<5; i++) {
      calculate_distance(trigger2, echo2);
      distance_rear = distance;
      if(distance < 50) {
        count++;
        break;  
      }
      delay(200);        // delay for 200 milliseconds
    }
  }

  calculate_distance(trigger2, echo2);
  distance_rear = distance; //get distance of rear sensor

  if(distance_rear < 50) {
    for(int i=0; i<5; i++) {
      calculate_distance(trigger1, echo1);
      distance_front = distance;
      if(distance < 50) {
        count--;
        break;
      }
      delay(200);        // delay for 200 milliseconds
    }
  }

  // code to get the distance of an object from both 
  // front and rear sensor
  /*Serial.print("Distance from front sensor = ");
  Serial.println(distance_front);
  Serial.print("Distance from rear sensor = ");
  Serial.println(distance_rear);*/

  // since count can't be negative, we set it to 0 if 
  // somehow it becomes negative
  if(count < 0)
    count = 0;
  
  delay(200);        // delay for 200 milliseconds

  // last_count stores the value of count in previous iteration
  // if count > last_count, it means a person has entered the room
  if(count > last_count)
  {
     Serial.println("Welcome");
     Serial.print("People in room = ");
     Serial.println(count);    
  }
  // if count < last_count, it means a person has left the room
  else if(count < last_count)
  {
    Serial.println("Thank you for visiting");
    Serial.print("People in room = ");
    Serial.println(count);
  }

  //last_count is set to count
  last_count = count;
       
}
