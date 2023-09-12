#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <Keypad.h>

const char* ssid = "Arlo"; //wifi ssid / wifi name
const char* password = "Jt170818"; //wifi password

//Your Domain name with URL path or IP address with path
String serverName = "http://192.168.43.198/loadcell/postdata.php?weight=";
unsigned long lastTime = 0;
unsigned long timerDelay = 1000;
const unsigned int MAX_MESSAGE_LENGTH = 12;
String text;
float angka;
//keypad
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','4','7','*'},
  {'2','5','8','0'},
  {'3','6','9','#'},
  {'A','B','C','D'}
};
byte rowPins[ROWS] = {D3, D2, D1, D0}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {D7, D6, D5, D4}; //connect to the column pinouts of the keypad
//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

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
      //BacaTimbangan();
      inputangka();
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

     //Reset for the next message
     message_pos = 0;
   }
 }
}

void inputangka(){
  char customKey = customKeypad.getKey();
  
  if (customKey){
    angka=customKey;
    Serial.println(angka);
  }
}
