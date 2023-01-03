#include <Arduino.h>
#include <ModbusMaster.h>
#include <SoftwareSerial.h>
#include <ESP8266Wifi.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>

// const char *ssid = "KS 24 BLIMBING OUTDOOR";
// const char *password = "pancongnyamantap";
const char *ssid = "such a person";
const char *password = "zidanedane";
// const char *ssid = "Kombukei";
// const char *password = "cobamisoa";
// const char *ssid = "Mavens 2G";
// const char *password = "adminmavens";

const char *mqtt_server = "118.98.64.212";
const char *userBroker = "admin";
const char *passBroker = "adminmavens";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];
char msg2[10];
char status_msg[25];
int value = 0;

//============================================================
// fungsi modul untuk koneksi wifi
void setup_wifi()
{

  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
//============================================================

//============================================================
// fungsi connect MQTT ke Server Broker
void connectMQTT()
{
  // Loop sampai reconnected
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // membuat client ID random
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(), userBroker, passBroker))
    {
      Serial.println("connected");
      // Jika connected, publish topic sekali...
      // client.publish("sensor/suara", "Pembacaan Sensor Suara");
      // ... dan resubscribe
      // client.subscribe("sensor/suara");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Delay 5 detik sampai tersambung lagi
      delay(5000);
    }
  }
}

//============================================================<

// fungsi kirim data format json
void sendJsonData(String jenisSuara, int wave, int rate, int tb, int btb)
{
  StaticJsonBuffer<300> JSONbuffer;
  JsonObject &JSONencoder = JSONbuffer.createObject();

  JSONencoder["jenisSuara"] = jenisSuara;
  JSONencoder["wave"] = wave;
  JSONencoder["rate"] = rate;
  JSONencoder["tb"] = tb;
  JSONencoder["btb"] = btb;

  char JSONmessageBuffer[100];
  JSONencoder.printTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
  Serial.println("Kirim data analisa to MQTT Broker");
  Serial.println(JSONmessageBuffer);

  if (client.publish("analisa/suara", JSONmessageBuffer) == true)
  {
    Serial.println("Success sending message");
  }
  else
  {
    Serial.println("Error sending message");
  }

  client.loop();
  Serial.println("-------------");
}
//============================================================>



void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, 1883);

  // client.setCallback(callback);
  digitalWrite(LED_BUILTIN, HIGH);

}

void loop() {
  // put your main code here, to run repeatedly:
}