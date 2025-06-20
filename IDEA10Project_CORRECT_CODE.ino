#include <Adafruit_Sensor.h> 

#include <DHT.h> 

#include <DHT_U.h> 

 

#define DHTPIN 8       // Digital pin connected to the DHT sensor 

#define DHTTYPE DHT11  // DHT 11 

 

DHT_Unified dht(DHTPIN, DHTTYPE); 

 

const float tempHighThreshold = 75.0; // Temperature threshold for cooling (in Fahrenheit) 

const float tempLowThreshold = 65.0;  // Temperature threshold for heating (in Fahrenheit) 

const float humidityThreshold = 60.0; // Humidity threshold for dehumidifier (in percentage) 

 

uint32_t delayMS; 

 

void setup() { 

  // Initialize device 

  Serial.begin(9600); 

 

  // Set LED pins as outputs 

  pinMode(2, OUTPUT); // Red LED for heating 

  pinMode(3, OUTPUT); // Blue LED for cooling 

  pinMode(4, OUTPUT); // Green LED for humidifier 

  pinMode(5, OUTPUT); // Yellow LED for dehumidifier 

   

  // Initialize LEDs to off 

  digitalWrite(2, LOW); 

  digitalWrite(3, LOW); 

  digitalWrite(4, LOW); 

  digitalWrite(5, LOW); 

 

  dht.begin(); 

  Serial.println(F("DHTxx Unified Sensor Example")); 

   

  // Set delay between sensor readings 

  sensor_t sensor; 

  dht.temperature().getSensor(&sensor); 

  delayMS = sensor.min_delay / 1000; 

} 

 

void loop() { 

  // Delay between measurements 

  delay(delayMS); 

 

  // Get temperature event and print its value 

  sensors_event_t event; 

  dht.temperature().getEvent(&event); 

   

  if (isnan(event.temperature)) { 

    Serial.println(F("Error reading temperature!")); 

  } else { 

    float temperatureF = event.temperature * 9 / 5 + 32; // Convert to Fahrenheit 

    Serial.print(F("Temperature: ")); 

    Serial.print(temperatureF); 

    Serial.println(F("°F")); 

 

    // Control LEDs based on temperature thresholds 

    if (temperatureF > tempHighThreshold) { 

      digitalWrite(3, HIGH); // Turn on blue LED for cooling 

      digitalWrite(2, LOW);  // Turn off red LED for heating 

    } else if (temperatureF < tempLowThreshold) { 

      digitalWrite(2, HIGH); // Turn on red LED for heating 

      digitalWrite(3, LOW);  // Turn off blue LED for cooling 

    } else { 

      digitalWrite(2, LOW);  // Turn off red LED for heating 

      digitalWrite(3, LOW);  // Turn off blue LED for cooling 

    } 

  } 

 

  // Get humidity event and print its value 

  dht.humidity().getEvent(&event); 

   

  if (isnan(event.relative_humidity)) { 

    Serial.println(F("Error reading humidity!")); 

  } else { 

    Serial.print(F("Humidity: ")); 

    Serial.print(event.relative_humidity); 

    Serial.println(F("%")); 

 

    // Control LEDs based on humidity threshold 

    if (event.relative_humidity > humidityThreshold) { 

      digitalWrite(5, HIGH); // Turn on yellow LED for dehumidifier 

      digitalWrite(4, LOW);  // Turn off green LED for humidifier 

    } else { 

      digitalWrite(4, HIGH); // Turn on green LED for humidifier 

      digitalWrite(5, LOW);  // Turn off yellow LED for dehumidifier 

    } 

  } 

} 
  