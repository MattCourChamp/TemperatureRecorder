import json
import time
import paho.mqtt.client as mqtt

id = 'MattCourChamp'

client_telemetry_topic = 'champ/iotMatt'
server_command_topic = client_telemetry_topic
client_name = id + 'led'

mqtt_client = mqtt.Client(client_name)
mqtt_client.connect('broker.hivemq.com') #    test.mosquitto.org

mqtt_client.loop_start()
def handle_telemetry(client, userdata, message):
    payload = message.payload.decode()
    print("Message received:", payload)
    # command = 'TurnOFF'
    # print("Sending message:", command)
    # client.publish(server_command_topic, command)
    myFile = open("temperatures.txt", "w")
    myFile.write(payload)
    myFile.close()

mqtt_client.subscribe(client_telemetry_topic)
mqtt_client.on_message = handle_telemetry

while True:
    time.sleep(2)
