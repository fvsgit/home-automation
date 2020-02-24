#include <Homie.h>
#include <DHTesp.h>

DHTesp dht;

const int TEMPERATURE_INTERVAL = 30;
unsigned long lastTemperatureSent = 0;

HomieNode temperatureNode("temperature", "temperature", "temperature");
HomieNode humidityNode("humidity", "humidity", "humidity");

void loopHandler() {
  if (millis() - lastTemperatureSent >= TEMPERATURE_INTERVAL * 1000UL || lastTemperatureSent == 0) {
    
    //Get the temprature and humidity
    float humidity = dht.getHumidity();
    float temperature = dht.getTemperature();

    Homie.getLogger() << "Temperature: " << temperature << " °C\t\tHumidity: " << humidity << endl;
    temperatureNode.setProperty("degrees").send(String(temperature));
    humidityNode.setProperty("humidity").send(String(humidity));
    lastTemperatureSent = millis();
  }
}

void setup() {
  Serial.begin(115200);
  Serial << endl << endl;
  Homie_setFirmware("awesome-temperature", "1.0.0");
  Homie.setLoopFunction(loopHandler);

  //Setup the homie node
  temperatureNode.advertise("degrees").setName("Degrees")
                                      .setDatatype("float")
                                      .setUnit("ºC");
  humidityNode.advertise("humidity").setName("Humidity")
                                      .setDatatype("float")
                                      .setUnit("%");

  Homie.setup();

  //Setup the DHT11 sensor
  dht.setup(14, DHTesp::DHT11); 
}

void loop() {
  Homie.loop();
}