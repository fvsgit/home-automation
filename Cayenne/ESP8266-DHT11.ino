#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <CayenneMQTTESP8266.h>

#define DHTPIN     14         // Pin connected to the DHT sensor.
#define CAYENNE_PRINT Serial
#define DHTTYPE    DHT11     // DHT 11

// WiFi network info.
char ssid[] = "xyz";
char wifiPassword[] = "xyz";

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "xxx";
char password[] = "xxx";
char clientID[] = "xxx";

DHT_Unified dht(DHTPIN, DHTTYPE);
uint32_t delayMS;

void setup() {
	Serial.begin(9600);
	Cayenne.begin(username, password, clientID, ssid, wifiPassword);

  // Initialize DHT11 device.
  dht.begin(); 
  sensor_t sensor;
  dht.temperature().getSensor(&sensor); 
  dht.humidity().getSensor(&sensor);
  delayMS = sensor.min_delay / 1000;
}

void loop() {
	Cayenne.loop();
}

// Default function for sending sensor data at intervals to Cayenne.
// You can also use functions for specific channels, e.g CAYENNE_OUT(1) for sending channel 1 data.
CAYENNE_OUT_DEFAULT()
{
	// Write data to Cayenne here. This example just sends the current uptime in milliseconds on virtual channel 0.
	//Cayenne.virtualWrite(0, millis());
	// Some examples of other functions you can use to send data.
	//Cayenne.celsiusWrite(1, 22.0);
	//Cayenne.luxWrite(2, 700);
	//Cayenne.virtualWrite(3, 50, TYPE_PROXIMITY, UNIT_CENTIMETER);

   sensors_event_t event;
    dht.temperature().getEvent(&event);
    if (isnan(event.temperature)) {
      Serial.println(F("Error reading temperature!"));
    }
    else {
      Serial.print(F("Temperature: "));
      Serial.print(event.temperature);
      Cayenne.celsiusWrite(1, event.temperature);
      Serial.println(F("°C"));
    }
    // Get humidity event and print its value.
    dht.humidity().getEvent(&event);
    if (isnan(event.relative_humidity)) {
      Serial.println(F("Error reading humidity!"));
    }
    else {
      Serial.print(F("Humidity: "));
      Serial.print(event.relative_humidity);
      Cayenne.virtualWrite(2, event.relative_humidity, "rel_hum", "p");
      Serial.println(F("%"));
    }
}

// Default function for processing actuator commands from the Cayenne Dashboard.
// You can also use functions for specific channels, e.g CAYENNE_IN(1) for channel 1 commands.
CAYENNE_IN_DEFAULT()
{
	CAYENNE_LOG("Channel %u, value %s", request.channel, getValue.asString());
	//Process message here. If there is an error set an error message using getValue.setError(), e.g getValue.setError("Error message");
}
