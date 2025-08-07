# Costco Gas Line Monitor
Every time I go to Costco for gas, I love the indicator showing available pumps. Thought it was cool to recreate it as a prototype and expand it into something smarter.

This project uses distance sensors to detect when a pump is occupied, which is processed through an ESP32 to view on a TFT display while also sending data to a server. The goal with the prototype is to let users check live pump availability and figure out the best times to get gas at a location.