"""
Flask + MQTT prototype application.

Subscribes to MQTT topic for gas station utilization data (percentage busy)
and serves a simple HTTP endpoint showing the current busyness.
"""

from flask import Flask
import paho.mqtt.client as mqtt
import os
from dotenv import load_dotenv

# Shared value (updated by MQTT thread, read by Flask)
busy = 0

app = Flask(__name__)

# MQTT setup (connection config loaded from .env file)
load_dotenv()
MQTT_BROKER = os.getenv("MQTT_BROKER")
MQTT_PORT = int(os.getenv("MQTT_PORT"))
MQTT_TOPIC = os.getenv("MQTT_TOPIC")
MQTT_USER = os.getenv("MQTT_USER")
MQTT_PASS = os.getenv("MQTT_PASS")

# Create MQTT client using config values
client = mqtt.Client(client_id="", protocol=mqtt.MQTTv311)
client.username_pw_set(MQTT_USER, MQTT_PASS)

def on_connect(client, userdata, flags, rc):
  if rc == 0:
    client.subscribe(MQTT_TOPIC)

def on_disconnect(client, userdata, rc):
  print("MQTT disconnected, reconnecting...")
  try:
    client.reconnect()
  except Exception as e:
    print(f"Reconnect failed: {e}")

def on_message(client, userdata, msg):
  global busy
  busy = int(msg.payload)

client.on_connect = on_connect
client.on_disconnect = on_disconnect
client.on_message = on_message

@app.route("/")
def index():
  # Serves station busyness value as plaintext
  return 'The station is currently ' + str(busy) + '% busy'

if __name__ == "__main__":
  # Connect to MQTT broker and leave it as looping background thread
  try:
    client.connect(MQTT_BROKER, MQTT_PORT, 60)
    client.loop_start()
  except Exception as e:
      print(f"Failed to connect to MQTT broker: {e}")

  HOST = os.getenv("FLASK_HOST", "127.0.0.1")
  PORT = int(os.getenv("FLASK_PORT", 5000))

  app.run(debug=True, host=HOST, port=PORT)