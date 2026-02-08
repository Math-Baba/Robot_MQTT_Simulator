#include <WiFi.h>
#include <PubSubClient.h>

#define ROBOT_ID 1   

const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqtt_server = "test.mosquitto.org";

WiFiClient espClient;
PubSubClient client(espClient);

int speed = 10;
int battery = 100;

void setup_wifi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void reconnect() {
  while (!client.connected()) {
    String clientId = "ESP32_" + String(ROBOT_ID);
    if (client.connect(clientId.c_str())) {
      client.subscribe(("cars/" + String(ROBOT_ID) + "/command").c_str());
    } else {
      delay(2000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  String cmd;
  for (int i = 0; i < length; i++) {
    cmd += (char)payload[i];
  }

  if (cmd == "speed_up") speed += 5;
  else if (cmd == "speed_down") speed -= 5;
  else if (cmd == "stop") speed = 0;
}

void setup() {
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) reconnect();
  client.loop();

  // 🔁 Simulation identique pour tous
  speed += random(-2, 3);
  if (speed < 0) speed = 0;
  if (speed > 50) speed = 50;

  battery -= random(0, 2);
  if (battery < 0) battery = 100;

  publishStatus("speed", speed, "km/h");
  publishStatus("battery", battery, "%");

  delay(2000);
}

void publishStatus(const char* key, int value, const char* unit) {
  char msg[50];
  sprintf(msg, "%s: %d %s", key, value, unit);

  String topic = "cars/" + String(ROBOT_ID) + "/status";
  client.publish(topic.c_str(), msg);
}
