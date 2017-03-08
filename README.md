# azure-iothub-pubsub-esp8266

Using the Arduino PubSub library with Azure IoT Hub on an ESP8266.

This sample demonstrates: 
+ how to subscribe to a devicebound topic endpoint in Azure IoT Hub
+ how to publish to a cloudbound topic endpoint in Azure IoT Hub

## Instructions

Ensure you have installed the necessary board dependencies for the ESP8266:

1. Open the Arduino IDE preferences
2. Add the following ESP8266 board configuration URL to the Additional Board Manager URLs field: `http://arduino.esp8266.com/stable/package_esp8266com_index.json`
3. Open the Boards Manager in the Arduino IDE, search for 'ESP8266', and click install on the appropriate result.

Libraries needed (install via the Arduino IDE Library Manager):

1. ESP8266WiFi
2. PubSubClient
3. ESP8266WiFi
4. ArduinoJson

**Important:** Before starting, follow the steps below to increase the message size allowed within the PubSubClientLibrary. This is necessary as the IoT Hub SAS token length exceeds the default allowable message size.

1. Open the file `PubSubClient.h` for editing, located where you installed the PubSubClient library
2. Change the value of `MQTT_MAX_PACKET_SIZE` variable from `128` to `256`
3. Save and close the file

You may now add your own Wifi and Azure Iot Hub credentials to this sketch before compiling + uploading.

---

Find a bug? Report it as an issue, or even better - send me a pull request!
