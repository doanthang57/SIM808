#include <SoftwareSerial.h>
SoftwareSerial sim808(7,8); // Rx, Tx
#define DEBUG true
volatile int output = LOW; 
String buff_data;
char phone_number[] = "sdt:"; // replace with your phone no.

  // put your setup code here, to run once:
void setup() 
{
  sim808.begin(9600);
  Serial.begin(9600);
  delay(50);
  attachInterrupt(digitalPinToInterrupt(2),buttonPressed1,RISING); 
  attachInterrupt(digitalPinToInterrupt(3),buttonPressed2,RISING); 
  pinMode(13,OUTPUT); 
}

void loop() 
{
  // put your main code here, to run repeatedly:
 serialEvent();
}
void buttonPressed1()           //ISR function excutes when push button at pinD2 is pressed
{                    
   output = LOW;                //Change Output value to LOW
   digitalWrite(13,output);     //Turns LED ON or OFF depending upon output value
   Serial.println("ON");                                
}

void buttonPressed2()           //ISR function excutes when push button at pinD3 is pressed                             
{                    
   output = HIGH;               //Change Output value to HIGH 
   digitalWrite(13,output);     //Turns LED ON or OFF depending upon output value       
   Serial.println("OFF");                            
}
void serialEvent() {
  if(sim808.available()>0)
  {
    buff_data = sim808.readString();
    delay(100);
    int str_len = buff_data.length() + 1; 
    // Prepare the character array (the buffer) 
    char char_array[str_len];
    char water_array[str_len];
    // Copy it over 
    buff_data.toCharArray(char_array, str_len);
    Serial.print("char arary: ");
    Serial.println((char*)char_array);
  }
}
