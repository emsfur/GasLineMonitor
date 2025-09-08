FROM eclipse-mosquitto:latest

# Arguments for username and password (override if needed)
ARG MQTT_USER=mqtt
ARG MQTT_PASS=password

# Copy mosquitto.conf into container (overrides default configuration)
COPY mosquitto.conf /mosquitto/config/mosquitto.conf

# Create credentials file inside container
RUN mosquitto_passwd -c -b /mosquitto/config/credentials $MQTT_USER $MQTT_PASS

EXPOSE 1883