/*

*******************************************************
   AZURE IOT HUB PUB/SUB SAMPLE CODE FOR THE ESP8266
*******************************************************

This sample shows how to subscribe to a devicebound topic endpoint, and to publish to a cloudbound topic endpoint

Ensure you have installed the necessary board dependencies for the ESP8266:

1. Open the Arduino IDE preferences
2. Add the following ESP8266 board configuration URL to the Additional Board Manager URLs field: http://arduino.esp8266.com/stable/package_esp8266com_index.json
3. Open the Boards Manager in the Arduino IDE and search for 'ESP8266', and click install on the appropriate result.

Libraries needed:

  1. ESP8266WiFi
  2. PubSubClient
  3. ESP8266WiFi
  4. ArduinoJson

** Important: Before starting, follow the steps below to increase the message size allowed within the PubSubClientLibrary.
   This is necessary as the IoT Hub SAS token length exceeds the default allowable message size.

  1. Open the file PubSubClient.h for editing
  2. Change the value of MQTT_MAX_PACKET_SIZE from 128 to 256
  3. Save and close the file

You may now add your own Wifi and Azure Iot Hub credentials to this sketch before compiling + uploading.

--------------------------------------------------------------------------------------------------------

Find a bug? Report it, or even better - send me a pull request at https://github.com/noopkat/azure-iothub-pubsub-esp8266

*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

// wifi credentials
const char* wifi_ssid = "";
const char* wifi_password = "";

// example: <myiothub>.azure-devices.net
const char* iothub_url = "";

// this is the id of the device created in Iot Hub
// example: myCoolDevice
const char* iothub_deviceid = "";

// <myiothub>.azure-devices.net/<myCoolDevice>
const char* iothub_user = "";

// SAS token should look like "SharedAccessSignature sr=<myiothub>.azure-devices.net%2Fdevices%2F<myCoolDevice>&sig=123&se=456"
const char* iothub_sas_token = "";

// default topic feed for subscribing is "devices/<myCoolDevice>/messages/devicebound/#""
const char* iothub_subscribe_endpoint = "";

// default topic feed for publishing is "devices/<myCoolDevice>/messages/events/"
const char* iothub_publish_endpoint = "";


WiFiClientSecure espClient;
PubSubClient client(espClient);

long lastMsg = 0;

// function to connect to the wifi
void setup_wifi() {
  delay(10);

  Serial.println();
  Serial.print("Connecting to wifi");

  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // debug wifi via serial
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

// function to connect to MQTT server
void connect_mqtt() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(iothub_deviceid, iothub_user, iothub_sas_token)) {
      Serial.println("connected");
      // ... and resubscribe
      client.subscribe(iothub_subscribe_endpoint);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println("try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

// callback function for when a message is dequeued from the MQTT server
void callback(char* topic, byte* payload, unsigned int length) {
  // print message to serial for debugging
  Serial.print("Message arrived: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println('---');
}

void setup() {
  // begin serial for debugging purposes
  Serial.begin(115200);

  // connect to wifi
  setup_wifi();

  // set up connection and callback for MQTT server
  client.setServer(iothub_url, 8883);
  client.setCallback(callback);

  // connect to MQTT
  connect_mqtt();
}


void loop() {
  client.loop();
  long now = millis();

  // publish data and debug mqtt connection every 10 seconds
  if (now - lastMsg > 10000) {
    lastMsg = now;

    Serial.print("is MQTT client is still connected: ");
    Serial.println(client.connected());

    // set up json object
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();

    // populate keys in json
    root["sensor"] = "moisture";
    // substitute the int value below for a real sensor reading (ie. an analogRead() result)
    root["data"] = 128;

    // convert json to buffer for publishing
    char buffer[256];
    root.printTo(buffer, sizeof(buffer));

    // publish
    client.publish(iothub_publish_endpoint, buffer);
  }
}
