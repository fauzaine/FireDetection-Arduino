//===== LCD ========
//LiquidCrystal lcd(18, 13, 22, 21, 20, 19);
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <String.h>
#define I2C_ADDR 0x3F     // Jika 0x3F tidak mau coba gunakan 0x27 atau 0x20
#define BACKLIGHT_PIN     3
#define En_pin  2
#define Rw_pin  1
#define Rs_pin  0
#define D4_pin  4
#define D5_pin  5
#define D6_pin  6
#define D7_pin  7
LiquidCrystal_I2C  lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);
SoftwareSerial SIM800L(2,3);

int relay = 12;
int pinApi = A0;
int pinGas = A1;
int buzzer = 8;
int sensorGasTres = 200;
int sensorApiTres = 400;
String textForSMS;
 
float sinVal;
int toneVal;

void setup()
{
  lcd.begin (16,2);
  // Nyalakan lampu backlight
  lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
  lcd.setBacklight(HIGH);

  pinMode (relay, OUTPUT);
  pinMode (pinApi, INPUT);
  pinMode (pinGas, INPUT);
  pinMode(8, OUTPUT);
  SIM800L.begin(9600);
  Serial.begin(9600);
}

 void SendTextMessage(String message)
 {
 SIM800L.write("AT+CMGF=1\r\n");
  delay(100);
  SIM800L.write("AT+CMGS=\"0812345678\"\r\n");
  delay(100);
  SIM800L.println(message);
  delay(100);
  SIM800L.write((char)26);
  delay(100);
  SIM800L.println();
 }
  

void loop()
{
 int sensorApi = analogRead(pinApi);
  Serial.print("Kadar Api: ");
  Serial.println (sensorApi);
  
 int sensorGas = analogRead(pinGas);       
  Serial.print("Kadar Gas: ");
  Serial.println(sensorGas); 
  
  sensorApi= analogRead (pinApi);
  if ((sensorApi < sensorApiTres) && (sensorGas > sensorGasTres ))
  {
   lcd.clear();
   lcd.setCursor(0, 0);
   lcd.print("STATUS :");
   lcd.setCursor(0, 1);
   lcd.print("Bahaya");
   Serial.println("Bahaya");
   digitalWrite(12, LOW);
   textForSMS = "Bahaya!!! Terdeteksi Api dan Asap";
   textForSMS = textForSMS;
   SendTextMessage(textForSMS);
    //Mengeluarkan Bunyi Alarm dari Buzzer
    for (int x=0; x<270; x++){
    sinVal = (sin(x*(3.1412/270)));
    toneVal = 1000+(int (sinVal*500));
    tone(8, toneVal);
    delay (2);
  }
  }
  
 if ((sensorApi < sensorApiTres) && (sensorGas < sensorGasTres))
  {
   lcd.clear();
   lcd.setCursor(0, 0);
   lcd.print("STATUS :");
   lcd.setCursor(0, 1);
   lcd.print("Terdeteksi Api");
   Serial.println("Terdeteksi Api");
  digitalWrite(12, LOW);
  textForSMS = "Terdeteksi Api";
  textForSMS = textForSMS;
  SendTextMessage(textForSMS);
    //Mengeluarkan Bunyi Alarm dari Buzzer
    for (int x=0; x<270; x++){
    sinVal = (sin(x*(3.1412/270)));
    toneVal = 1000+(int (sinVal*500));
    tone(8, toneVal);
    delay (2);
 }
 }
  
  if ((sensorApi > sensorApiTres) && (sensorGas > sensorGasTres))
  {
   lcd.clear();
   lcd.setCursor(0, 0);
   lcd.print("STATUS :");
   lcd.setCursor(0, 1);
   lcd.print("Terdeteksi Asap");
    Serial.println("Terdeteksi Asap");
   digitalWrite(12, HIGH);
   textForSMS = "Terdeteksi Asap";
   textForSMS = textForSMS;
   SendTextMessage(textForSMS);
    //Mengeluarkan Bunyi Alarm dari Buzzer
    for (int x=0; x<180; x++){
    sinVal = (sin(x*(3.1412/180)));
    toneVal = 2000+(int (sinVal*100));
    tone(8, toneVal);
    delay (2);
  }
  }
  
  if ((sensorApi > sensorApiTres) && (sensorGas < sensorGasTres))
  {
   lcd.clear();
   lcd.setCursor(0, 0);
   lcd.print("STATUS :");
   lcd.setCursor(0, 1);
   lcd.print("Aman");
    Serial.println("Aman");
     digitalWrite(12, HIGH);
    noTone(8); 
  }
  else
  {
    noTone(8); 
  }
}
