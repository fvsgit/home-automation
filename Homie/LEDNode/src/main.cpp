#include <Homie.h>
#include <DHT.h>
#include <DHT_U.h>

const int TEMPERATURE_INTERVAL = 30;
unsigned long lastTemperatureSent = 0;

//Define the Pin
#define DHTPIN     14         // Pin connected to the DHT sensor.

// Uncomment the type of sensor in use:
#define DHTTYPE    DHT11     // DHT 11
//#define DHTTYPE    DHT22     // DHT 22 (AM2302)
//#define DHTTYPE    DHT21     // DHT 21 (AM2301)

HomieNode temperatureNode("temperature", "Temperature", "temperature");
DHT_Unified dht(DHTPIN, DHTTYPE);

void loopHandler() {
  if (millis() - lastTemperatureSent >= TEMPERATURE_INTERVAL * 1000UL || lastTemperatureSent == 0) {
    float temperature = 22; // Fake temperature here, for the example
    Homie.getLogger() << "Temperature: " << temperature << " °C" << endl;
    temperatureNode.setProperty("degrees").send(String(temperature));
    lastTemperatureSent = millis();
  }
}

void setup() {
  Serial.begin(115200);
  Serial << endl << endl;
  Homie_setFirmware("awesome-temperature", "1.0.0");
  Homie.setLoopFunction(loopHandler);

  temperatureNode.advertise("degrees").setName("Degrees")
                                      .setDatatype("float")
                                      .setUnit("ºC");

  Homie.setup();
}

void loop() {
  Homie.loop();
}