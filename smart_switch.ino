#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
int currentHour; 
int currentMinute;
int buttonSwitchs = 26;
int sensorState; //สถานะเซนเซอร์
int readLightSensor = 12;  
int lightState; //สถานะแสง ค่าแสง 1 คือมืด 0 คือสว่าง
int switchState; // สถานะสวิตช์
int switchRead; 
int count; // นับป้องกัน เพื่อไม่ให้ปิดเปิดไปมาเอง
int vcc1 = 15;
int gnd1 = 2;
int relay = 23;
int green = 5; //ไฟสีเขียว
int blue = 17; //ไฟสีน้ำเงิน
int red = 16; //ไฟสีเขียว
int vcc2 = 4;
int vcc3 = 14;
int gnd3 = 27;
const char* ssid     = "YourSSID"; 
const char* password = "YourPassword"; 
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 7 * 3600, 60000);  // UTC+7 เป็นเวลาไทย

void setup() {
  WiFi.begin(ssid, password);
  Serial.begin(9600);
  pinMode(readLightSensor, INPUT);  // สถานะค่าแสงเป็น input
  pinMode(buttonSwitchs, INPUT);  // สถานะสวิตช์เป็น input
  pinMode(relay, OUTPUT);
  pinMode(vcc1, OUTPUT);
  pinMode(vcc2, OUTPUT);
  pinMode(gnd1, OUTPUT);
  pinMode(vcc3, OUTPUT);
  pinMode(gnd3, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(blue, OUTPUT);
  digitalWrite(vcc1,HIGH);
  digitalWrite(vcc2,HIGH);
  digitalWrite(gnd1,LOW);
  digitalWrite(vcc3,HIGH);
  digitalWrite(gnd3,LOW);
  digitalWrite(green,HIGH);
  digitalWrite(blue,HIGH);
  digitalWrite(red,HIGH);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
  digitalWrite(green,LOW);
  digitalWrite(red,LOW);
  Serial.print(".");
  delay(500);
  digitalWrite(green,HIGH);
  digitalWrite(red,HIGH);
  delay(500);
  }
  Serial.println("\nConnected to WiFi");
  timeClient.begin();
}

void loop() {
  timeClient.update(); //อัพเดตเวลา
  currentHour = timeClient.getHours();
  currentMinute = timeClient.getMinutes();
  Serial.print("Time: ");
  Serial.print(currentHour);
  Serial.print(":");
  Serial.println(currentMinute);
  lightState = digitalRead(readLightSensor);
  switchRead = digitalRead(buttonSwitchs);
  Serial.print("Button State: ");
  Serial.println(lightState); 
  // ถ้ามีการกดปุ่ม
 if(switchRead == 1){
   switchState ++;
   delay(300);
   if(switchState > 1) switchState = 0;
 }
  // ถ้าเวลาเป็นเที่ยงคืน หรือ มีแสง ให้ดับไฟ
  if((currentHour == 0 && currentMinute == 0) || lightState == 0) switchState = 0;
  // ถึงเวลาตั้งแต่ 17:00 น.เป็นต้นไป และไม่มีแสง ให้ติดไฟ ยาวถึงเที่ยงคืน
 if(currentHour >= 17 && lightState == 1){
  
  if(count == 0){
   switchState += 1; 
   delay(300);
   if(switchState > 1) switchState = 0;
  }
  count = 1; // ให้ count = 1 เพื่อไม่ให้ปิดเปิดไปมาเอง
  if(switchState == 1) switchOn();
  else switchOff();
 } 
 // นอกจากนั้น
 else {
  count = 0;
  if(switchState == 1) switchOn();
  else switchOff();
 }
  
}
// สถานะไฟเปิด
void switchOn(){
  digitalWrite(relay,LOW);
  digitalWrite(green,LOW);
  digitalWrite(red,HIGH);
}
// สถานะไฟปิด
void switchOff(){
  digitalWrite(relay,HIGH);
  digitalWrite(green,HIGH);
  digitalWrite(red,LOW);
}
