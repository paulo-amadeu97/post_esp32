#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <MFRC522.h>
#include <WebServer.h>
#include <ArduinoJson.h>

//WIFI
const char *ssid = "REDE_TEST";
const char *password = "robotica01";
bool wifiStatus = false;
IPAddress local_IP(192, 168, 0, 30);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);

//DHT
#define DHTPIN 26
#define DHTTYPE DHT11


//OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SCREEN_ADDRESS 0x3C

//RFID
#define SDA_PIN 5
#define RST_PIN 2

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);
DHT dht(DHTPIN, DHTTYPE);
MFRC522 rfid(SDA_PIN, RST_PIN);

//LOGO DE ADS
const unsigned char adsLogo[] PROGMEM = {
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xdf, 0xfc, 0x00, 0x00, 0x3f, 0xfb, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xfe, 0x3f, 0xf0, 0x00, 0x00, 0x0f, 0xfc, 0x7f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0x78, 0x7f, 0xc0, 0x00, 0x00, 0x03, 0xfe, 0x1e, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xfe, 0x70, 0xff, 0x80, 0x00, 0x00, 0x01, 0xff, 0x0e, 0x7f, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xfe, 0x73, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x8e, 0x7f, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xfc, 0x6f, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xe6, 0x3f, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xfc, 0xff, 0xf8, 0x07, 0xfe, 0x00, 0x00, 0x1f, 0xfe, 0x3f, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xec, 0xff, 0xf0, 0x1e, 0x4f, 0x80, 0x00, 0x0f, 0xff, 0x37, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xec, 0xe1, 0xe0, 0x6c, 0x44, 0xc0, 0x00, 0x07, 0x85, 0x77, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xce, 0x43, 0xc0, 0xd8, 0x42, 0x30, 0x00, 0x07, 0xc3, 0x73, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xce, 0x0f, 0xc1, 0x10, 0x43, 0x18, 0x00, 0x03, 0xf0, 0x73, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xc6, 0xff, 0x82, 0x30, 0x41, 0x0c, 0x00, 0x01, 0xff, 0x63, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xc7, 0xff, 0x86, 0x20, 0x41, 0x84, 0x00, 0x01, 0xff, 0xe3, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xfe, 0xe7, 0x8f, 0x0f, 0xff, 0xff, 0xfe, 0x00, 0x00, 0xf1, 0xe7, 0x7f, 0xff, 0xff, 
	0xff, 0xff, 0xfe, 0xe6, 0x1f, 0x0c, 0x60, 0xc0, 0xc3, 0x00, 0x00, 0xf8, 0x67, 0x7f, 0xff, 0xff, 
	0xff, 0xff, 0xfe, 0x78, 0x3e, 0x08, 0x40, 0x40, 0x41, 0x00, 0x00, 0x7c, 0x1e, 0x7f, 0xff, 0xff, 
	0xff, 0xff, 0xfe, 0x7f, 0xfe, 0x18, 0x40, 0x40, 0x41, 0x00, 0x00, 0x7f, 0xde, 0x7f, 0xff, 0xff, 
	0xff, 0xff, 0xfe, 0x37, 0xfc, 0x10, 0xc0, 0x40, 0x41, 0x80, 0x00, 0x7f, 0xfc, 0x7f, 0xff, 0xff, 
	0xff, 0xff, 0xfe, 0x37, 0x3c, 0x10, 0xc0, 0x40, 0x41, 0x80, 0x00, 0x3c, 0xec, 0x7f, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0x3c, 0x7c, 0x10, 0x80, 0x40, 0x40, 0x80, 0x00, 0x3e, 0x2c, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xb8, 0x7c, 0x1f, 0xff, 0xff, 0xff, 0x80, 0x00, 0x3e, 0x1d, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xfb, 0xf0, 0xfc, 0x10, 0xc0, 0x40, 0x44, 0x80, 0x00, 0x3f, 0x0b, 0xdf, 0xff, 0xff, 
	0xff, 0xff, 0xf9, 0xef, 0xf8, 0x10, 0xc0, 0x40, 0x7f, 0x80, 0x00, 0x3f, 0xf7, 0x9f, 0xff, 0xff, 
	0xff, 0xff, 0xf8, 0xef, 0x78, 0x10, 0xc0, 0x40, 0x40, 0x80, 0x00, 0x3e, 0xf7, 0x1f, 0xff, 0xff, 
	0xff, 0xff, 0xfc, 0xee, 0x78, 0x10, 0x40, 0x40, 0x80, 0x40, 0x00, 0x1e, 0x76, 0x3f, 0xff, 0xff, 
	0xff, 0xff, 0xfc, 0x7c, 0x78, 0x18, 0x40, 0x41, 0x80, 0x20, 0x00, 0x1e, 0x36, 0x3f, 0xff, 0xff, 
	0xff, 0xff, 0xfe, 0x38, 0xf8, 0x08, 0x40, 0x41, 0x00, 0x20, 0x00, 0x1f, 0x14, 0x7f, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0x31, 0xf8, 0x0f, 0xff, 0xff, 0x00, 0x20, 0x00, 0x3f, 0x84, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xf7, 0xe7, 0xf8, 0x07, 0xff, 0xff, 0x00, 0x20, 0x00, 0x3f, 0xe7, 0xef, 0xff, 0xff, 
	0xff, 0xff, 0xf3, 0xef, 0x7c, 0x06, 0x30, 0x41, 0x00, 0x20, 0x00, 0x3e, 0xf7, 0xcf, 0xff, 0xff, 
	0xff, 0xff, 0xf8, 0xee, 0x7c, 0x03, 0x10, 0x43, 0x80, 0x20, 0x00, 0x3e, 0x7f, 0x1f, 0xff, 0xff, 
	0xff, 0xff, 0xf8, 0x7c, 0x7c, 0x01, 0x98, 0x42, 0x80, 0x40, 0x00, 0x3e, 0x3e, 0x3f, 0xff, 0xff, 
	0xff, 0xff, 0xfc, 0x3c, 0x7c, 0x00, 0xcc, 0x46, 0x61, 0xe0, 0x00, 0x3e, 0x3c, 0x3f, 0xff, 0xff, 
	0xff, 0xff, 0xfe, 0x3c, 0xfe, 0x00, 0x34, 0x4d, 0xff, 0x70, 0x00, 0x7f, 0x3c, 0x7f, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0x95, 0xde, 0x00, 0x1f, 0x5f, 0x00, 0x38, 0x00, 0x7b, 0xa9, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xf7, 0x9e, 0x00, 0x03, 0xf8, 0x00, 0x1c, 0x00, 0x79, 0xef, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xf9, 0xff, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x00, 0xf8, 0xff, 0x9f, 0xff, 0xff, 
	0xff, 0xff, 0xfc, 0x7b, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0xf8, 0xfe, 0x3f, 0xff, 0xff, 
	0xff, 0xff, 0xfe, 0x1f, 0x3f, 0x80, 0x00, 0x00, 0x00, 0x03, 0x81, 0xf8, 0xf8, 0x7f, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0x0f, 0x37, 0x80, 0x00, 0x00, 0x00, 0x01, 0xc1, 0xec, 0xf0, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xc7, 0x77, 0xc0, 0x00, 0x00, 0x00, 0x00, 0xe3, 0xce, 0xe3, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xfe, 0xe3, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x47, 0xc7, 0x7f, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xe3, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xc7, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0x1f, 0x62, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x46, 0xf8, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0x83, 0xe6, 0x78, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x67, 0xc1, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xe0, 0xee, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x77, 0x07, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xf8, 0x1e, 0x7f, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x78, 0x1f, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xee, 0x7f, 0x80, 0x00, 0x00, 0x01, 0xfe, 0x7f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xfe, 0x67, 0xc0, 0x00, 0x00, 0x07, 0xe6, 0x7f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xf0, 0x1a, 0xe7, 0xf0, 0x00, 0x00, 0x0f, 0xe7, 0x78, 0x0f, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xfc, 0x04, 0xe3, 0xfe, 0x00, 0x00, 0x7f, 0xc7, 0x20, 0x3f, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xe3, 0xff, 0xc0, 0x07, 0xff, 0xc7, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xf3, 0xff, 0xff, 0xff, 0xff, 0xcf, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xe7, 0xff, 0xff, 0xff, 0xff, 0xe7, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xf0, 0x03, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x0f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xc0, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x07, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};
//PORTA E BUZZER
#define DOOR 25
#define DOOR_CLOSE 33
#define BOTON 4
#define BUZZER_PIN 32

//URLs DA API
const char* serverUrlStData = "http://192.168.0.25:8080/statedata/";
const char* serverUrlAccessData = "http://192.168.0.25:8080/access/";
const char* postUrl = "http://192.168.0.25:8080/auth/register/rfid/esp/";

//VARIÁVEIS DE CONTROLE
unsigned long previousMillis = 0;
unsigned long previousMillisRfid = 0;
unsigned long daniedTime = 0;
unsigned long doorOpenTime = 0;
const long interval = 30000; 
const long intervalRfid = 50;
bool doorOpen = false;
bool printDanied = false;

//FUNÇÕES DE INICIALIZAÇÃO
void initDisplay();
void handleButtonPress();
void checkDoorTimeout();
void openDoor();
void closeDoor();
void monPrinDanied();
void initWiFi();
void sendSensorData();
void reconnectWiFiIfNeeded();

WebServer server(80);

void setup() {
  Serial.begin(9600);
  pinMode(DOOR, OUTPUT);
  pinMode(DOOR_CLOSE, OUTPUT);
  pinMode(BOTON, INPUT);
  digitalWrite(DOOR, HIGH);
  pinMode(BUZZER_PIN, OUTPUT);

  initDisplay();
  dht.begin();
  initWiFi();
  setupServer();

  SPI.begin();
  rfid.PCD_Init();
}

void loop() {
  handleButtonPress();
  monPrinDanied();
  checkDoorTimeout();
  sendSensorData();
  reconnectWiFiIfNeeded();
  monRfid();
  processRequest();
}

//INICIALIZAÇÃO DO OLED
void initDisplay() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println("Falha ao inicializar a tela OLED");
    while (1);
  }
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.drawBitmap(0, 0, adsLogo, SCREEN_WIDTH, SCREEN_HEIGHT, SSD1306_WHITE);
  display.display();
  delay(1000);
  printData();
}

//CONECTAR WIFI
void initWiFi() {
  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("Falha ao configurar IP estático.");
  }

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    handleButtonPress();
    checkDoorTimeout();
    printData();
    delay(1000);
    Serial.println("Conectando ao WiFi...");
  }
  Serial.println("WiFi conectado!");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());
  wifiStatus = true;
  printData();
}


void beepOk() {
  tone(BUZZER_PIN, 1000, 300);
}


void beepErro(){
  tone(BUZZER_PIN, 100, 1000);
}


void handleButtonPress() {
  if (digitalRead(BOTON) == HIGH && !doorOpen) {
    openDoor();
    printData();
    beepOk();
  }
}


void checkDoorTimeout() {
  if (doorOpen && millis() - doorOpenTime >= 5000) {
    closeDoor();
    printData();
  }
}

bool getDoorStatus(){
  if(digitalRead(DOOR) == HIGH){
    return false;
  }
  return true;
}

void displayData(float temperature, float humidity, float tmpOrv){
  String wifi = "";
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.print("Temp: ");
  display.print(temperature);
  display.println(" C");
  display.print("Umi: ");
  display.print(humidity);
  display.println(" %");
  display.print("Temp_Orv:");
  display.print(tmpOrv);
  display.println(" C");
  display.println();
  display.setTextSize(2);
  if(getDoorStatus()){
    display.println("ABERTO");
  }else{
    display.println("FECHADO");
  }
  if(wifiStatus){
    wifi = "conectado!";
  } else {
    wifi = "desconectado!";
  }
  display.setTextSize(1);
  display.println();
  display.println(wifi);
  display.display();
}

void printData(){
  if(printDanied){
    return;
  }
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  float tmpOrv = (237.7 * (((17.27 * temperature) / (237.7 + temperature)) + 
                  log(humidity / 100))) / (17.27 - (((17.27 * temperature) / 
                  (237.7 + temperature)) + log(humidity / 100)));

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Falha ao ler do sensor DHT!");
    return;
  }
  displayData(temperature, humidity, tmpOrv);
}

void sendSensorData() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();
    float tmpOrv = (237.7 * (((17.27 * temperature) / (237.7 + temperature)) + 
                  log(humidity / 100))) / (17.27 - (((17.27 * temperature) / 
                  (237.7 + temperature)) + log(humidity / 100)));

    if (isnan(temperature) || isnan(humidity)) {
      Serial.println("Falha ao ler do sensor DHT!");
      return;
    }

    displayData(temperature, humidity, tmpOrv);

    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      http.begin(serverUrlStData);
      http.addHeader("Content-Type", "application/json");

      String jsonData = "{\"temperature\": " + String(temperature) + ", \"humidity\": " + String(humidity) + "}";
      int httpResponseCode = http.POST(jsonData);

      Serial.println(jsonData);
      if (httpResponseCode > 0) {
        String response = http.getString();
        Serial.println("Resposta do servidor: " + response);
      } else {
        Serial.println("Erro na requisição POST: " + String(httpResponseCode));
      }
      http.end();
    } else {
      Serial.println("Erro de conexão WiFi");
      wifiStatus = false;
    }
  }
}

bool sendLogData(String url) {
  if (WiFi.status() == WL_CONNECTED) {
      String rfid = readRfid();
      HTTPClient http;
      http.begin(url);
      http.addHeader("Content-Type", "application/json");

      String jsonData = "{\"rfid\": \"" + rfid + "\"}";
      int httpResponseCode = http.POST(jsonData);

      Serial.println(jsonData);
      if (httpResponseCode > 0) {
        String response = http.getString();
        Serial.println("Resposta do servidor: " + response);

        return (httpResponseCode >= 200 && httpResponseCode < 300);
      } else {
        Serial.println("Erro na requisição POST: " + String(httpResponseCode));
        return false;
      }
      http.end();
  } else {
    Serial.println("Erro de conexão WiFi");
    wifiStatus = false;
    return false;
  }
}

void openDoor() {
  Serial.println("Porta aberta");
  digitalWrite(DOOR, LOW);
  digitalWrite(DOOR_CLOSE, HIGH);
  doorOpen = true;
  doorOpenTime = millis();
}

void closeDoor() {
  Serial.println("Porta fechada");
  digitalWrite(DOOR, HIGH);
  digitalWrite(DOOR_CLOSE, LOW);
  doorOpen = false;
}

void reconnectWiFiIfNeeded() {
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.status();
    Serial.println("Desconectado do WiFi, tentando reconectar...");
    WiFi.disconnect();
    wifiStatus = false;
    initWiFi();
  }
}

String readRfid(){
  String rfidData = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    if (rfid.uid.uidByte[i] < 0x10) {
      rfidData += "0";
    }
    rfidData += String(rfid.uid.uidByte[i], HEX);
  }
  rfid.PICC_HaltA();
  return rfidData;
}

void monRfid(){
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillisRfid >= intervalRfid) {
    previousMillisRfid = currentMillis;

    if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    return;
    }
    if(sendLogData(serverUrlAccessData)){
      openDoor();
      printData();
      beepOk();
    } else {
      float temperature = dht.readTemperature();
      float humidity = dht.readHumidity();
      float tmpOrv = (237.7 * (((17.27 * temperature) / (237.7 + temperature)) + 
                    log(humidity / 100))) / (17.27 - (((17.27 * temperature) / 
                    (237.7 + temperature)) + log(humidity / 100)));

      if (isnan(temperature) || isnan(humidity)) {
        Serial.println("Falha ao ler do sensor DHT!");
        return;
      }
      daniedTime = millis();
      printDanied = true;
      beepErro();
      String wifi = "";
      display.clearDisplay();
      display.setCursor(0, 0);
      display.setTextSize(1);
      display.print("Temp: ");
      display.print(temperature);
      display.println(" C");
      display.print("Umi: ");
      display.print(humidity);
      display.println(" %");
      display.print("Temp_Orv:");
      display.print(tmpOrv);
      display.println(" C");
      display.println();
      display.setTextSize(2);
      display.println("NEGADO!");
      if(wifiStatus){
        wifi = "conectado!";
      } else {
        wifi = "desconectado!";
      }
      display.setTextSize(1);
      display.println();
      display.println(wifi);
      display.display();
    }
  }
}

void monPrinDanied(){
  if (printDanied && millis() - daniedTime >= 5000) {
    printDanied = false;
    printData();
  }
}

void setupServer() {
  server.on("/email/", HTTP_POST, processRequest);
  server.begin();
  Serial.println("Servidor iniciado e aguardando requisições POST em /email/");
}

void processRequest() {
  server.handleClient();
  if (server.hasArg("plain")) {
    String json = server.arg("plain");
    Serial.println("JSON recebido: " + json);

    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, json);

    if (!error) {
      String email = doc["email"];
      Serial.println("Email recebido: " + email);
      server.send(200, "text/plain", "OK");

      display.clearDisplay();
      display.setCursor(0, 0);
      display.setTextSize(2);
      display.println("APROXIME");
      display.println("A TAG");
      display.display();

      unsigned long previousMillis = millis();
      String rfidTag = "";
      bool cardDetected = false;

      while ((millis() - previousMillis <= 30000) && !cardDetected) {
        if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
          rfidTag = readRfid();
          sendEmailAndRfid(email, rfidTag);
          cardDetected = true;
          beepOk();
        }
      }

      if (!cardDetected) {
        display.clearDisplay();
        display.setCursor(0, 0);
        display.setTextSize(2);
        display.println("NENHUM CARTAO");
        display.println("DETECTADO!");
        display.display();
        Serial.println("Nenhum cartão detectado no tempo limite.");
        beepErro();
        delay(2000);
      }
      ESP.restart();
    } else {
      Serial.println("Erro JSON: " + String(error.c_str()));
    }
  }
}

void sendEmailAndRfid(String email, String rfid) {
  if(WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(postUrl);
    http.addHeader("Content-Type", "application/json");

    String jsonPayload = "{\"email\": \"" + email + "\", \"rfid\": \"" + rfid + "\"}";

    Serial.println(jsonPayload);

    int httpResponseCode = http.POST(jsonPayload);
    
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("Resposta do servidor: " + response);
    } else {
      Serial.println("Erro ao enviar POST: " + String(httpResponseCode));
    }
    http.end();
  } else {
    Serial.println("Erro: Não conectado ao WiFi");
    wifiStatus = false;
  }
}