#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

const char* ssid = "Scrambler"; //wifi ssid / wifi name
const char* password = "Multistrada123"; //wifi password

//Your Domain name with URL path or IP address with path
String serverName = "http://192.168.137.254/loadcell/postdata.php?weight=";
unsigned long lastTime = 0;
unsigned long timerDelay = 5000;
const unsigned int MAX_MESSAGE_LENGTH = 12;
String text;
float angka;


void setup() {
  Serial.begin(9600); 
  Serial.println("Pastikan Tidak Ada Beban");
  delay(1000);
  Serial.println("Silahkan Untuk Menimbang");
 
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
 
  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");
}

void loop() {
  // Send an HTTP POST request depending on timerDelay
 if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      WiFiClient client;
      HTTPClient http;
      BacaTimbangan();
      Serial.println(angka);
      String serverPath = serverName + String (angka);      
      // Your Domain name with URL path or IP address with path
      http.begin(client, serverPath.c_str());
      
      // Send HTTP GET request
      int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
        Serial.println(serverPath);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
       //Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}


void BacaTimbangan(){
  //periksa apakah serial tersedia
 while (Serial.available() > 0)
 {
   //buat variable untuk menyimpan pesan dari serial
   static char message[MAX_MESSAGE_LENGTH];
   static unsigned int message_pos = 0;

   //baca setiap byte dari serial
   char inByte = Serial.read();

   //setelah pesan masuk/terbaca(periksa adakah karakter 'enter' dan cek ukuran byte pesan)
   if ( inByte != '\n' && (message_pos < MAX_MESSAGE_LENGTH - 1) )
   {
     //tambahkan setiap byte variable pesan
     message[message_pos] = inByte;
     message_pos++;
   }
   //setelah byte terpenuhi (12 byte)
   else
   {
     //tampilkan pesan (or do other things)
     text = message;
     text = text.substring(3,8);
     angka = text.toFloat();
//     Serial.println(angka);

     //Reset for the next message
     message_pos = 0;
   }
 }
}





