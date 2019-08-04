//This is a program to control the motor according to the water level in the tank
const int water_level_full = 2;
const int water_level_medium=3;
const int water_level_intermediate=4;
const int water_level_low=5;
const int motor_on_relay=6;
const int motor_off_relay=7;

char tank_level="";
int motor_status=0;

void setup() {
  // put your setup code here, to run once:
pinMode(water_level_full, INPUT);
pinMode(water_level_medium, INPUT);
pinMode(water_level_intermediate, INPUT);
pinMode(water_level_low, INPUT);
pinMode(motor_on_relay, OUTPUT);
pinMode(motor_off_relay, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
water_level_finder();
}
void water_level_finder()
{
  if  (digitalRead(water_level_full)==HIGH){
      tank_level='full';
  }
  if  (digitalRead(water_level_medium)==HIGH){
      tank_level='medium';
  }
  if  (digitalRead(water_level_intermediate)==HIGH){
      return 2;
  }
  if  (digitalRead(water_level_low)==HIGH){
      return 1;
  }
}
void motor_start(){
  digitalWrite(motor_on_relay,LOW);
  delay(2000);
  digitalWrite(motor_on_relay,HIGH);
  Serial.println("motor starting sequence executed");
  motor_status=1;
  Serial.println("Motor start message notified to user");
}
