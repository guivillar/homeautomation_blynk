#define BLYNK_PRINT Serial

#include <StopWatch.h>
#include <SPI.h>
#include <Ethernet.h>
#include <BlynkSimpleEthernet.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "qu1dZovtFRPvQjTU-h-Q1O2bY2mkNMck";

#define W5100_CS  10
#define SDCARD_CS 4

int stButton;
unsigned long currentTime;
int pinData = 0;
int s=0, m=0, h=0;
long current, previous;

StopWatch mySW;
WidgetLCD lcd(V2);


BLYNK_WRITE(V1) //Button Widget is writing to pin V1
{
  pinData = param.asInt();
  if (pinData == 1){
    s = 0;
    m = 0;
    h = 0;
    previous = 0;
    current = 0;
    lcd.clear();
    mySW.start();
    Serial.println("Startou");
  }
  else if (pinData == 0){
    mySW.stop();
    mySW.reset();
    Serial.println("Stopou");
    //lcd.print(4, 0, "DESLIGADO!");
    
  }
}

void relogio(){
  current = mySW.elapsed();
  if ((current - previous) >= 1000){
    previous = previous + 1000;
    s = s + 1;
    if (s == 60){
      s = 0;
      m = m + 1;
      if (m == 60){
        m = 0;
        h = h + 1;
      }
    }
  }

  
  if(h=0){lcd.print(4, 0, "0");}
  else{lcd.print(4, 0, h);}
  lcd.print(5, 0, "h");
  if(m<10){lcd.print(6, 0, "0");
  lcd.print(7, 0, m);}
  else{lcd.print(6, 0, m);}
  lcd.print(8, 0, "m");
  if(s<10){lcd.print(9, 0, "0");
  lcd.print(10, 0, s);}
  else{lcd.print(9, 0, s);}
  lcd.print(11, 0, "s");
}

void setup()
{
  // Debug console
  Serial.begin(9600);
  
  //pinMode(SDCARD_CS, OUTPUT);
  //digitalWrite(SDCARD_CS, HIGH); // Deselect the SD card

  Blynk.begin(auth);
  
  //currentTime = millis();
}

void loop()
{
  stButton = digitalRead(5);
  //Serial.println(stButton);
  if (pinData == 1){
    currentTime = mySW.elapsed();
    relogio();
    if (stButton == 1){
      Serial.println("Esta desligado");
      Serial.println(mySW.elapsed());
      if (currentTime >= 3600L * 1000){
        digitalWrite(5, 0);
        stButton = 1;
        mySW.reset();
        mySW.start();
        Serial.println("Ligado");
        s = 0;
        m = 0;
        h = 0;
        previous = 0;
        current = 0;
      }
    }
    else if (stButton == 0){
      Serial.println("Esta ligado");
      Serial.println(mySW.elapsed());
      if (currentTime >= 3600L * 2000){
        digitalWrite(5, 1);
        stButton = 0;
        mySW.reset();
        mySW.start();
        Serial.println("Desligado");
        s = 0;
        m = 0;
        h = 0;
        previous = 0;
        current = 0;
      }
    }
  }
  Blynk.run();
}
