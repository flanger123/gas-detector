#include <Adafruit_BME280.h>
#include <DFPlayer_Mini_Mp3.h>
#include "U8glib.h"
#define SEALEVELPRESSURE_HPA (1013.25)
        Adafruit_BME280 bme; 
#define PIN 3    
#include "GyverButton.h"
GButton butt1(PIN);
#include <iarduino_RTC.h>
iarduino_RTC time(RTC_DS3231);

SoftwareSerial mySerial(5, 4);

//unsigned long timer=0;
int count=0;

int value = 0;
int mq2=0;
int mq9=0;
int mq135=0;

U8GLIB_SSD1306_128X32 u8g(U8G_I2C_OPT_NONE);	// I2C / TWI 

void draw(void) {
 String str;
 char result[8];
  // graphic commands to redraw the complete screen should be placed here  
  u8g.setFont(u8g_font_unifont);
  //u8g.setFont(u8g_font_osb21);
  u8g.drawStr( 7, 10, "TEMP:");
  str=String(bme.readTemperature());
  str.toCharArray(result,5); 
  u8g.drawStr( 60, 10, result);
  u8g.drawStr( 93, 10, "C");

  str=String(bme.readHumidity());
  str.toCharArray(result,5); 
  u8g.drawStr( 7, 21, "HUMID:");
  u8g.drawStr( 60, 21, result);
  u8g.drawStr( 93, 21, "%");

  str=String(bme.readPressure()/133);
  str.toCharArray(result,6); 
  u8g.drawStr( 7, 32, "PRESS:");
  u8g.drawStr( 60, 32, result);

  
}

void setup(void) {
  bme.begin();
  
                        // assign default color value
                        if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
                          u8g.setColorIndex(255);     // white
                        }
                        else if ( u8g.getMode() == U8G_MODE_GRAY2BIT ) {
                          u8g.setColorIndex(3);         // max intensity
                        }
                        else if ( u8g.getMode() == U8G_MODE_BW ) {
                          u8g.setColorIndex(1);         // pixel on
                        }
                        else if ( u8g.getMode() == U8G_MODE_HICOLOR ) {
                          u8g.setHiColorByRGB(255,255,255);
                        }
  
  pinMode(8, OUTPUT);
  pinMode(A0, INPUT);  //mq2
  pinMode(A1, INPUT);  //mq9
  pinMode(A2, INPUT);  //mq135
   
   Serial.begin(9600);
   mySerial.begin (9600); // открываем софт-порт
   mp3_set_serial (mySerial); // задаем порт для плеера
   mp3_set_volume (30); //устанавливаем уровень громкости

  time.begin();
   mp3_play (200);
   delay(2400);

    butt1.setDebounce(50);        // настройка антидребезга (по умолчанию 80 мс)
    butt1.setTimeout(300);        // настройка таймаута на удержание (по умолчанию 500 мс)
    butt1.setClickTimeout(300);   // настройка таймаута между кликами (по умолчанию 300 мс)
    butt1.setType(HIGH_PULL);
     butt1.setDirection(NORM_OPEN);
}
void loop(void) {
  butt1.tick(); 
  u8g.firstPage();  
  do {
    draw();
  } while( u8g.nextPage() );


  getmq();
  //delay(500);
   //mp3_play (1);

    //if (time.Hours == 9 && time.minutes == 0) 
     if (butt1.isClick()) run_platon();
}

void run_platon(){

  mp3_play (200);
  delay(2400);
    mp3_play (100); //добрый день
    delay(1000);

 /////////////////
//ТЕМПЕРАТУРА ДОМА
/////////////////
    mp3_play (101);//температура в помещении
    delay(1500);
    //Serial.println(int(bme.readTemperature()));
    float t=bme.readTemperature();
    float h=bme.readHumidity();

  if(t>=20){
    int temp1=int((int(t))/10)*10;
    int temp2=int(t-temp1);

    mp3_play(temp1);  //двадцать
    delay(600);
    mp3_play(temp2); //пять


      if(temp2==0){
        delay(100);
        mp3_play (112); //градусов
        }

      if(temp2==1){
        delay(500);
        mp3_play (110); //градус   
      }

      if(temp2>1&&temp2<5){
        delay(500);
        mp3_play (111); //градуса
      }

      if(temp2>4){
        delay(400);
        mp3_play (112); //градусов
      }

  }
  else{
    mp3_play(int(t));
    delay(500);
    mp3_play (110); //градусов
    }

  delay(1000);

/////////////
//ВЛАЖНОСТЬ ДОМА
/////////////
  mp3_play (102);
  delay(1500);

 if(h>=20){
  
  int vl1= int((int(h))/10)*10;
  int vl2= int(h-vl1);


  mp3_play(vl1); //тридцать
  delay(600);
  mp3_play(vl2); //восемь


      if(vl2==0){
        delay(100);
        mp3_play (122); //процентов
      }
      
      if(vl2==1){
        delay(500);
        mp3_play (120);//процент
      }
      
      if(vl2>1&&vl2<5){
        delay(500);
        mp3_play (121);//процента
      }
      
      if(vl2>4){
        delay(500);
        mp3_play (122);//процентов
        }
 }
 else{
    mp3_play(int(h));
    delay(500);
    mp3_play (122); //процентов
    }

    delay(1000);
    mp3_play (164); //приятного дня
    delay(2000);
    mp3_play (105); //приятного дня
    delay(1000);
    mp3_play (106); //приятного дня
    delay(1000);
  
}






void getmq(){
        
        Serial.print("MQ2: ");
        mq2 = analogRead(A0);
        Serial.println(mq2);
        
       
        Serial.print("MQ9: ");
        mq9 = analogRead(A1);
        Serial.println(mq9);
      
      
        Serial.print("MQ135: ");
        mq135 = analogRead(A2);
        Serial.println(mq135);


        int mq = max(mq2, mq9); 
      
        mq = max(mq, mq135);
      
        int range = map(mq, 0, 1023, 1, 5);
  
        switch (range) {
                case 2:
                   count++;
                   if(count>60)
                   {
                    mp3_play (160);//зафиктирован рост концентрации углеводородных газов
                    count=0;
                   }
                  //wdt_delay(5000);
                  delay(5000);
                  break;
                case 3:
                  count++;  
                  if(count>30)
                   { 
                    mp3_play (161);//сильня концентрация углеводородных газов
                    //wdt_delay(5000);
                    count=0;
                   }
                  delay(5000);
                  break;
                case 4:
                 count++;  
                  if(count>5)
                   {   
                  mp3_play (162);//концентрация углеводородных газов критическая
                  //wdt_delay(5000);
                   count=0;
                   }
                  delay(5000);
                  break;
                }

   
  }
