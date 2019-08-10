#include <avr/wdt.h>
const int water_level_full = 2;
const int water_level_medium=3;
const int water_level_intermediate=4;
const int water_level_low=5;
int motor_on_relay = 8;
int motor_off_relay=7;
String tank_level="NN";
#include <SoftwareSerial.h>
//
int motor_status=0;//the motor status at any given time.this is a digital variable meaning the physical running of the motor is not specified.
int red_flag =0;//any parameters out of range then this red_flag is raised and anywhere in the program this occurs the motor is turned down.
//int water_sensor=0;//not needed in this iteration
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
  delay(5000);
  Serial.println("program started and successfully connected to SERIAL monitor");
  delay(5000);//...........??????provide a certain delay before the motor starts so that the voltages settle
  pinMode(water_level_full, INPUT);
  pinMode(water_level_medium, INPUT);
  pinMode(water_level_intermediate, INPUT);
  pinMode(water_level_low, INPUT);
  wdt_enable(WDTO_4S);
}

void loop()
{
  Serial.println("running loop");
  water_level_finder();
  if (initialize==1){
  delay(3000);
  wdt_reset();
  initialize=0;
  }
  if((motor_status==1) and ((red_flag==1) or (tank_level=="full")) ){
     motor_stop();
     Serial.println("motor has stopped");
  }
  if((motor_status==0) and (red_flag==0) and (tank_level=="low")){
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

//  if(motor_status==1){
//      water_running();
//  } 
}
/////
void motor_start(){
  digitalWrite(motor_on_relay,LOW);
  wdt_reset();
  delay(2000);
  digitalWrite(motor_on_relay,HIGH);
  Serial.println("motor starting sequence executed");
  motor_status=1;
  wdt_reset();
}

////define motor stopping sequence here
void motor_stop(){
  digitalWrite(motor_off_relay,LOW);
  wdt_reset();
  delay(2000);
  digitalWrite(motor_off_relay,HIGH);
  Serial.println("motor stopping assequence executed");
  motor_status=0;
  wdt_reset();
}

void water_level_finder(){
  if  (digitalRead(water_level_full)==HIGH){
      tank_level="full";
  }
  else if  (digitalRead(water_level_medium)==HIGH){
      tank_level="medium";
  }
  else if  (digitalRead(water_level_intermediate)==HIGH){
      tank_level="intermediate";
  }
  else {
      tank_level="low";
  }
  Serial.print("tank_level=");
  Serial.println(String(tank_level));
  Serial.println("water level finder executed");
}
//void water_running(){
//  red_flag=0;
//  if(water_sensor==0){
//    red_flag==1;
//  }
//  Serial.println("water running sequence executed");
//  wdt_reset();
//}
