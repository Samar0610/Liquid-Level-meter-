#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include"Config.h"
#include "Liquid_level.h"

WiFiClient espClient;
PubSubClient client(espClient);


int height = 7;



void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
 
  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  float dist = ultarsonic_distance();
  float percentage;
     if (dist<=1){
      percentage=100;
     }
     else if(dist>=7)
     {
      percentage=0;
     }
     else
     {
         percentage = (((height - (dist-1)) / height) * 100);
     }

  // Prints the distance on the Serial Monitor
  Serial.print("Distance (cm): ");
  Serial.println(dist);
  Serial.print("Percentage : ");
  Serial.println(percentage);
  Serial.println("%");
  
 

  if (client.publish("ultasonic/level", String(percentage).c_str(), true)) {
    Serial.println("Publish successful");
  } else {
    Serial.println("Publish failed");
  }

  

  delay(4000);
}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}


