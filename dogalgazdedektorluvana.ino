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

///Client için yazdığım

#include <ESP8266WiFi.h>

const char* ssid = "YILANDIL";                  // Your wifi Name       
const char* password = "hakanyilandil1996"; // Your wifi Password
const char * host = "192.168.2.173";        // IP Server

const int httpPort = 80;
uint8_t BUTON10= D1;
uint8_t BUTON11= D2;
uint8_t BUTON20= D3;
uint8_t BUTON21= D4;
uint8_t BUTON30= D5;
uint8_t BUTON31= D6;
String Commands10 ="F";                    // The command variable that is sent to theserver
String Commands11 ="K";                     // The command variable that is sent to theserver
String Commands20 ="H";                      // The command variable that is sent to theserver                     
String Commands21 ="Y"; 
String Commands30 ="R"; 
String Commands31 ="C";
//int button = 5;                             // push button is connected

//bool btn_press = true;                      // The variable to detect the button has been pressed
String tampon[120];
int buton_basim_sayisi = 0;                                // Variables for mode

void setup() {
  pinMode(BUTON10, INPUT_PULLUP);            // initialize the pushbutton pin as an input:
  pinMode(BUTON11, INPUT_PULLUP);
  pinMode(BUTON20, INPUT_PULLUP);
  pinMode(BUTON21, INPUT_PULLUP);
  pinMode(BUTON30, INPUT_PULLUP);
  pinMode(BUTON31, INPUT_PULLUP);
  Serial.begin(115200);                     // initialize serial:
  Serial.println("");
  Serial.println("Client-------------------------------");
  Serial.print("Connecting to Network ");
  WiFi.mode(WIFI_STA);                      // Mode Station
  WiFi.begin(ssid, password);               // Matching the SSID and Password
  delay(1000);

  // Waiting to connect to wifi
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("Successfully Connecting");  
  Serial.println("Status : Connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("-------------------------------------");
  Serial.println("");
}

void loop() {
  // put your main code here, to run repeatedly:  
  if (digitalRead(BUTON10) == LOW) 
  {
    if(buton_basim_sayisi==0 && tampon[0] == "/O")
    {
      Serial.println("Client--------------------------");
      Serial.print("Send Command = ");
      Serial.println(Commands10);
      send_commands(Commands10);
     }
        else if(buton_basim_sayisi <120)
        {
          buton_basim_sayisi++;
          tampon[buton_basim_sayisi] = Commands10; 
          Serial.println(  buton_basim_sayisi); 
          Serial.println( tampon[buton_basim_sayisi]);         
        }
   }
  delay(100);
  if(digitalRead(BUTON11) == LOW)
  {
    if(buton_basim_sayisi == 0 && tampon[0] == "/0")
    {
      Serial.println("Client----------------------");
      Serial.print("Send Command = ");
      Serial.println(Commands11);
      send_commands(Commands11);      
    }
      else if(buton_basim_sayisi < 120)
      {
        buton_basim_sayisi++;  // tampon index arttır.
        tampon[buton_basim_sayisi] = Commands11;
         Serial.println(tampon[buton_basim_sayisi]); 
         Serial.println(  buton_basim_sayisi);  
      }
    
  }
  if(digitalRead(BUTON20) ==LOW)
  {
    if(buton_basim_sayisi == 0 && tampon[0] == "/0")
    {
      Serial.println("Client----------------------");
      Serial.print("Send Command = ");
      Serial.println(Commands20);
      send_commands(Commands20); 
    }
    else if(buton_basim_sayisi < 120)
      {
        buton_basim_sayisi++;  // tampon index arttır.
        tampon[buton_basim_sayisi] = Commands20;
         Serial.println(tampon[buton_basim_sayisi]); 
         Serial.println(buton_basim_sayisi);  
      }
   }
   if(buton_basim_sayisi > 0) // eğer tamponda iş var ise.
                {
                    send_commands(tampon[buton_basim_sayisi]); // tampona son girilen işi gönder.
                    buton_basim_sayisi--;  // ve tamponu 1 azalt ( 1 iş eksildiği için.)
                }
                
                if(buton_basim_sayisi < 0)
                    {
                        buton_basim_sayisi = 0;
                    }
                else if(buton_basim_sayisi > 120)
                    {
                        buton_basim_sayisi = 120;  // tamponu taşırma.
                    } 
 }

void send_commands(String ex){
  Serial.println("Sending command...");
  Serial.println("Don't press the button for now...");
  Serial.println("");
  Serial.print("Connecting to ");
  Serial.println(host);
  WiFiClient client;
  
  if (!client.connect(host, httpPort)) {
    Serial.println("Connection failed");
    return;
  }
 
  Serial.print("Requesting URL : ");
  Serial.println(ex);

  // This will send the request to the server
  client.print(String("GET ") + ex + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: Close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {      
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  
  Serial.print("Server Reply = "); 
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  Serial.println("Now you can press the button ...");
  Serial.println("-------------------------------------");
  Serial.println("");
}


///Server için yazdığım

#include <SPI.h>
#include <ESP8266WiFi.h>
uint8_t BUTON1= D4;
uint8_t BUTON2= D5;
uint8_t BUTON3= D6;

#define KULLANICI_1 'F'
#define KULLANICI_2 'R'
#define KULLANICI_3 'H'
char ssid[] = "SOLDAMATIC";               // SSID of your home WiFi
char pass[] = "";               // password of your home WiFi

IPAddress server(192,168,0,80); 
WiFiClient client;                   
char tampon[120];
int buton_basim_sayisi=0;
void setup() {
        Serial.begin(115200); 
        delay(10);
        Serial.printf("Connecting to%s ", ssid);
        WiFi.begin(ssid, pass);                 // connects to the WiFi router
        while (WiFi.status() != WL_CONNECTED) {   
        client.connect(server, 80);   // Connection to the server     
        pinMode(BUTON1,INPUT);
        pinMode(BUTON2,INPUT);
        pinMode(BUTON3,INPUT);
        }
        Serial.println("Connected");
}

void loop() {
  
  if(digitalRead(BUTON1)==1) // BUTON1 E BASILDIMI
            {
                if(buton_basim_sayisi == 0 && tampon[0] == '0' )  // başka butona basılmamış ise ve tampon boş ise
                    {
                        client.println(KULLANICI_1); // CLİENTE SOYLE KULLANICI 1 İN İSİNİ YAPSIN
                    }
                        else if(buton_basim_sayisi < 120) // Eger daha once bırı butona basmıs ve tampon bos degıl ıse ve tampon dolmamış ise.
                            {
                                buton_basim_sayisi++;  // tampon index arttır.
                                tampon[buton_basim_sayisi] = KULLANICI_1;  // işlemi tampona al.    
                            }                        
            }
            if(digitalRead(BUTON2)==1) // BUTON2 YE BASILDIMI
            {
                if(buton_basim_sayisi == 0 && tampon[0] == '0' )  // başka butona basılmamış ise ve tampon boş ise
                    {
                        client.println(KULLANICI_2); // CLİENTE SOYLE KULLANICI 2 NİN İSİNİ YAPSIN
                    }
                         else if(buton_basim_sayisi < 120)  // Eger daha once bırı butona basmıs ve tampon bos degıl ıse
                            {
                                buton_basim_sayisi++;  // tampon index arttır.
                                tampon[buton_basim_sayisi] = KULLANICI_2;  // işlemi tampona al.    
                            }      
            }
            
          if(digitalRead(BUTON3)==1) // BUTON2 YE BASILDIMI
            {
                if(buton_basim_sayisi == 0 && tampon[0] == '0' )  // başka butona basılmamış ise ve tampon boş ise
                    {
                        client.println(KULLANICI_3); // CLİENTE SOYLE KULLANICI 2 NİN İSİNİ YAPSIN
                    }
                         else if(buton_basim_sayisi < 120)  // Eger daha once bırı butona basmıs ve tampon bos degıl ıse
                            {
                                buton_basim_sayisi++;  // tampon index arttır.
                                tampon[buton_basim_sayisi] = KULLANICI_3;  // işlemi tampona al.    
                            }      
            }
           if(buton_basim_sayisi > 0) // eğer tamponda iş var ise.
                {
                    client.println(tampon[buton_basim_sayisi]); // tampona son girilen işi gönder.
                    buton_basim_sayisi--;  // ve tamponu 1 azalt ( 1 iş eksildiği için.)
                }
                
                if(buton_basim_sayisi < 0)
                    {
                        buton_basim_sayisi = 0;
                    }
                else if(buton_basim_sayisi > 120)
                    {
                        buton_basim_sayisi = 120;  // tamponu taşırma.
                    } 
       client.flush();
    }
