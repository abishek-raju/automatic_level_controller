#include <NTPClient.h>
#include <ESP8266WiFi.h> // provides ESP8266 specific Wi-Fi routines we are calling to connect to network
#include <WiFiUdp.h> //handles sending and receiving of UDP packages

const int time_relay=2;
const char *ssid     = "JioFiber-2.4G";
const char *password = "12345678";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 19800,60000);
// You can specify the time server pool and the offset (in seconds, can be
// changed later with setTimeOffset() ). Additionaly you can specify the
// update interval (in milliseconds, can be changed using setUpdateInterval() ).
  

void setup(){
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }
  delay(3000);
  timeClient.begin();
  pinMode(time_relay, OUTPUT);
  digitalWrite(time_relay,HIGH);
  delay(5000);
}

void loop() {
  timeClient.update();
  
  
//  Serial.println(timeClient.getFormattedTime());/
  
  int hh = timeClient.getHours();
  int mm = timeClient.getMinutes();
  int ss = timeClient.getSeconds();
  if ((hh > 12) and (mm > 57) and (mm <59)){
    digitalWrite(time_relay,LOW);
  }
  else{
    digitalWrite(time_relay,HIGH);
  }
  Serial.print("hh : ");
  Serial.print(hh);
  Serial.print(" mm : ");
  Serial.print(mm);
  Serial.print(" ss : ");
  Serial.println(ss);
  
  delay(1000);
}
