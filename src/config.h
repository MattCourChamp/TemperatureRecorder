#pragma once
#include <string>
using namespace std;
// WiFi credentials
char * SSID = "ChamplainPSK";
char * PASSWORD = "letusdare";

 // MQTT settings
const string ID = "MattCourChamp";
const string BROKER = "broker.hivemq.com";
const string CLIENT_NAME = ID + "weather_client";

const string CLIENT_TELEMETRY_TOPIC =  "champ/iotMatt";
