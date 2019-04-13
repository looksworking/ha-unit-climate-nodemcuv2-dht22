#include <Arduino.h>
#include <ArduinoJson.h>
#include <DHTesp.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>
#include "LocalVariables.h"

DHTesp dht;
WiFiUDP udpClient;
char jsonChar[200];

void udpSend();
void getJson(const char *status, float humidity, float temperature);

void setup() {
    Serial.begin(115200);
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    dht.setup(D2, DHTesp::DHT22);
}

void loop() {
    if ((WiFi.status() == WL_CONNECTED)) {
        delay(dht.getMinimumSamplingPeriod());
        getJson(dht.getStatusString(), dht.getHumidity(), dht.getTemperature());
        udpSend();
        delay(sleep);
    }
}

void udpSend() {
    Serial.println("Sending ");
    udpClient.beginPacket(host, port);
    udpClient.write(jsonChar);
    udpClient.endPacket();
    Serial.println("Sent");
}

void getJson(const char *status, float humidity, float temperature) {
    StaticJsonDocument<200> doc;
    doc["unit"] = id;
    doc["status"] = status;
    doc["humidity"] = humidity;
    doc["temperature"] = temperature;
    serializeJson(doc, jsonChar);
    Serial.println("JSON data: ");
    Serial.println(jsonChar);
}