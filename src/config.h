#pragma once
#include <string>
using namespace std;
// WiFi credentials
const char * SSID = "ChamplainPSK";
const char * PASSWORD = "letusdare";

//const char * SSID = "GungorGuest";
//const char * PASSWORD = "welcomehome";

 // MQTT settings
const string ID = "MattCourChamp"; // Put something different
const string BROKER = "broker.hivemq.com";
const string CLIENT_NAME = ID + "remotelight_client";

const string CLIENT_TELEMETRY_TOPIC =  "champ/iotMatt";
