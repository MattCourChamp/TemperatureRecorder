#include <Arduino.h>
#include <WiFiNINA.h>
#include <PubSubClient.h>
//#include "thingProperties.h"
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

    if (message == "TurnON"){
        digitalWrite(LED_BUILTIN, HIGH);
    }
    else
        digitalWrite(LED_BUILTIN, LOW);
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
    ; // Wait for Serial to be ready

    float temperature;
    ArduinoCloud.addProperty(temperature, READ, 1 * SECONDS, NULL);

    delay(500);
    CARRIER_CASE = false;
    carrier.begin();
    carrier.display.setRotation(0);
    delay(1500);

    pinMode(LED_BUILTIN, OUTPUT);
    connectWiFi();
    createMQTTClient();
}

//int period = 3000;
unsigned long time_now = 0;
#define INTERVAL_MESSAGE 10000

void loop() {
    //Serial.println("waiting...");
    if(millis() > time_now + INTERVAL_MESSAGE){
        time_now = millis();
        int temperature = carrier.Env.readTemperature();
        reconnectMQTTClient();
        client.loop();
        Serial.println("Sending telemetry messages");
        //string telemetry = "TurnON"; 
        string telemetry = to_string(temperature);  
        client.publish(CLIENT_TELEMETRY_TOPIC.c_str(), telemetry.c_str());
        //delay(10000);
    }
    //Serial.println("wait over");
}