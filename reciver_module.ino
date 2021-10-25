#include <Servo.h>

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

Servo sg90;
Servo sg91;

        //Modül ile ilgili kütüphaneleri ekliyoruz
int mesaj[2];
RF24 alici(7,8);
const uint64_t kanal = 0xE8E8F0F0E1LL;
int yatay_sayac=70;
int dikey_sayac=70;

unsigned long eskizaman=0;
unsigned long yenizaman;
void setup() {
 
 // put your setup code here, to run once:
Serial.begin(38400);
 sg90.attach(3);
 sg91.attach(6);
 alici.begin();
  sg90.write(yatay_sayac);
  delay(10);
  sg91.write(dikey_sayac);
  delay(10);
 alici.openReadingPipe(1,kanal);
 alici.startListening();
}

void loop() {
  // put your main code here, to run repeatedly:
 
if (alici.available())
 {
    alici.read(&mesaj, sizeof(mesaj) );
//    Serial.print(mesaj[0]);
//    Serial.print("/t/t");
//    Serial.println(mesaj[1]);
    sg90.write(mesaj[0]);
    delay(10);
    sg91.write(mesaj[1]);
    delay(10);
 }

  

}
