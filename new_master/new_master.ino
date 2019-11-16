#include <avr/wdt.h>
const int water_level_full = 4;
const int water_level_medium=3;
const int water_level_intermediate=2;
const int water_sensor=5;
const int motor_on_led_pin=6;
const int red_flag_led=7;
int motor_on_relay = 8;
int motor_off_relay=9;
const int manual_Start_auto_Off_switch=10;
const int semi_auto_mode_light=11;
const int auto_mode_light=12;
const int water_running_light=A0;
String tank_level="NN";
int f=0;
int m=0;
String mode_0f_Operation="auto";
unsigned long startTimerTime=0; 
unsigned long stopTimerTime=0;
const int  max_motor_running_time=1500000;//max time the motor should run regardless of what the situation is.
#include <SoftwareSerial.h>
//
int motor_status=0;//the motor status at any given time.this is a digital variable meaning the physical running of the motor is not specified.
int red_flag =0;//any parameters out of range then this red_flag is raised and anywhere in the program this occurs the motor is turned down.

int initialize=1;

void setup()
{
  wdt_disable();//watch_dog timer is disabled here so that if enabled elsewhere it gets nullified
  //some of the setup is done here
  Serial.begin(9600);//for screen output
  delay(2000); 
  pinMode(motor_on_relay,OUTPUT);
  digitalWrite(motor_on_relay,HIGH);
  pinMode(motor_off_relay,OUTPUT);
  digitalWrite(motor_off_relay,HIGH);
  pinMode(motor_on_led_pin,OUTPUT);
  digitalWrite(motor_on_led_pin,LOW);
  pinMode(red_flag_led, OUTPUT);
  digitalWrite(red_flag_led,LOW);
  pinMode(semi_auto_mode_light, OUTPUT);
  digitalWrite(semi_auto_mode_light,LOW);
  pinMode(auto_mode_light, OUTPUT);
  digitalWrite(auto_mode_light,LOW);
  pinMode(water_running_light, OUTPUT);
  digitalWrite(water_running_light,LOW);
  delay(5000);
  Serial.println("program started and successfully connected to SERIAL monitor");
  delay(5000);//...........??????provide a certain delay before the motor starts so that the voltages settle
  pinMode(water_level_full, INPUT);
  pinMode(water_level_medium, INPUT);
  pinMode(water_level_intermediate, INPUT);
  pinMode(water_sensor, INPUT);
  pinMode(manual_Start_auto_Off_switch, INPUT);
  
  wdt_enable(WDTO_4S);
}

void loop()
{
  Serial.println("red_flagstatus");
  Serial.println(red_flag);
  Serial.println("running loop");
  water_level_finder();
  motor_on_led();
  Serial.print("timer");
  Serial.println(startTimerTime);
  Serial.println(millis());
  if ((digitalRead(manual_Start_auto_Off_switch)==HIGH) and (initialize==1)){
    Serial.println("running in semi auto mode");
    manual_Start_auto_Off();
    mode_0f_Operation="semi_auto";
  }
  if (initialize==1){
  delay(3000);
  wdt_reset();
  initialize=0;
  }
  
  
  if(red_flag==1){
    digitalWrite(red_flag_led,HIGH);
  }
  if((motor_status==1) and ((red_flag==1) or (tank_level=="full")) ){
     motor_stop();
     Serial.println("motor has stopped");
  }
  if((motor_status==0) and (red_flag==0) and (tank_level=="low") and (mode_0f_Operation=="auto")){
    ///start the motor if everything is ok 
      motor_start();
      delay(3000);
      wdt_reset();
      delay(2000);
      wdt_reset();
  }
  wdt_reset();
  Serial.print("motor status:");
  Serial.println(motor_status);
  delay(2000);
  if(motor_status==1 and (millis()-startTimerTime)>120000){
      Serial.println("dry run check initiated###############################################################");
      water_running();
      Serial.println("dry run check completed");
  } 
  Serial.println(mode_0f_Operation);
  if(digitalRead(water_sensor)==HIGH){
    digitalWrite(water_running_light,HIGH);
  }
  semi_0r_auto_light();
//  if(motor_status==0 and (millis()-stopTimerTime)>60000){
//      Serial.println("dry run check initiated");
//      red_flag=0;
//      Serial.println("dry run check completed");
//  }
}
/////
void motor_start(){
  digitalWrite(motor_off_relay,LOW);
  wdt_reset();
  delay(2000);
  wdt_reset();
  delay(2000);
  wdt_reset();
  digitalWrite(motor_on_relay,LOW);
  wdt_reset();
  delay(2000);
  digitalWrite(motor_on_relay,HIGH);
  Serial.println("motor starting sequence executed");
  motor_status=1;
  Serial.println(millis());
  startTimerTime=millis();
  wdt_reset();
}

////define motor stopping sequence here
void motor_stop(){
  digitalWrite(motor_off_relay,HIGH);
  wdt_reset();
  delay(2000);
  Serial.println("motor stopping assequence executed");
  motor_status=0;
  stopTimerTime=millis();
  wdt_reset();
}

void water_level_finder(){
  int i=0;
  delay(150);
  wdt_reset();
  while (i<3){
   wdt_reset();
   if (digitalRead(water_level_full)==HIGH){
      tank_level="full";
    }
    else if (digitalRead(water_level_medium)==HIGH){
      tank_level="medium";
    }
    else if (digitalRead(water_level_intermediate)==HIGH){
      tank_level="intermediate";
    }
    else{
      tank_level="low";
    }
    wdt_reset();
    i=i+1;
  } 
  Serial.println("value of water level full");
  Serial.println(digitalRead(water_level_full));
  Serial.println("value of water level medium");
  Serial.println(digitalRead(water_level_medium));
  Serial.println("value of water level intermediate");
  Serial.println(digitalRead(water_level_intermediate));
  Serial.print("tank_level=");
  Serial.println(String(tank_level));
  Serial.println("water level finder executed");
}
void water_running(){
  if(digitalRead(water_sensor)==LOW){
    Serial.println("red_flag_raised");
    red_flag=1;
    
  }
  Serial.println("water running sequence executed");
  wdt_reset();
}
void motor_on_led(){
  if(motor_status==1){
    digitalWrite(motor_on_led_pin,HIGH);
  }
  if(motor_status==0){
    digitalWrite(motor_on_led_pin,LOW);
  } 
}
void red_flag_light(){
  if (red_flag==1){
    digitalWrite(red_flag_led,HIGH);
  }
  else{
     digitalWrite(red_flag_led,LOW);
  }
}
void manual_Start_auto_Off(){
  motor_status=1;
  Serial.println("program running in manual start auto off mode");
  digitalWrite(motor_off_relay,LOW);
  Serial.println(millis());
  startTimerTime=millis();
  wdt_reset();
}
void semi_0r_auto_light(){
  if (mode_0f_Operation=="auto"){
    
    digitalWrite(auto_mode_light,HIGH);
    
  }
  if (mode_0f_Operation=="semi_auto"){
    digitalWrite(semi_auto_mode_light,HIGH);
  }
}
