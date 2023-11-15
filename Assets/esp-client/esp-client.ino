#include <WiFi.h>
#include "MPU9250.h"

// Configurações de Wi-Fi
const char* ssid = "Espada";
const char* password = "12345678";

// Configurações do servidor TCP
const char* host = "192.168.0.101";
const uint16_t port = 8052;

MPU9250 mpu;
unsigned long interval;
unsigned long previousMillis = 0;
WiFiClient client;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  delay(2000);

  // Inicializa o MPU9250
  if (!mpu.setup(0x68)) {
    Serial.println("MPU connection failed.");
    while (1)
      ;
  }

  // Conecta ao Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");

  // Mostra o IP atribuído ao dispositivo
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  previousMillis = millis();
}

void loop() {
  unsigned long currentMillis = millis();

  if (mpu.update()) {
    if (client.connect(host, port)) {
      client.printf("%lu, %.2f, %.2f, %.2f\n", currentMillis, mpu.getYaw(), mpu.getPitch(), mpu.getRoll());
      client.stop();
    } else {
      // Mostra um erro caso a conexão falhe
      Serial.print("Failed to connect to server. Error code: ");
      // Serial.println(client.lastError());
    }
  }
}
