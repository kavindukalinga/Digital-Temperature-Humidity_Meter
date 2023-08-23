#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <RTClib.h>
#include <dht.h>

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
int led=13;
int buz=12;
char tempunit='C';
int safetemp=40;
float hourtemp=0;
float todaytemp=0;
int page=0;
float z;
float x;
int th; int tm;int dy;int dm;int dd;
DateTime now;

RTC_DS3231 rtc;
dht tnh;

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

void setup() {
  pinMode(led, OUTPUT);pinMode(buz, OUTPUT);
  pinMode(2,INPUT); pinMode(3,INPUT);
  
  attachInterrupt(0,button1,RISING);
  attachInterrupt(1,button2,RISING);
  
  Serial.begin(9600);
  Wire.begin();
  rtc.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
  //delay(1000);
  display.clearDisplay();
  rtc.adjust(DateTime(2023, 4, 19, 12, 45, 30));
}

void loop() {
  now = rtc.now();
  int tval=tnh.read11(7);
  display.clearDisplay();
  display.setTextColor(WHITE);
  x=tnh.temperature;

  if (int(x)> todaytemp) todaytemp=x;
  if (now.hour()==0 && now.minute()==0) todaytemp=0;
  
  if (int(x)> hourtemp) hourtemp=x;
  if (now.minute()==0) hourtemp=0;
  
  if (page==0) page0();
    
  else if (page==1) page1();
  
  else if (page==2) page2();
  
  else if (page==3) page3();
  display.display();
  delay(1000);
}

void button1() {
  if (page>=3) page=0;
  else page++;
}

void button2() {
  if (page==0){
  if (tempunit=='C') tempunit='F';
  else tempunit = 'C';
  }
  else if (page==1){
    if (tempunit=='C') tempunit='F';
    else tempunit = 'C';
  }
  else if (page==2){
    safetemp+=1;
    if (safetemp>60) safetemp=10;
//    th = now.hour() + 1; 
//    if (th > 23) {
//        th = 0; 
//    }
//    rtc.adjust(DateTime(now.year(), now.month(), now.day(), th, now.minute(), 15));
  }
}

float ctof(float x){
  return (x*9/5+32);
}

void page0(){
  //Time
    display.setTextSize(2);
    display.setCursor(100,0);
    if(now.hour() < 10) display.print("0");
    display.print(now.hour(), DEC);
    display.setCursor(100,16);
    if(now.minute() < 10) display.print("0");
    display.print(now.minute(), DEC);
  //  if(now.second() < 10) display.print("0");
  //  display.println(now.second(), DEC);
    
    //Temp  
    display.setTextSize(2);
    display.setCursor(0,0);
    if (tempunit=='F'){
      display.print(int(ctof(x)));
      display.print(" F");  }
    else {
      display.print(int(x));
      display.print(" C");
    }
    if (x>safetemp){
      digitalWrite(13,HIGH);
    }else{
      digitalWrite(13,LOW);
    }
    
    //Humidity
    z=tnh.humidity;
    display.setTextSize(1);
    display.setCursor(0,16);
    display.print("H ");
    display.print(int(z));
    display.print(" %");
  
    
    //Verbal Output
    display.setTextSize(1);
    display.setCursor(16,24);
    if (x>30) display.print(" Hot Day");
    else if (x<20) display.print(" Cool Day");
    else if (x>27 && z>60) display.print(" Swaety Day");
    else display.print(" Comfortable");
    
  //  display.setTextSize(1);
  //  display.print(now.day(), DEC);display.print("/");
  //  display.print(now.month(), DEC);display.print("/");
  //  display.println(now.year(), DEC);
  }



void page1(){
  display.setTextSize(1);
  display.setCursor(0,0);
  if (tempunit=='F'){  
    display.print("Now: ");
    display.print(ctof(x));
    display.println(" F");        
    display.print("This Hour: ");
    display.print(int(ctof(hourtemp)));
    display.println(" F");
    display.print("Today: ");
    display.print(int(ctof(todaytemp)));
    display.println(" F");  }
  else {
    display.print("Now: ");
    display.print(x);
    display.println(" C");
    display.print("This Hour: ");
    display.print(int(hourtemp));
    display.println(" C");
    display.print("Today: ");
    display.print(int(todaytemp));
    display.println(" C");
    }
  float z=tnh.humidity;
  display.setTextSize(1);
  display.setCursor(0,24);
  display.print("Humidity: ");
  display.print(z);
  display.print(" %");
  }

void page2(){
  display.setTextSize(1);
    display.setCursor(0,0);
    if (tempunit=='F'){
      display.print(int(ctof(x)));
      display.print(" F");  }
    else {
      display.print(int(x));
      display.print(" C");
    }
    display.setTextSize(1);
    display.setCursor(0,8);
    display.print("Safe Temperature");
    display.setTextSize(2);
    display.setCursor(0,16);
    if (tempunit=='F'){
      display.print(int(ctof(safetemp)));
      display.print(" F");  }
    else {
      display.print(safetemp);
      display.print(" C");
    }
}

void page3(){
  display.setTextSize(2);
  display.setCursor(0,0);
  if(now.hour() < 10) display.print("0");
  display.print(now.hour(), DEC); display.print(":");
  if(now.minute() < 10) display.print("0");
  display.print(now.minute(), DEC);display.print(":");
  if(now.second() < 10) display.print("0");
  display.println(now.second(), DEC);
  
  display.setTextSize(1);
  display.setCursor(0,24);
  display.print(now.day(), DEC);display.print("/");
  display.print(now.month(), DEC);display.print("/");
  display.print(now.year(), DEC);display.print(" ");
  display.print(daysOfTheWeek[now.dayOfTheWeek()]);
  }


//void page3(){
//  display.setTextSize(2);
//  display.setCursor(0,0);
//  if(now.hour() < 10) display.print("0");
//  display.print(now.hour(), DEC);
//  display.print(" : ");
//  if(now.minute() < 10) display.print("0");
//  display.print(now.minute(), DEC);
//  
//}
