#include <SoftwareSerial.h>
#include <Wire.h>
#include <avr/wdt.h>
const int water_level_full = 4;
const int water_level_medium=3;
const int water_level_intermediate=2;
const int water_sensor=5;
const int motor_on_led_pin=6;
const int red_flag_led=7;
//int motor_on_relay = 8;
int motor_off_relay=9;
const int manual_Start_auto_Off_switch=10;
const int semi_auto_mode_light=11;
const int auto_mode_light=12;
const int voltage_input_pin=A0;
int tank_level_protocol=2;
boolean mode_0f_Operation_auto=true;
unsigned long startTimerTime=0; 
//unsigned long stopTimerTime=0;
const long max_motor_running_time=30*60000;//max time the motor should run regardless of what the situation is.
boolean motor_status=false;//the motor status at any given time.this is a digital variable meaning the physical running of the motor is not specified.
boolean red_flag =false;//any parameters out of range then this red_flag is raised and anywhere in the program this occurs the motor is turned down.
boolean voltage_flag=false;//0 means voltage is ok 1 means voltage is not ok
boolean initialize=true;
boolean time_flag=false;
const int time_input_pin=A5;
void setup()
{
  wdt_disable();//watch_dog timer is disabled here so that if enabled elsewhere it gets nullified
  //some of the setup is done here
  Serial.begin(9600);//for screen output
  delay(2000); 
//  pinMode(motor_on_relay,OUTPUT);
//  digitalWrite(motor_on_relay,HIGH);
//  pinMode(motor_off_relay,OUTPUT);
//  digitalWrite(motor_off_relay,HIGH);
//  pinMode(motor_on_led_pin,OUTPUT);
//  digitalWrite(motor_on_led_pin,LOW);
//  pinMode(red_flag_led, OUTPUT);
//  digitalWrite(red_flag_led,LOW);
  pinMode(semi_auto_mode_light, OUTPUT);
  digitalWrite(semi_auto_mode_light,LOW);
//  pinMode(auto_mode_light, OUTPUT);
//  digitalWrite(auto_mode_light,LOW);
//  delay(5000);
  Serial.println("program started and successfully connected to SERIAL monitor");
  delay(5000);//...........??????provide a certain delay before the motor starts so that the voltages settle
  pinMode(water_level_full, INPUT);
  pinMode(water_level_medium, INPUT);
  pinMode(water_level_intermediate, INPUT);
//  pinMode(water_sensor, INPUT);
//  pinMode(manual_Start_auto_Off_switch, INPUT);
  pinMode(voltage_input_pin,INPUT);
  if ((digitalRead(manual_Start_auto_Off_switch)==HIGH) and (initialize==1)){
    led_indicator(semi_auto_mode_light,true);
    Serial.println("running in semi auto mode");
    boolean mtr_on_off=manual_Start_auto_Off(motor_off_relay);
    if (mtr_on_off==true){
      led_indicator(motor_on_led_pin,true);
    }
    Serial.println(millis());
    startTimerTime=millis();
    mode_0f_Operation_auto=false;
    initialize=0;
    motor_status=true;
  }
  else if ((digitalRead(manual_Start_auto_Off_switch)==LOW) and (initialize==1)){
    led_indicator(auto_mode_light,true);
    Serial.println("running in auto mode");
    mode_0f_Operation_auto=true;
    initialize=0;
  }
  wdt_enable(WDTO_4S);
}

void loop()
{
  Serial.println("running loop");
  Serial.print("red_flagstatus = ");
  Serial.println(red_flag);
  tank_level_protocol=water_level_finder(water_level_intermediate,water_level_medium,water_level_full);
  Serial.print("tank_level_protocol = ");
  Serial.println(String(tank_level_protocol));
  voltage_flag=voltage_check_function(voltage_input_pin);
  Serial.print("voltage_flag = ");
  Serial.println(voltage_flag);
  time_flag=read_time_and_Set_flag(time_input_pin);
  Serial.print("time_flag = ");
  Serial.println(time_flag);
  if((motor_status==true) and ((red_flag==true) or (tank_level_protocol==2) or ((millis()-startTimerTime)>max_motor_running_time) or (voltage_flag==false)) ){
     motor_stop();
     motor_status=false;
     Serial.println("motor has stopped");
  }
  if((motor_status==false) and (red_flag==false) and (tank_level_protocol==0) and (mode_0f_Operation==true) and (voltage_flag==true) and (time_flag==true)){
    ///start the motor if everything is ok 
      motor_start();
      wdt_reset();
      startTimerTime=millis();
      motor_status=true;
  }
  Serial.print("motor_status = ");
  Serial.println(motor_status);
//  wdt_reset();
//  Serial.print("motor status:");
//  Serial.println(motor_status);
//  delay(2000);
//  if(motor_status==1 and (millis()-startTimerTime)>120000){
//      Serial.println("dry run check initiated###############################################################");
//      water_running();
//      Serial.println("dry run check completed");
//  } 
//  Serial.println(mode_0f_Operation);
//  semi_0r_auto_light();
////  if(motor_status==0 and (millis()-stopTimerTime)>60000){
////      Serial.println("dry run check initiated");
////      red_flag=0;
////      Serial.println("dry run check completed");
////  }

  delay(1000);
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
  delay(1500);
  digitalWrite(motor_on_relay,HIGH);
  wdt_reset();
}
//
//////define motor stopping sequence here
void motor_stop(){
  digitalWrite(motor_off_relay,HIGH);
  wdt_reset();
}

int water_level_finder(int wt_lvl_int,int wt_lvl_med,int wt_lvl_full){
  int tank_protocol = 2;
  int i=0;
  delay(150);
  wdt_reset();
  while (i<3){
   wdt_reset();
   if (digitalRead(wt_lvl_full)==HIGH){
      //protocol 1
      tank_protocol = 2;
    }
    else if ((digitalRead(wt_lvl_int)==HIGH) or (digitalRead(wt_lvl_med)==HIGH)){
      //protocol 2
      tank_protocol = 1;
    }
    else{
      //protocol 3
      tank_protocol = 0;
    }
    wdt_reset();
    i=i+1;
  }
//  Serial.print("tank_protocol=");
//  Serial.println(String(tank_protocol));
//  Serial.println("water level finder executed");
  return tank_protocol;
}
void led_indicator(int led_pin,boolean high_low){
  wdt_reset();
  if(high_low==true){
    digitalWrite(led_pin,HIGH);
  }
  else{
    digitalWrite(led_pin,LOW);
  } 
  wdt_reset();
}
//void water_running(){
//  if(digitalRead(water_sensor)==LOW){
//    Serial.println("red_flag_raised");
//    red_flag=1;
//    
//  }
//  Serial.println("water running sequence executed");
//  wdt_reset();
//}
//void motor_on_led(){
//  if(motor_status==1){
//    digitalWrite(motor_on_led_pin,HIGH);
//  }
//  if(motor_status==0){
//    digitalWrite(motor_on_led_pin,LOW);
//  } 
//}
//void red_flag_light(){
//  if (red_flag==1){
//    digitalWrite(red_flag_led,HIGH);
//  }
//  else{
//     digitalWrite(red_flag_led,LOW);
//  }
//}
boolean manual_Start_auto_Off(int mtr_off_relay){
  wdt_reset();
  int mtr_sts=1;
  digitalWrite(mtr_off_relay,LOW);
  wdt_reset();
  return true;
}
//void semi_0r_auto_light(){
//  if (mode_0f_Operation=="auto"){
//    
//    digitalWrite(auto_mode_light,HIGH);
//    digitalWrite(semi_auto_mode_light,LOW);
//    
//  }
//  if (mode_0f_Operation=="semi_auto"){
//    digitalWrite(semi_auto_mode_light,HIGH);
//    digitalWrite(auto_mode_light,LOW);
//  }
//}
boolean voltage_check_function(int vlt_inp_pin){
  wdt_reset();
  if (digitalRead(vlt_inp_pin)==HIGH){
    return true;
  }
  else if(digitalRead(vlt_inp_pin)==LOW){
    return false;
  }
}

boolean read_time_and_Set_flag(int time_inp_pin ){
  wdt_reset();
  if (digitalRead(time_inp_pin)==HIGH){
    return true;
  }
  else if(digitalRead(time_inp_pin)==LOW){
    return false;
  }
}
