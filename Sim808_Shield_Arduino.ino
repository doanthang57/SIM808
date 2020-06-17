#include <SoftwareSerial.h>
SoftwareSerial sim808(7,6);
String buff_data;
char phone_no[] = "0797582335"; // replace with your phone no.
String data[5];
#define DEBUG true
String state,timegps,latitude,longitude;
volatile int output = HIGH;   
void setup() {
sim808.begin(9600);
Serial.begin(9600);
delay(50);
attachInterrupt(digitalPinToInterrupt(2),buttonPressed1,RISING); 
attachInterrupt(digitalPinToInterrupt(3),buttonPressed2,RISING); 
pinMode(4,OUTPUT);       
digitalWrite(4,output);     //Turns LED ON or OFF depending upon output value
sim808.print("AT+CMGF=1\r");
delay(400);
sendData("AT+CGATT?",1000,DEBUG);
delay(50);
sendData("AT+CGNSPWR=1",1000,DEBUG);
delay(50);
sendData("AT+CGNSSEQ=RMC",1000,DEBUG);
delay(150);
}

void loop() 
{
  if(sim808.available()>0)
  {
    buff_data = sim808.readString();
    delay(100);
    int str_len = buff_data.length() + 1; 
    // Prepare the character array (the buffer) 
    char char_array[str_len];
    // Copy it over 
    buff_data.toCharArray(char_array, str_len);  
    if(char_array[2]=='R' && char_array[3]=='I'  )
      {
      Serial.println("GPS CHECKING");
      sim808.print("AT+CMGF=1\r");
      delay(400);
      sendData("AT+CGNSPWR=1",1000,DEBUG);
      delay(50);
      sendData("AT+CGNSSEQ=RMC",1000,DEBUG);
      delay(150);
      sendTabData("AT+CGNSINF",1000,DEBUG);
      if (state != 0) 
      {
      Serial.println("State  :"+state);
      Serial.println("Time  :"+timegps);
      Serial.println("Latitude  :"+latitude);
      Serial.println("Longitude  :"+longitude);
      
      sim808.print("AT+CMGS=\"");
      sim808.print(phone_no);
      sim808.println("\"");
      sim808.print(latitude);
      sim808.print(",");
      sim808.print (longitude);
      delay(300);
      sim808.print("http://maps.google.com/maps?q=loc:");
      sim808.print(latitude);
      sim808.print(",");
      sim808.print (longitude);
      delay(200);
      sim808.println((char)26); // End AT command with a ^Z, ASCII code 26
      delay(200);
      sim808.println();
      delay(10000);
      sim808.flush();
      } 
      else 
        {
        Serial.println("GPS Initializing…");
        }
      }
    }
  }

void sendTabData(String command , const int timeout , boolean debug){

sim808.println(command);
long int time = millis();
int i = 0;

while((time+timeout) > millis()){
while(sim808.available()){
char c = sim808.read();
if (c != ',') {
data[i] +=c;
delay(100);
} else {
i++;
}
if (i == 5) {
delay(100);
goto exitL;
}
}
}exitL:
if (debug) {
state = data[1];
timegps = data[2];
latitude = data[3];
longitude =data[4];
}
}
String sendData (String command , const int timeout ,boolean debug){
String response = "";
sim808.println(command);
long int time = millis();
int i = 0;

while ( (time+timeout ) > millis()){
while (sim808.available()){
char c = sim808.read();
response +=c;
}
}
if (debug) {
Serial.print(response);
}
return response;
}
void buttonPressed1()           //ISR function excutes when push button at pinD2 is pressed
{                    
   output = LOW;                //Change Output value to LOW
   digitalWrite(4,output);     //Turns LED ON or OFF depending upon output value
   Serial.println("ON");                                
}

void buttonPressed2()           //ISR function excutes when push button at pinD3 is pressed                             
{                    
   output = HIGH;               //Change Output value to HIGH 
   digitalWrite(4,output);     //Turns LED ON or OFF depending upon output value       
   Serial.println("OFF");                            
}
