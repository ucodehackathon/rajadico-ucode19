//RST           D9
//SDA(SS)      D10
//MOSI         D11
//MISO         D12
//SCK          D13


#include <WiFi.h>
#include <WiFiMulti.h>
WiFiMulti WiFiMulti;
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_NeoPixel.h>

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN            13
// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      7
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int LOGO_HEIGHT = 0;
int LOGO_WIDTH = 0;

int value;
int field1 = 0;
int field2= 0;
int field3= 0;
int field4= 0;
int field5= 0;
int field6= 0;
int field7= 0;
int field8= 0;

const int buzzer = 33 ;
const int ledBoard = 2; 
const int RST_PIN = 36;            // Pin 9 para el reset del RC522
const int SS_PIN = 5;            // Pin 10 para el SS (SDA) del RC522
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Crear instancia del MFRC522
String ID="";
String IDuser1="042a2382c75581";
String IDuser2="04ed2382c75580";
String IDuser3="040c2382c75581";
String IDuser4="04ce2382c75580";
String IDuser5="04ec2382c75580";
int ScoreUser1=30;
int ScoreUser2=30;
int ScoreUser3=30;
int ScoreUser4=30;
int ScoreUser5=30;

const char* ssid     = "El_chico_WiFi"; // Your SSID (Name of your WiFi)
const char* password = "wifiparamiiPad"; //Your Wifi password
const char* host = "api.thingspeak.com";
String api_key = "OHSHRDCZT2UIAGYJ"; // Your API Key provied by thingspeak
void Connect_to_Wifi();
void Send_Data();
void testdrawbitmap(char dibujo);
void pitido(void);
void color(int r, int g, int b);


static const unsigned char PROGMEM logo_bmp[] =
{ //  69x64px
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x01, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x03, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xfe, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x07, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f,
  0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0x80, 0x00, 0x00, 0x00, 0x0e, 0x00,
  0x00, 0x0f, 0xff, 0x80, 0x00, 0x03, 0x80, 0x0f, 0xc0, 0x00, 0x1f, 0xff, 0xc0, 0x00, 0x3f, 0x80,
  0x0f, 0xf8, 0x00, 0x1f, 0xff, 0xc0, 0x00, 0xff, 0x80, 0x0f, 0xfe, 0x00, 0x1f, 0xff, 0xc0, 0x03,
  0xff, 0x80, 0x0f, 0xff, 0x00, 0x1f, 0xff, 0xc0, 0x07, 0xff, 0x80, 0x0f, 0xff, 0xc0, 0x1f, 0xff,
  0xc0, 0x1f, 0xff, 0x80, 0x0f, 0xff, 0xe0, 0x3f, 0xff, 0xc0, 0x3f, 0xff, 0x00, 0x07, 0xff, 0xf0,
  0x3f, 0xff, 0xc0, 0x7f, 0xff, 0x00, 0x07, 0xff, 0xf8, 0x3f, 0xff, 0xc0, 0xff, 0xff, 0x00, 0x07,
  0xff, 0xfc, 0x3f, 0xff, 0xc1, 0xff, 0xff, 0x00, 0x03, 0xff, 0xfe, 0x3f, 0xff, 0xc3, 0xff, 0xfe,
  0x00, 0x03, 0xff, 0xff, 0x3f, 0xff, 0xc7, 0xff, 0xfe, 0x00, 0x01, 0xff, 0xff, 0xbf, 0xff, 0xcf,
  0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xff,
  0xff, 0xff, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x3f, 0xfd, 0xfd, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xfd, 0xf9, 0xff, 0x80, 0x00,
  0x00, 0x00, 0x00, 0x07, 0xfc, 0xf9, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x71, 0xf8,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x61, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xcc, 0x00, 0x40, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0xec, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xec, 0x00, 0xe0, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x03, 0xfc, 0x3f, 0xec, 0x3f,
  0xe3, 0xfe, 0x3e, 0x00, 0x07, 0xfc, 0x7f, 0xec, 0x7f, 0xe7, 0xfe, 0xff, 0x00, 0x0f, 0x3c, 0xf3,
  0xec, 0xf3, 0xef, 0x3e, 0xe3, 0x80, 0x0c, 0x1c, 0xc1, 0xec, 0xe1, 0xee, 0x0e, 0xe0, 0x00, 0x1c,
  0x0d, 0xc0, 0xec, 0xc0, 0xec, 0x0e, 0xfe, 0x00, 0x1c, 0x0d, 0xc0, 0xec, 0xc0, 0xec, 0x0e, 0x7f,
  0x80, 0x1c, 0x0c, 0xc0, 0xec, 0xc0, 0xec, 0x0e, 0x07, 0x80, 0x0e, 0x1c, 0xe1, 0xec, 0xe1, 0xee,
  0x1e, 0xe3, 0x80, 0x0f, 0xfc, 0x7f, 0xec, 0x7f, 0xe7, 0xfe, 0xf7, 0x80, 0x07, 0xfc, 0x3f, 0xec,
  0x3f, 0xe3, 0xfe, 0x7f, 0x00, 0x00, 0xc0, 0x0c, 0x00, 0x0c, 0x00, 0xc0, 0x1c, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const unsigned char PROGMEM mini_logo_bmp[] =
{ // 'logo_pequenopequeno', 20x19px
  0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x60, 0x00, 0x00, 0xf0, 0x00, 0x60, 0xf0, 0x60, 0x78,
  0xf1, 0xe0, 0x7c, 0xf3, 0xe0, 0x3e, 0xf7, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xfe,
  0x00, 0x03, 0x0c, 0x00, 0x01, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x37, 0xbf, 0xc0,
  0x59, 0xdd, 0xe0, 0x7f, 0xff, 0x60, 0x00, 0x00, 0x00
};

static const unsigned char PROGMEM cora_bmp[] =
{ B00000000, B00000000,
  B00011100, B00111000,
  B00111110, B01111100,
  B01111111, B11111110,
  B11111111, B11111111,
  B11111111, B11111111,
  B11111111, B11111111,
  B11111111, B11111111,
  B11111111, B11111111,
  B01111111, B11111110,
  B00111111, B11111100,
  B00011111, B11111000,
  B00000111, B11100000,
  B00000011, B11000000,
  B00000001, B10000000,
  B00000000, B00000000
};
static const unsigned char PROGMEM cora_crush_bmp[] =
{ B00000000, B00000000,
  B00011100, B00111000,
  B00110110, B01111100,
  B01100001, B10000110,
  B11000000, B00000011,
  B11000000, B00000011,
  B11000000, B00000011,
  B11000000, B00000011,
  B11000000, B00000011,
  B01100000, B00000110,
  B00110000, B00001100,
  B00011000, B00011000,
  B00000110, B01100000,
  B00000011, B11000000,
  B00000001, B10000000,
  B00000000, B00000000
};

String byteToHex(byte *buffer,byte bufferSize) { // Guardar ID del TAG
    String ID = "";
    for (int i = 0; i < bufferSize; i++) {
        ID += String((buffer[i] & 0xff) + 0x100,16).substring(1);
    }
    return ID;
}

void setup()
{

  pixels.begin(); // This initializes the NeoPixel library.
  for(int i=0;i<NUMPIXELS;i++){
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(0,2,0)); // Moderately bright green color.
    pixels.show(); // This sends the updated pixel color to the hardware.
    delay(200); // Delay for a period of time (in milliseconds).
  }
  pixels.show();
  
   Serial.begin(9600);      //Inicializa la velocidad de Serial
   SPI.begin();         //Función que inicializa SPI
   mfrc522.PCD_Init();     //Función  que inicializa RFID
   
   pinMode(ledBoard, OUTPUT);
   digitalWrite(ledBoard, HIGH);  

   pinMode(buzzer, OUTPUT);
   digitalWrite(buzzer, HIGH);
   delay(50); 
   digitalWrite(buzzer, LOW); 

   Connect_to_Wifi();

   // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }
  // Clear the buffer
  display.clearDisplay();

  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  display.display();
  delay(2000);
  // display.display() is NOT necessary after every single drawing command,
  // unless that's what you want...rather, you can batch up a bunch of
  // drawing operations and then update the screen all at once by calling
  // display.display(). These examples demonstrate both approaches...

   //*****************************************************
  //AL ENCENDER MOSTRAMOS 7 SEGUNDOS EL LOGO DE ADIDAS
  
  testdrawbitmap(1);
  delay(2000);

  

  //AUTOMÁTICAMENTE DESPUÉS TENEMOS 3 VIDAS
  testdrawbitmap(2);
  delay(2000);

  /*

  //IF NOS QUITAN UNA VIDA
  testdrawbitmap(3); //DOS VIDAS
  delay(1000);

  //IF NOS QUITAN OTRA VIDA
  testdrawbitmap(4); //UNA VIDA
  delay(1000);

  //IF NOS QUITAN OTRA VIDA
  testdrawbitmap(5); //CERO VIDAS
  delay(1000);
  //TERMINAR PARTIDA
  
//********************************************************
*/
}

void loop()
{
  digitalWrite(buzzer, LOW); 
  // Detectar tarjeta
   if (mfrc522.PICC_IsNewCardPresent())
   {
      if (mfrc522.PICC_ReadCardSerial())
      {
         //Serial.print(F("Card UID:"));
         ID=byteToHex(mfrc522.uid.uidByte, mfrc522.uid.size);
         //Serial.print(ID);
         
         Serial.println("antes del if");
          if(not ID.equals(IDuser1)){
            if(ScoreUser1!=0){
              ScoreUser1-=10;
              //quitan vida
              pitido();
            }
             
            Serial.println("User 1 loses 10 points");
          }else{
            ScoreUser1=30;  
          }
          }/*else if(ID!=IDuser2){
            if(ScoreUser2!=0){
              ScoreUser2-=10;
            }
            Serial.println("User 2 loses 10 points");
            
          }else if(ID!=IDuser3){
            if(ScoreUser3!=0){
              ScoreUser3-=10;
            }
            Serial.println("User 3 loses 10 points");

          }else if(ID!=IDuser4){
            if(ScoreUser4!=0){
              ScoreUser4-=10;
            }
            Serial.println("User 4 loses 10 points");
            
          }else if(ID==IDuser5){
            if(ScoreUser1!=0){
              ScoreUser5-=10;
            }
            Serial.println("User 5 loses 10 points");
          }else{
            Serial.println("ID not recognized");
          }
          */
          Serial.println("Scores: ");
          Serial.print("User 1: ");
          Serial.println(ScoreUser1);
          Serial.print("User 2: ");
          Serial.println(ScoreUser2);
          Serial.print("User 3: ");
          Serial.println(ScoreUser3);
          Serial.print("User 4: ");
          Serial.println(ScoreUser4);
          Serial.print("User 5: ");
          Serial.println(ScoreUser5);  
         // Finalizar lectura actual

         switch(ScoreUser1){
          case 30:
            color(0, 2, 2);
            testdrawbitmap(2);
            //delay(1000);
            break;

          case 20:
            color(2, 1, 1);
            testdrawbitmap(3);
            //delay(1000);
            break;

          case 10:
            color(2, 2, 0);
            testdrawbitmap(4);
            //delay(1000);
            break;

          default:
            color(2, 0, 0);
            testdrawbitmap(5);
            delay(5000);
            testdrawbitmap(1);
            //delay(5000);
            ScoreUser1=30;
            break;
            
         }
         
         Send_Data(); // call function to send data to Thingspeak
         delay(1000);
         mfrc522.PICC_HaltA();
      }
  
      
      
}


void pitido(){
  
  digitalWrite(buzzer, HIGH); 
  delay(200);
  digitalWrite(buzzer, LOW);
  delay(100);
  digitalWrite(buzzer, HIGH); 
  delay(200);
  digitalWrite(buzzer, LOW);
  delay(100);
  digitalWrite(buzzer, HIGH); 
  delay(200);
  digitalWrite(buzzer, LOW);
  delay(100);
  
  }

void color(int r, int g, int b){
    for(int i=0;i<NUMPIXELS;i++){   pixels.setPixelColor(i, pixels.Color(r,g,b)); // Moderately bright green color.
    pixels.show(); // This sends the updated pixel color to the hardware.
    delay(100); // Delay for a period of time (in milliseconds).
  }
}


void Connect_to_Wifi()
{

  // We start by connecting to a WiFi network
  WiFiMulti.addAP(ssid, password);

  Serial.println();
  Serial.println();
  Serial.print("Wait for WiFi... ");

  while (WiFiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

}



void Send_Data()
{

  // map the moist to 0 and 100% for a nice overview in thingspeak.
  
  value = constrain(value,0,5000);
  value = map(value,0,5000,100,0);

  Serial.println("Prepare to send data");

  // Use WiFiClient class to create TCP connections
  WiFiClient client;

  const int httpPort = 80;

  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  else
  {
    String data_to_send = api_key;
    data_to_send += "&field1=";
    data_to_send += String(ScoreUser1);
    data_to_send += "&field2=";
    data_to_send += String(ScoreUser2);
    data_to_send += "&field3=";
    data_to_send += String(ScoreUser3);
    data_to_send += "&field4=";
    data_to_send += String(ScoreUser4);
    data_to_send += "&field5=";
    data_to_send += String(ScoreUser5);
    data_to_send += "&field6=";
    data_to_send += String(field6);
    data_to_send += "&field7=";
    data_to_send += String(field7);
    data_to_send += "&field8=";
    data_to_send += String(field8);
    data_to_send += "\r\n\r\n";

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + api_key + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(data_to_send.length());
    client.print("\n\n");
    client.print(data_to_send);

    delay(1000);
  }

  client.stop();

}

void testdrawbitmap(char dibujo) {
  display.clearDisplay();

  switch (dibujo) {
    case 1: //LOGO ADIDAS
      display.clearDisplay();
      LOGO_WIDTH = 69;
      LOGO_HEIGHT = 64;
      display.drawBitmap(
        (display.width()  - LOGO_WIDTH ) / 2,
        (display.height() - LOGO_HEIGHT) / 2,
        logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
      display.display();
      break;

    case 2: //Tres vidas
      //loguito adidas
      LOGO_WIDTH = 20;
      LOGO_HEIGHT = 19;
      display.drawBitmap(
        102,
        4,
        mini_logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
      display.display();

      LOGO_WIDTH = 16;
      LOGO_HEIGHT = 16;
      display.drawBitmap(
        20,
        30,
        cora_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
      display.display();
      display.drawBitmap(
        55,
        30,
        cora_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
      display.display();
      display.drawBitmap(
        90,
        30,
        cora_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
      display.display();
      break;

    case 3: //DOS VIDAS
      //loguito adidas
      LOGO_WIDTH = 20;
      LOGO_HEIGHT = 19;
      display.drawBitmap(
        102,
        4,
        mini_logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
      display.display();

      LOGO_WIDTH = 16;
      LOGO_HEIGHT = 16;
      display.drawBitmap(
        20,
        30,
        cora_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
      display.display();
      display.drawBitmap(
        55,
        30,
        cora_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
      display.display();
      display.drawBitmap(
        90,
        30,
        cora_crush_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
      display.display();
      break;

    case 4: //UNA VIDA
      //loguito adidas
      LOGO_WIDTH = 20;
      LOGO_HEIGHT = 19;
      display.drawBitmap(
        102,
        4,
        mini_logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
      display.display();

      LOGO_WIDTH = 16;
      LOGO_HEIGHT = 16;
      display.drawBitmap(
        20,
        30,
        cora_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
      display.display();
      display.drawBitmap(
        55,
        30,
        cora_crush_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
      display.display();
      display.drawBitmap(
        90,
        30,
        cora_crush_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
      display.display();
      break;

    case 5: //UNA VIDA
      //loguito adidas
      LOGO_WIDTH = 20;
      LOGO_HEIGHT = 19;
      display.drawBitmap(
        102,
        4,
        mini_logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
      display.display();

      LOGO_WIDTH = 16;
      LOGO_HEIGHT = 16;
      display.drawBitmap(
        20,
        30,
        cora_crush_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
      display.display();
      display.drawBitmap(
        55,
        30,
        cora_crush_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
      display.display();
      display.drawBitmap(
        90,
        30,
        cora_crush_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
      display.display();
      break;

    default:
      // statements
      break;
  }
}




