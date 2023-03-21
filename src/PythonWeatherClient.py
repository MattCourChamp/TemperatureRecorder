import json
import time
import paho.mqtt.client as mqtt
from datetime import datetime

id = 'MattCourChamp'

client_telemetry_topic = 'champ/iotMatt'
server_command_topic = client_telemetry_topic
client_name = id + 'led'

mqtt_client = mqtt.Client(client_name)
mqtt_client.connect('broker.hivemq.com') #    test.mosquitto.org

mqtt_client.loop_start()
# recieves temperature, saves it in a txt file
def handle_telemetry(client, userdata, message):
    payload = message.payload.decode()
    print("Message received:", payload)
    myFile = open("weatherReports.txt", "a")
    myFile.write(str(datetime.now().strftime("%m/%d/%y %H:%M:%S")) + " - " + payload + "\n")
    myFile.close()
    time.sleep(10)

mqtt_client.subscribe(client_telemetry_topic)
mqtt_client.on_message = handle_telemetry

while True:
    time.sleep(2)
