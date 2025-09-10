"""
Flask + MQTT prototype application.

Subscribes to MQTT topic for gas station utilization data (percentage busy)
and serves a simple HTTP endpoint showing the current busyness.
"""

from flask import Flask, render_template_string
import paho.mqtt.client as mqtt
import os
from dotenv import load_dotenv

from datetime import datetime
import pytz

import pandas as pd
import plotly.express as px
import numpy as np

# Shared value (updated by MQTT thread, read by Flask)
data = {(day, hour): 0.0 for day in range(7) for hour in range(24)}
alpha = 0.3

app = Flask(__name__)


# MQTT setup (connection config loaded from .env file)
load_dotenv()
MQTT_BROKER = os.getenv("MQTT_BROKER")
MQTT_PORT = int(os.getenv("MQTT_PORT"))
MQTT_TOPIC = os.getenv("MQTT_TOPIC")
MQTT_USER = os.getenv("MQTT_USER")
MQTT_PASS = os.getenv("MQTT_PASS")

TIMEZONE = os.getenv("TIMEZONE", "UTC")

# Create MQTT client using config values
client = mqtt.Client(client_id="", protocol=mqtt.MQTTv311)
client.username_pw_set(MQTT_USER, MQTT_PASS)

def on_connect(client, userdata, flags, rc):
  if rc == 0:
    client.subscribe(MQTT_TOPIC)

def on_message(client, userdata, msg):
  try:
    currentUtilization = int(msg.payload)
    print("Parsed:", currentUtilization)
  except ValueError:
    print("Bad payload:", msg.payload)
    return

  # uses server time to target correct day/hour
  now = datetime.now(pytz.timezone(TIMEZONE))
  day = now.weekday()
  hour = now.hour

  # uses EMA algorithm to update average utilization value
  old_avg = data[(day, hour)]
  new_avg = old_avg * (1 - alpha) + currentUtilization * alpha
  data[(day, hour)] = new_avg

client.on_connect = on_connect
client.on_message = on_message

@app.route("/")
def heatmap():
  # convert dict -> dataframe
  rows = [{"day": d, "hour": h, "value": v} for (d, h), v in data.items()]
  df = pd.DataFrame(rows)

  # map day numbers to names (matches datetime order of 0=Monday)
  day_names = ["Mon","Tue","Wed","Thu","Fri","Sat","Sun"]
  df["day"] = df["day"].apply(lambda x: day_names[x])

  heatmap_df = df.pivot(index="day", columns="hour", values="value").reindex(["Sun","Mon","Tue","Wed","Thu","Fri","Sat"]) # reordered to have Sunday as first row
  
  # Mark closed hours as NaN
  # Weekdays: 5:30am to 10:00pm (includes down to 5:00am)
  weekday_days = ["Mon", "Tue", "Wed", "Thu", "Fri"]
  heatmap_df.loc[weekday_days, 0:4] = float("nan")
  heatmap_df.loc[weekday_days, 22:23] = float("nan")

  # Saturday: 6:00am to 8:30pm (goes up to 9:00pm)
  heatmap_df.loc[["Sat"], 0:5] = float("nan")
  heatmap_df.loc[["Sat"], 21:23] = float("nan")

  # Sunday: 6:00am to 7:30pm (goes up to 8:00pm)
  heatmap_df.loc[["Sun"], 0:5] = float("nan")
  heatmap_df.loc[["Sun"], 20:23] = float("nan")

  # make heatmap
  fig = px.imshow(
    heatmap_df,
    color_continuous_scale=[
        [0.0, "green"],       # 0%
        [0.5, "#FFEF14"],     # softer yellow (Excel "light yellow")
        [1.0, "red"],         # 100%
    ],
    aspect="auto",
    labels=dict(x="Hour", y="Day", color="Utilization %"),
    zmin=0,     # minimum value of 0% utilization
    zmax=100    # maximum value of 100% utilization
  )

  # convert to HTML snippet
  graph_html = fig.to_html(full_html=False)

  # render inline template
  return render_template_string("""
  <html>
    <head>
      <title>Gas Station Heatmap</title>
    </head>
    <body>
      <h1>Gas Station Utilization</h1>
      {{ graph_html | safe }}
    </body>
  </html>
  """, graph_html=graph_html)

if __name__ == "__main__":
  # Connect to MQTT broker and leave it as looping background thread
  try:
    client.connect(MQTT_BROKER, MQTT_PORT, 60)
    client.loop_start()
  except Exception as e:
    print(f"Failed to connect to MQTT broker: {e}")

  HOST = os.getenv("FLASK_HOST", "127.0.0.1")
  PORT = int(os.getenv("FLASK_PORT", 5000))

  app.run(use_reloader=False, host=HOST, port=PORT)