#include <Wire.h>
#include <iarduino_RTC.h>
#include <EEPROM.h>

iarduino_RTC time(RTC_DS3231);


#define pin_SW_SDA 3  // Назначение любого вывода Arduino для работы в качестве линии SDA программной шины I2C.
#define pin_SW_SCL 8
/*
#include <OLED_I2C.h>

OLED  myOLED(SDA, SCL, 8);
extern uint8_t TinyFont[];

extern uint8_t SmallFont[];
*/

////////////////// Значения по умолчанию ///////////////////////
//Время вкл
byte onH = 15;
byte onM = 00;

byte durOn = 60;//Длительность вкл

//Время вык
byte offH = 23; 
byte offM = 00; 

byte durOff = 60;//Длительность выкл

byte maxBr = 0; //Максимальная яркость
byte brOn = 55; //Яркость при включении
byte brOff = 60; //Яркость при выключении
///////////////////////////////////////////////////////////////////
//ВАЖНО!!

const byte pin = 9; //Номер пина на который будет подаваться сигнал

//////////////////////////////////////////////////////////////////

unsigned long timeOn;
unsigned long timeOff;
unsigned long timeCurr;

byte sig;

String strData = "";

byte S; //Секунды
byte M; //Минуты
byte H; // Часы

void setup() {
 /*   myOLED.begin();
    myOLED.setFont(SmallFont);
    myOLED.print("AQUARIUM PRO 2.0", CENTER, 32);
    myOLED.update();*/
    
    delay(1000);// Ждем готовности модуля отвечать на запросы
    Serial.begin(9600);   // Инициируем передачу данных в монитор последовательного порта на скорости 9600 бод.
    time.begin();
    
    help();
    
    delay (300);
    if (EEPROM.read(0) != 255) { //Проверка значений в EEPROM
      onH = EEPROM.read(0);
    }
    else {
      Serial.println("onH data not set!");
    }
    
    if (EEPROM.read(1) != 255) {
      onM = EEPROM.read(1);
    }
    else {
      Serial.println("onM data not set!");
    }
    
    if (EEPROM.read(2) != 255) {
      offH = EEPROM.read(2);
    }
    else {
      Serial.println("offH data not set");
    }
    
    if (EEPROM.read(3) != 255) {
      offM = EEPROM.read(3);
    }
    else {
      Serial.println("offM data not set!");
    }
    
    if (EEPROM.read(4) != 255) {
      durOn = EEPROM.read(4);
    }
    else {
      Serial.println("durOn data not set!");
    }
    
    if (EEPROM.read(5) != 255) {
      durOff = EEPROM.read(5);
    }
    else {
      Serial.println("durOff data not set!");
    }
    
    if (EEPROM.read(6) != 255) {
      maxBr = EEPROM.read(6);
    }
    else {
      Serial.println("maxBr data not set!");
    }
    
    if (EEPROM.read(7) != 255) {
      brOn = EEPROM.read(7);
    }
    else {
      Serial.println("brOn data not set!");
    }
    
    if (EEPROM.read(8) != 255) {
      brOff = EEPROM.read(8);
    }
    else {
      Serial.println("brOff data not set!");
    }
    delay (100);
  //  myOLED.clrScr();
  //  myOLED.update();
}

void help() {//Вывести все команды

    Serial.println("* Aquarium Pro v. 2.0 *");
    Serial.println("Commands are:");
    Serial.println("setTime HH:MM:SS");
    Serial.println("setTimeOn HH:MM");
    Serial.println("setTimeOff HH:MM");
    Serial.println("setDurOn 0-240");
    Serial.println("setDurOff 0-240");
    Serial.println("setMaxBr 0-240");
    Serial.println("setBrOn 0-240");
    Serial.println("setBrOff 0-240");
    Serial.println("test 0-1023");
    Serial.println("params");
    Serial.println("memTest");
    Serial.println("***********************");  
}

void readCmd() {//Парсинг команд
  if (Serial.available() == 0) 
    return;
    
  strData = Serial.readString();    
  Serial.print(">"); Serial.println(strData);

  if (strData.startsWith("setTime ")) 
  {         
    strData.remove(0, 8); // обрезать строку, оставить только значение
    H = (strData.substring(0,2)).toInt();
    strData.remove(0, 3);
    M = (strData.substring(0,2)).toInt();
    strData.remove(0, 3);
    S = (strData.substring(0,2)).toInt();
    time.settime(S,M,H);
    return;
  }
  
  if (strData.startsWith("setTimeOn")) {
           
    strData.remove(0, 10); // обрезать строку, оставить только значение
    onH = (strData.substring(0,2)).toInt();
    EEPROM.write(0, onH);
    strData.remove(0, 3);
    onM = (strData.substring(0,2)).toInt();
    EEPROM.write(1, onM);
    return;
  }
  
  if (strData.startsWith("setTimeOff")) {         
    strData.remove(0, 11); // обрезать строку, оставить только значение
    offH = (strData.substring(0,2)).toInt();
    EEPROM.write(2, offH);
    strData.remove(0, 3);
    offM = (strData.substring(0,2)).toInt();
    EEPROM.write(3, offM);
    return;
  }
  
  if (strData.startsWith("setDurOn")) {        
    strData.remove(0, 9); // обрезать строку, оставить только значение
    durOn = (strData.substring(0,3)).toInt();
    EEPROM.write(4, durOn);
    return;
  }

  if (strData.startsWith("setDurOff")) {         
    strData.remove(0, 10); // обрезать строку, оставить только значение
    durOff = (strData.substring(0,3)).toInt();
    EEPROM.write(5, durOff);
    return;
  }
  
  if (strData.startsWith("setMaxBr")) {         
    strData.remove(0, 9); // обрезать строку, оставить только значение
    maxBr = (strData.substring(0,3)).toInt();
    EEPROM.write(6, maxBr);
    return;
  }
  
  if (strData.startsWith("setBrOn")) {        
    strData.remove(0, 8); // обрезать строку, оставить только значение
    brOn = (strData.substring(0,3)).toInt();
    EEPROM.write(7, brOn);
    return;
  }
  
  if (strData.startsWith("setBrOff")) {        
    strData.remove(0, 9); // обрезать строку, оставить только значение
    brOff = (strData.substring(0,3)).toInt();
    EEPROM.write(8, brOff);
    return;
  }
  
  if (strData.startsWith("test")) {      
    strData.remove(0, 5); // обрезать строку, оставить только значение
    analogWrite(pin, (strData.substring(0,3)).toInt());
    delay(10000);
    return;
  }
  
  if (strData.startsWith("help")) {    
    help();
    delay(10000);
    return;
  }
  
  if (strData.startsWith("params")){ //Вывести текущие параметры         
    Serial.println("* Aquarium Pro v. 2.0 *");
    Serial.print("timeOn: "); Serial.print(onH); Serial.print(" "); Serial.println(onM);
    Serial.print("timeOff: "); Serial.print(offH); Serial.print(" "); Serial.println(offM);
    Serial.print("durOn: "); Serial.println(durOn);
    Serial.print("durOff: "); Serial.println(durOff);
    Serial.print("maxBr: "); Serial.println(maxBr);
    Serial.print("brOn: "); Serial.println(brOn);
    Serial.print("brOff: ");  Serial.println(brOff);
    Serial.print("***********************");
    delay(10000);
    return;
  }
  
  if (strData.startsWith("memTest")) { //Вывести все значения из EEPROM 
     Serial.println("Reading all adresses from EEPROM");
    for (int addr=0; addr<1024; addr++) { // для всех ячеек памяти (для Arduino UNO 1024)
    byte val = EEPROM.read(addr); // считываем 1 байт по адресу ячейки
    Serial.print(addr); // выводим адрес в послед. порт 
    Serial.print("\t"); // табуляция
    Serial.println(val); // выводим значение в послед. порт
  }
 Serial.println("Done!"); 
 delay(30000);
  }
 
}

void loop() {    
  time.gettime();//Получаем время.
  H = time.Hours;
  M = time.minutes;
  S = time.seconds;
  Serial.print(H);  Serial.print(":");  Serial.print(M); Serial.print(":");  Serial.print(S); // Выводим время.
  
  readCmd();
  
  timeOn = onH;
  timeOn *= 60 * 60;
  timeOn += onM * 60;
  
  timeOff = offH;
  timeOff *= 60 * 60;
  timeOff += offM * 60;
  
  timeCurr = H;
  timeCurr *= 60 * 60;
  timeCurr += M * 60 + S;  

  long x;

  if ((timeCurr < timeOn) or (timeCurr >= timeOff)) {
    Serial.print("  "); Serial.print("Night");  
    sig = 255; 
  }
  
  else if ((timeCurr >= timeOn) and (timeCurr <= (timeOn + durOn * 60))) {
    Serial.print("  "); Serial.print("Sunrise");  
//    x = (((timeCurr - timeOn) * (maxBr - brOn)) / (durOn * 60)) + brOn;
    x = timeCurr - timeOn;
    x *= maxBr - brOn;
    x /= durOn * 60;
    x += brOn;
    sig = x;
  }
  
  else  if ((timeCurr < timeOff) and (timeCurr >= (timeOff - durOff * 60)))  {
    Serial.print("  "); Serial.print("Sunset");  
    x = timeOff - timeCurr;
    x *= maxBr - brOff;
    x /= durOff * 60;
    x += brOff;
    sig = x;
  }
  
  else{ 
    Serial.print("  "); Serial.print("Day");  
    sig = maxBr;
  } 
   
  Serial.print("   ");  Serial.println(sig);
  analogWrite(pin, sig);
  delay(1000); 
}
