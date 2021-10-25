#include <Wire.h>
#include <MPU6050.h>
#include <I2Cdev.h>

#include <SimpleSDAudio.h>
#include  <SPI.h> // SPI Arayüzünü dahil ettik.
#include <nRF24L01.h>
#include <RF24.h> // RF24 kütüpanelerini dahil ettik.


int msg[2]; //Göndereceğimiz mesaj değişkenini oluşturduk.
RF24 radio(7, 8); //
const uint64_t pipe = 0xE8E8F0F0E1LL;

MPU6050 ivme_sensor;
int16_t ax,ay,az;
int16_t gx,gy,gz;

int kalibrasyon_delay = 1000;
int sol, sag, asagi, yukari;
int sol_goz,sag_goz,asagi_goz,yukari_goz;
int buzzer = 5;
byte com = 0;
int kosul = 1;
int eog = 0;
int kalibrasyon_kosul = 1;
int goz_yatay;
int goz_dikey;
int yatay_sayac = 90;
int dikey_sayac = 90;
int dur = 1;
int kalibrasyon_durum=0;
unsigned long eskizaman=0;
unsigned long yenizaman;
int egim=0;
int goz_kalibrasyon=0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(38400);


  Serial.write(0xAA);

  Serial.write(0x37);

  delay(1000);

  Serial.write(0xAA);

  Serial.write(0x21);


  Serial.println("basladı");

  radio.begin(); // Kablosuz iletişimi başlattık.
  radio.openWritingPipe(pipe); //Gönderim yapacağımız kanalın ID'sini tanımladık.

  Wire.begin();
  ivme_sensor.initialize();
  

  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, HIGH);

  if (!SdPlay.init(SSDA_MODE_HALFRATE | SSDA_MODE_MONO | SSDA_MODE_AUTOWORKER)) {
    while (1);
  }
  SdPlay.setSDCSPin(4);
  digitalWrite(buzzer, LOW);
  delay(50);
  digitalWrite(buzzer, HIGH);

  Serial.println("setup");
  while (kalibrasyon_kosul == 1) {
    if (Serial.available() > 0) {

      com = Serial.read();
      Serial.println(com);
      if (com == 17) {
        kalibrasyon_kosul = 0;
        Serial.println("Sistem Başlatılıyor");

      }
    }
  }
  SdPlay.setFile("mer.wav");
  SdPlay.play(); // seçilen dosyayı çal
  while (!SdPlay.isStopped()) { // müzik bittiğinde tekrar çal
    ;
  }

}


void kalibrasyon() {
  goz_kalibrasyon=1;
  if (dur == 1) {
    Serial.println("kalibrasyon");
    SdPlay.setFile("haz.wav");
    SdPlay.play(); // seçilen dosyayı çal
    while (!SdPlay.isStopped()) { // müzik bittiğinde tekrar çal
      ;
    }
  }

  if (dur == 1) {
    kalibrasyon_kosul = 1;
    while (kalibrasyon_kosul == 1) {
      if (Serial.available() > 0) {
        com = Serial.read();
        Serial.println(com);
        if (com == 17) {
          kalibrasyon_kosul = 0;
          Serial.println("Sistem Başlatılıyor");

        }
        else if (com == 18) {
          kalibrasyon_durdur();
          dur = 0;
          kalibrasyon_kosul = 0;
        }
      }
    }

  }//DUR
  if (dur == 1) {
    /// SOL
    SdPlay.setFile("sol.wav");
    SdPlay.play(); // seçilen dosyayı çal
    while (!SdPlay.isStopped()) { // müzik bittiğinde tekrar çal
      ;
    }
    kalibrasyon_kosul = 1;
    while (kalibrasyon_kosul == 1) {
      if (Serial.available() > 0) {
        com = Serial.read();
        Serial.println(com);
        if (com == 17) {
          kalibrasyon_kosul = 0;
          Serial.println("Sistem Başlatılıyor");

        }
        else if (com == 18) {
          kalibrasyon_durdur();
          dur = 0;
          kalibrasyon_kosul = 0;
        }
      }
    }
  }
  if (dur == 1) {
    for (int x = 0; x < 4; x++) {
      digitalWrite(buzzer, LOW);
      delay(200);
      digitalWrite(buzzer, HIGH);
      delay(200);
    }
    digitalWrite(buzzer, LOW);
    delay(kalibrasyon_delay);
    sol = analogRead(A0);

    delay(kalibrasyon_delay / 2);
    digitalWrite(buzzer, HIGH);
    Serial.println(sol);


  }//DUR
  if (dur == 1) {
    ///SAĞ
    SdPlay.setFile("sag.wav");
    SdPlay.play(); // seçilen dosyayı çal
    while (!SdPlay.isStopped()) { // müzik bittiğinde tekrar çal
      ;
    }

    kalibrasyon_kosul = 1;
    while (kalibrasyon_kosul == 1) {
      if (Serial.available() > 0) {
        com = Serial.read();
        Serial.println(com);
        if (com == 17) {
          kalibrasyon_kosul = 0;
          Serial.println("Sistem Başlatılıyor");

        }
        else if (com == 18) {
          kalibrasyon_durdur();
          dur = 0;
          kalibrasyon_kosul = 0;
        }
      }
    }
  }
  if (dur == 1) {
    for (int x = 0; x < 4; x++) {
      digitalWrite(buzzer, LOW);
      delay(200);
      digitalWrite(buzzer, HIGH);
      delay(200);
    }
    digitalWrite(buzzer, LOW);
    delay(kalibrasyon_delay);
    sag = analogRead(A0);

    delay(kalibrasyon_delay / 2);
    digitalWrite(buzzer, HIGH);
    Serial.println(sag);


  }

  if (dur == 1) {
    //// AŞAĞI
    SdPlay.setFile("as.wav");
    SdPlay.play(); // seçilen dosyayı çal
    while (!SdPlay.isStopped()) { // müzik bittiğinde tekrar çal
      ;
    }
    kalibrasyon_kosul = 1;
    while (kalibrasyon_kosul == 1) {
      if (Serial.available() > 0) {
        com = Serial.read();
        Serial.println(com);
        if (com == 17) {
          kalibrasyon_kosul = 0;
          Serial.println("Sistem Başlatılıyor");

        }
        else if (com == 18) {
          kalibrasyon_durdur();
          dur = 0;
          kalibrasyon_kosul = 0;
        }
      }
    }
  }

  if (dur == 1) {
    for (int x = 0; x < 4; x++) {
      digitalWrite(buzzer, LOW);
      delay(200);
      digitalWrite(buzzer, HIGH);
      delay(200);
    }
    digitalWrite(buzzer, LOW);
    delay(kalibrasyon_delay);
    asagi = analogRead(A1);

    delay(kalibrasyon_delay / 2);
    digitalWrite(buzzer, HIGH);
    Serial.println(asagi);

  }

  if (dur == 1) {

    ///YUKARI
    SdPlay.setFile("yu.wav");
    SdPlay.play(); // seçilen dosyayı çal
    while (!SdPlay.isStopped()) { // müzik bittiğinde tekrar çal
      ;
    }

    kalibrasyon_kosul = 1;
    while (kalibrasyon_kosul == 1) {
      if (Serial.available() > 0) {
        com = Serial.read();
        Serial.println(com);
        if (com == 17) {
          kalibrasyon_kosul = 0;
          Serial.println("Sistem Başlatılıyor");

        }
        else if (com == 18) {
          kalibrasyon_durdur();
          dur = 0;
          kalibrasyon_kosul = 0;
        }
      }
    }

  }

  if (dur == 1) {

    for (int x = 0; x < 4; x++) {
      digitalWrite(buzzer, LOW);
      delay(200);
      digitalWrite(buzzer, HIGH);
      delay(200);
    }
    digitalWrite(buzzer, LOW);
    delay(kalibrasyon_delay);
    yukari = analogRead(A1);

    delay(kalibrasyon_delay / 2);
    digitalWrite(buzzer, HIGH);
    Serial.println(yukari);
    SdPlay.setFile("an.wav");
    SdPlay.play(); // seçilen dosyayı çal
    while (!SdPlay.isStopped()) { // müzik bittiğinde tekrar çal
      ;
    }

  }

  if (dur == 1) {


    if (sol < 160) {
      if (sag > 700) {
        if (asagi < 160) {
          if (yukari > 700) {
            SdPlay.setFile("son.wav");
            SdPlay.play(); // seçilen dosyayı çal
            while (!SdPlay.isStopped()) { // müzik bittiğinde tekrar çal
              ;
            }
            kalibrasyon_durum=1;
            kalibrasyon_kosul = 1;
            sol_goz=sol+30;
            sag_goz=sag-30;
            asagi_goz=asagi+30;
            yukari_goz=yukari-30;
            while (kalibrasyon_kosul == 1) {
              if (Serial.available() > 0) {
                com = Serial.read();
                Serial.println(com);
                if (com == 17) {
                  kalibrasyon_kosul = 0;
                  Serial.println("Sistem Başlatılıyor");

                }
                else if (com == 18) {
                  kalibrasyon_durdur();
                  kalibrasyon_kosul = 0;
                  dur = 0;
                }
              }
            }


          }
          else {
            SdPlay.setFile("yua.wav");
            SdPlay.play(); // seçilen dosyayı çal
            while (!SdPlay.isStopped()) { // müzik bittiğinde tekrar çal
              ;
            }
            kalibrasyon_kosul = 1;
            while (kalibrasyon_kosul == 1) {
              if (Serial.available() > 0) {
                com = Serial.read();
                Serial.println(com);
                if (com == 19) {

                  kalibrasyon();
                  Serial.println("Sistem Başlatılıyor");

                }
                else if (com == 18) {
                  kalibrasyon_durdur();
                  kalibrasyon_kosul = 0;
                  dur = 0;
                }
              }
            }

          }
        }
        else {
          SdPlay.setFile("asa.wav");
          SdPlay.play(); // seçilen dosyayı çal
          while (!SdPlay.isStopped()) { // müzik bittiğinde tekrar çal
            ;
          }
          kalibrasyon_kosul = 1;
          while (kalibrasyon_kosul == 1) {
            if (Serial.available() > 0) {
              com = Serial.read();
              Serial.println(com);
              if (com == 19) {
                kalibrasyon();
                Serial.println("Sistem Başlatılıyor");

              }
              else if (com == 18) {
                kalibrasyon_durdur();
                kalibrasyon_kosul = 0;
                dur = 0;
              }
            }
          }
        }
      }
      else {
        SdPlay.setFile("saga.wav");
        SdPlay.play(); // seçilen dosyayı çal
        while (!SdPlay.isStopped()) { // müzik bittiğinde tekrar çal
          ;
        }
        kalibrasyon_kosul = 1;
        while (kalibrasyon_kosul == 1) {
          if (Serial.available() > 0) {
            com = Serial.read();
            Serial.println(com);
            if (com == 19) {
              kalibrasyon();
              Serial.println("Sistem Başlatılıyor");

            }
            else if (com == 18) {
              kalibrasyon_durdur();
              kalibrasyon_kosul = 0;
              dur = 0;
            }
          }
        }
      }
    }
    else {
      SdPlay.setFile("sola.wav");
      SdPlay.play(); // seçilen dosyayı çal
      while (!SdPlay.isStopped()) { // müzik bittiğinde tekrar çal
        ;
      }
      kalibrasyon_kosul = 1;
      while (kalibrasyon_kosul == 1) {
        if (Serial.available() > 0) {
          com = Serial.read();
          Serial.println(com);
          if (com == 19) {
            kalibrasyon();
            Serial.println("Sistem Başlatılıyor");

          }
          else if (com == 18) {
            kalibrasyon_durdur();
            kalibrasyon_kosul = 0;
            dur = 0;
          }
        }
      }
    }
    if (dur == 1) {
      for (int x = 0; x < 7; x++) {
        digitalWrite(buzzer, LOW);
        delay(100);
        digitalWrite(buzzer, HIGH);
        delay(100);
      }
    }

  }//DUR
}

void gonder(){
 radio.write(&msg, sizeof(msg));
}

void kalibrasyon_durdur() {
  SdPlay.setFile("kald.wav");
  SdPlay.play(); // seçilen dosyayı çal
  while (!SdPlay.isStopped()) { // müzik bittiğinde tekrar çal
    ;
  }
  dur = 0;
  goz_kalibrasyon=0;
}




void loop() {
  // put your main code here, to run repeatedly:

 yenizaman=millis();
  while (Serial.available()) {

    com = Serial.read();

    Serial.println(com);

    switch (com) {

      case 17:   //command HAZIRIM

        Serial.println("17");


        break;

      case 18:  //command DURDUR

        Serial.println("18");

        eog = 0;


        break;

      case 19:  //command KALİBRASYON

        Serial.println("19");
        kalibrasyon();
        dur = 1;
        kalibrasyon_kosul = 1;
        break;

      case 20:  //command GÖZÜ AKTİFLEŞTİR
        Serial.println("20");
        SdPlay.setFile("goz.wav");
        SdPlay.play(); // seçilen dosyayı çal
        while (!SdPlay.isStopped()) { // müzik bittiğinde tekrar çal
          ;
        }
        eog = 1;

        break;

      case 21:  //command SİSTEMİ ÇALIŞTIR
        SdPlay.setFile("hara.wav");
        SdPlay.play(); // seçilen dosyayı çal
        while (!SdPlay.isStopped()) { // müzik bittiğinde tekrar çal
          ;
        }
        Serial.println("21");
        egim=1;

        break;

    }
  }

  while (eog == 1) {

    yenizaman=millis();
     


 if(yenizaman-eskizaman>300){
     goz_yatay = analogRead(A0);

     if(goz_kalibrasyon==0){
    
    if (goz_yatay < 100) {
      yatay_sayac = yatay_sayac - 15;
    }
    else if (goz_yatay > 740) {
      yatay_sayac = yatay_sayac + 15;
    }
    if (yatay_sayac > 180) {
      yatay_sayac = 180;
    }
    else if (yatay_sayac < 0) {
      yatay_sayac = 0;
    }
    msg[0] = yatay_sayac;
    goz_dikey = analogRead(A1);
    if (goz_dikey < 100) {
      dikey_sayac = dikey_sayac - 15;
    }
    else if (goz_dikey > 740) {
      dikey_sayac = dikey_sayac + 15;
    }
    if (dikey_sayac > 180) {
      dikey_sayac = 180;
    }
    else if (dikey_sayac < 0) {
      dikey_sayac = 0;
    }
    
     msg[1] = dikey_sayac;

     }
     if(goz_kalibrasyon==1){
     
      if (goz_yatay < sol_goz) {
      yatay_sayac = yatay_sayac - 15;
    }
    else if (goz_yatay > sag_goz) {
      yatay_sayac = yatay_sayac + 15;
    }
    if (yatay_sayac > 180) {
      yatay_sayac = 180;
    }
    else if (yatay_sayac < 0) {
      yatay_sayac = 0;
    }
    msg[0] = yatay_sayac;
    goz_dikey = analogRead(A1);
    if (goz_dikey < asagi_goz) {
      dikey_sayac = dikey_sayac - 15;
    }
    else if (goz_dikey > yukari_goz) {
      dikey_sayac = dikey_sayac + 15;
    }
    if (dikey_sayac > 180) {
      dikey_sayac = 180;
    }
    else if (dikey_sayac < 0) {
      dikey_sayac = 0;
    }
    
     msg[1] = dikey_sayac;

     }

     
     
     gonder();
 
     eskizaman=yenizaman;
 }
     
     
    
    

    

    
     

while (Serial.available() > 0) {
      com = Serial.read();
      Serial.println(com);
      if (com == 18) {
        eog = 0;
        SdPlay.setFile("gozd.wav");
        SdPlay.play(); // seçilen dosyayı çal
        while (!SdPlay.isStopped()) { // müzik bittiğinde tekrar çal
          ;
        }
      }
    }
  }
    

    while (egim == 1) {

    yenizaman=millis();
    ivme_sensor.getMotion6(&ax,&ay,&az,&gx,&gy,&gz);

 if(yenizaman-eskizaman>200){
     
     msg[0] = map(ay,-20000,20000,10,170);
     
     msg[1] = map(az,-20000,20000,20,160);    
     gonder();

     eskizaman=yenizaman;
 }
    delay(100);
     
  

    

    
     

while (Serial.available() > 0) {
      com = Serial.read();
      Serial.println(com);
      if (com == 18) {
        egim = 0;
        SdPlay.setFile("hard.wav");
        SdPlay.play(); // seçilen dosyayı çal
        while (!SdPlay.isStopped()) { // müzik bittiğinde tekrar çal
          ;
        }
      }
    }
  }
    
   
}
