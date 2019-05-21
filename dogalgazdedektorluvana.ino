#include <LiquidCrystal.h>

// Kütüphaneyi arayüz pinleri ile başlattık.
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int GelenDeger;                     // Değişken tanımlama kısmında okunan analog değer, gerilime çevirme işlemi sonrasındaki değer tanımlandı.
float GelenDegerGerilim;
float GazMiktari;

int a;                              // Değişkenimizi tanımladık.
int esikDegeri = 750;               //Gaz eşik değerini belirliyoruz. Araştırmalara göre dikkate alınması gereken eşik değerini kendiniz belirleyebilirsiniz.    
int BuzzerPin = 9;                  //Buzzerın takılacağı pin numarası
int deger;                          //Sensörden okunan değer

int motor1 = 6;
int motor2 = 7;
bool durum;                         //Boolean değişkeni tanımlandı.



  void setup(){
                                   // LCD sütun ve satır sayısı ayarlandı.
     lcd.begin(16, 2);
                                   // LCD ye mesaj yazdırdım. (isteğe göre değişebilir)
     lcd.print("Algilaniyor..");
                                  // Algilaniyor..  yazısından sonra 2 saniye beklettim.
     delay(2000);
                                  //Ekranı temizledim ve daha sonra Gaz değerini yazdirdim.
     lcd.clear();
     lcd.print("GAZ MIKTARI:");
  
     Serial.begin(9600);                           // Seri haberleşmeyi başlatmış oldum.
     pinMode(BuzzerPin, OUTPUT);                   //Buzzer pinini çıkış olarak ayarlıyoruz.
     pinMode(motor1,OUTPUT);
     pinMode(motor2,OUTPUT);
     durum = 0;
 }
void loop () {
   GelenDeger = analogRead(A0);                //A0 pininden analog değer okuma
   GelenDegerGerilim=GelenDeger*(5000/1023);    //0-1023 arası değer gelecektir. 0 = 0V / 1023=5V temsil eder.
                                               // bu değeri mV cinsinden alıyoruz.  
   GazMiktari=GelenDegerGerilim/26;             //LCD de daha düşük rakamların gözükmesi için scala ediyorum.
                                               //sıcaklık değerini hesaplıyoruz
   lcd.setCursor(0,1);                          //0 sütun, 1 satır numarası. (saymaya 0 dan başladığımız için aslında 2.satırı temsil eder.)
  
  lcd.print(GazMiktari);                       //Gaz değişkeninin değerini ekrana yazdıralım.

 
  delay(1000);                                 //1 saniye aralıklarla gaz değişimini kontrol edelim.
  
 a=analogRead(A0);                             // Gaz sensörümüzün analog çıkış ucunu arduinomuza bağladığımız kısım
 Serial.println(a);                            // Sonucu ekrana yazdırıyoruz. 
 deger = analogRead(A0);                       //Sensörden analog değer okuyoruz.
  if(deger > esikDegeri)
  {                                            //Sensörden okunan değer eşik değerinden büyükse çalışır.
    lcd.setCursor(0,1);
    lcd.print("GAZ KACAGI VAR");
    delay(500);
    lcd.clear();
    digitalWrite(BuzzerPin, HIGH);
    delay(100);
    digitalWrite(BuzzerPin, LOW);
    delay(100);
  }
  else{                             //Sensörden okunan değer eşik değerinin altındaysa çalışır.
   lcd.setCursor(0,0);             //LCDde yazılacak satırı seçiyoruz.
   lcd.print("TEHLIKE YOK");
   digitalWrite(BuzzerPin, LOW);
  }
       if(durum == 1){                       //Alınan değer belirlediğimiz eşik değerinin altında ise motorun çalışma durumunu belirledim.
                    if(deger < esikDegeri){
                                      digitalWrite(motor2,HIGH); 
                                      digitalWrite(motor1,LOW);
                                      delay(1600);                 //Motorun çalışır vaziyette olma süresi ile vananın tam açılma süresini belirledim.
                                      digitalWrite(motor2,LOW);    //Tam tur döndükten sonra dönecek ve durumunu bu sefer 0 a eşitleyecek.
                                      digitalWrite(motor1,LOW);
                                      durum = 0;
                                      
                                     }
                   }
    if(durum == 0){
                    if(deger > esikDegeri)                        // Alınan değer belirlediğiniz eşik değerinden yüksek bir değer ise
                    {
                                      digitalWrite(motor1,HIGH);  //Vananın kapanması için motorun çalışma durumunu belirledim.
                                      digitalWrite(motor2,LOW);
                                      delay(1600);                 //Tam kapanma süresi kadar motor çalışacak sonrasında duracak
                                      digitalWrite(motor1,LOW);
                                      digitalWrite(motor2,LOW);
                                      durum = 1;                    //Durumu bu sefer 1 e eşitleyecektir.
      
                    }
                   }

}
