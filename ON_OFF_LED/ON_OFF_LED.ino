#include <SoftwareSerial.h>
SoftwareSerial gsm(2,3);
String buff_data;
const int LED = 13;

void setup() {
  gsm.begin(9600);
  Serial.begin(9600);
  Serial.println("PUMP CONTROL");

  delay(1000);

  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
}

void loop() {
  showSMS();
  if ( buff_data == "ON")
  {
    digitalWrite(LED, HIGH);
      Serial.println("OFF");
    delay(1000);
  }
  if (buff_data == "OFF")
  {
    digitalWrite(LED, LOW);
      Serial.println("ON");
    delay(1000);
  }
}

void showSMS()
{
 buff_data = gsm.readString();
    delay(100);
    int str_len = buff_data.length() + 1; 
    // Prepare the character array (the buffer) 
    char char_array[str_len];
    Serial.print("buff_data: ");
    Serial.println(buff_data);
    // Copy it over 
    buff_data.toCharArray(char_array, str_len);  
}
