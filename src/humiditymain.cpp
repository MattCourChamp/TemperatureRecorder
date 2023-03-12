#include <Arduino.h>
#include <WiFiNINA.h>
#include <PubSubClient.h>
#include <Arduino_MKRIoTCarrier.h>
#include <ArduinoIoTCloud.h>
#include <SNU.h>
MKRIoTCarrier carrier;

#include "config.h"

void connectWiFi()
{
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("Connecting to WiFi..");
        WiFi.begin(SSID, PASSWORD);
        delay(500);
    }

    Serial.println("Connected!");
}

WiFiClient wifiClient;
PubSubClient client(wifiClient);

void clientCallback(char *topic, uint8_t *payload, unsigned int length)
{
    char buff[length + 1];
    for (int i = 0; i < length; i++)
    {
        buff[i] = (char)payload[i];
    }
    buff[length] = '\0';

    Serial.print("Message received:");
    Serial.println(buff);
    String message = buff;
    Serial.println(message);
}

void reconnectMQTTClient()
{
    while (!client.connected())
    {
        Serial.print("Attempting MQTT connection...");

        if (client.connect(CLIENT_NAME.c_str()))
        {
            Serial.println("connected");
            client.subscribe(CLIENT_TELEMETRY_TOPIC.c_str()); // SERVER_COMMAND_TOPIC
        }
        else
        {
            Serial.print("Retying in 5 seconds - failed, rc=");
            Serial.println(client.state());
            
            delay(5000);
        }
    }
}

void createMQTTClient()
{
    client.setServer(BROKER.c_str(), 1883);
    client.setCallback(clientCallback);
    reconnectMQTTClient();
}

void setup() {
    Serial.begin(9600);
    while (!Serial)
    ;

    float humidity;
    ArduinoCloud.addProperty(humidity, READ, 1 * SECONDS, NULL);

    delay(500);
    CARRIER_CASE = false;
    carrier.begin();
    carrier.display.setRotation(0);
    delay(1500);
    connectWiFi();
    createMQTTClient();
}

//int period = 3000;
unsigned long time_now = 0;
#define INTERVAL_MESSAGE 10000

void loop() {
   
    if(millis() > time_now + INTERVAL_MESSAGE){
        time_now = millis();
        int humidity = carrier.Env.readHumidity();
        reconnectMQTTClient();
        client.loop();
        Serial.println("Sending telemetry messages");
        
        string telemetry = to_string(humidity);  
        client.publish(CLIENT_TELEMETRY_TOPIC.c_str(), telemetry.c_str());
        
    }
  
}
