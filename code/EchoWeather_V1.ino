#include "AS5600.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include "Adafruit_VEML7700.h"
#include "LTR390.h"
//#include <esp_sleep.h>
#include "WiFi.h"
#include "Creds.h"
#include "time.h"
#include <sys/time.h>
#include <ESP_Google_Sheet_Client.h>
#include <GS_SDHelper.h>

// Wind, rain, and battery pin definitions
#define windPin 10
#define rainPin 6
#define batteryPin 2

// Token Callback function
void tokenStatusCallback(TokenInfo info);

// NTP server to request epoch time
const char* ntpServer = "pool.ntp.org";

// Variable to save current epoch time
unsigned long epochTime; 

bool connected = false;

// Function that gets current epoch time
unsigned long getTime() {
  time_t now;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    //Serial.println("Failed to obtain time");
    return(0);
  }
  time(&now);
  return now;
}

AS5600 as5600;
Adafruit_BME280 bme; // I2C
Adafruit_VEML7700 veml = Adafruit_VEML7700();
UDOUBLE UV,ALS;

// Calibration Factors
float UVscale = 0.0285;
float lightScale1 = 1.8;
float lightScale2 = 7;
float tempScale = 1;
float tempOff = 0;
float presScale = 1.003;
float presOff = 0;
float humidScale = 1;
float humidOff = 0;
int directionOffset = -270; //degree offset

volatile unsigned long windCount = 0;
volatile unsigned long rainCount = 0;

unsigned long startTime = millis();
unsigned long windPulseTime = millis();
unsigned long windElapsedPulse = 0;
unsigned long measureTime = 591; // Time in Seconds 591(10 mins, extra delay for WiFi connection) -----------------------------------<<<<<<
//unsigned long restTime = 20; // Time in Seconds

float gustWindSpeed = 0;
float maxGust = 0;

// interrupt for wind pulses and calculating wind gust speed
void IRAM_ATTR onWindPulse() {
  windElapsedPulse = millis() - windPulseTime;
  gustWindSpeed = 1.654 / (windElapsedPulse/1000.0);
  if (gustWindSpeed > maxGust){
    maxGust = gustWindSpeed;
  }
  windPulseTime = millis();
  windCount++;
}

// interrupt for counting rain pulses
void IRAM_ATTR onRainPulse() {
  rainCount++;
}


void setup() {
  Serial.begin(115200);

  delay(500);
  //Serial.print("Wake Interval: ");
  //Serial.print(restTime);
  //Serial.print("s, ");
  Serial.print("Measure Interval: ");
  Serial.print(measureTime);
  Serial.println("s");
  //esp_sleep_enable_timer_wakeup(wakeUpInterval * 1000000);
  Wire.begin();

// Wind Direction Setup
  as5600.begin(4);  //  set direction pin.
  as5600.setDirection(AS5600_CLOCK_WISE);  //  default, just be explicit.
  //int b = as5600.isConnected();
  //Serial.print("Connect: ");
  //Serial.println(b);
  delay(100);

// BME setup
  unsigned status;
  status = bme.begin(0x76);
  bme.setSampling(Adafruit_BME280::MODE_FORCED,
                    Adafruit_BME280::SAMPLING_X1, // temperature
                    Adafruit_BME280::SAMPLING_X1, // pressure
                    Adafruit_BME280::SAMPLING_X1, // humidity
                    Adafruit_BME280::FILTER_OFF   );   
  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
    Serial.print("SensorID was: 0x"); Serial.println(bme.sensorID(),16);
    Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
    Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
    Serial.print("        ID of 0x60 represents a BME 280.\n");
    Serial.print("        ID of 0x61 represents a BME 680.\n");
    //while (1) delay(10);
  }

  // veml Setup
  if (!veml.begin()) {
    Serial.println("Sensor not found");
  }

  veml.setGain(VEML7700_GAIN_1_8);      // 1/8
  veml.setIntegrationTime(VEML7700_IT_25MS); // 25 ms
  veml.interruptEnable(false);

  //UVSetup
  if(LTR390_Init() != 0){
    //Serial.print("init err!!!");
    while(1);
  }
  LTR390_SetIntVal(5, 20);

  //Wind speed setup
  pinMode(windPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(windPin), onWindPulse, FALLING);

  //Rain Gauge Setup
  pinMode(rainPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(rainPin), onRainPulse, FALLING);

  //Battery Voltage Setup
  pinMode(batteryPin, INPUT);

}

void loop() {

if (!connected){
// Measuring Phase for rain and wind speed
  unsigned long cycleStart = millis();
  unsigned long elapsedTime = 0;
  while (elapsedTime <= measureTime*1000){
    elapsedTime = millis() - cycleStart;
    delay(1);
  }
  connected = ConnectToSheets();
}
if (!connected) return;
  bool ready = GSheet.ready();

if (ready){
  // Get sensor values
    int windDegree = GetWindDegree();
    String windCardinalDirection = GetWindCardinalDirection(windDegree);
    float temperatureF = (bme.readTemperature() * 1.8 + 32) * tempScale + tempOff;
    float pressure = bme.readPressure() / 100.0F * presScale + presOff;
    float humidity = bme.readHumidity() * humidScale + humidOff;
    int lightLevel = GetLuxLevel();
    int UV = LTR390_UVS()*UVscale;
    float windSpeed = (float(windCount)/float(measureTime))*1.66;
    float rainRate = float(rainCount)/float(measureTime)*99.216;
    float batteryVoltage = GetBatteryVoltage();
    epochTime = getTime();
    
  // Print Values
    /*
    Serial.print("Wind Direction Degrees: ");
    Serial.print(windDegree);
    Serial.println("°");
    Serial.print("Wind Cardinal Direction: ");
    Serial.println(windCardinalDirection);
    Serial.print("Temperature: ");
    Serial.print(temperatureF);
    Serial.println("°F");
    Serial.print("Pressure: ");
    Serial.print(pressure);
    Serial.println("hPa");
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println("%");
    Serial.print("Light Level: ");
    Serial.print(lightLevel);
    Serial.println("Lux");
    Serial.print("UV Level: ");
    Serial.println(UV);
    Serial.print("Wind speed: ");
    Serial.print(windSpeed);
    Serial.println(" mph");
    Serial.print("Wind Gust: ");
    Serial.print(maxGust);
    Serial.println(" mph");
    Serial.print("Rain Rate: ");
    Serial.print(rainRate);
    Serial.println(" in/hr");
    Serial.print("Battery Voltage: ");
    Serial.print(batteryVoltage);
    Serial.println(" V");
    Serial.println("");
    */

    FirebaseJson response;

    Serial.printf("Free heap: %u bytes\n", ESP.getFreeHeap());

    Serial.println("\nAppend spreadsheet values...");
    Serial.println("----------------------------");

    FirebaseJson valueRange;

    // append values to spread sheet
    valueRange.add("majorDimension", "COLUMNS");
    valueRange.set("values/[0]/[0]", epochTime);
    valueRange.set("values/[1]/[0]", (int)temperatureF);
    addFloat(valueRange, "values/[2]/[0]", humidity);
    addFloat(valueRange, "values/[3]/[0]", pressure);
    valueRange.set("values/[4]/[0]", windDegree);
    valueRange.set("values/[5]/[0]", windCardinalDirection);
    addFloat(valueRange, "values/[6]/[0]", windSpeed);
    addFloat(valueRange, "values/[7]/[0]", maxGust);
    addFloat(valueRange, "values/[8]/[0]", rainRate);
    valueRange.set("values/[9]/[0]", lightLevel);
    valueRange.set("values/[10]/[0]", UV);
    addFloat(valueRange, "values/[11]/[0]", batteryVoltage);
    valueRange.set("values/[12]/[0]", location);

    // check if successfully appended to sheet
    bool success = GSheet.values.append(&response /* returned response */, spreadsheetId /* spreadsheet Id to append */, "Sheet1!A1" /* range to append */, &valueRange /* data range to append */);
    if (success){
        response.toString(Serial, true);
        valueRange.clear();
    }
    else{
        Serial.println(GSheet.errorReason());
    }
    Serial.println();

    delay(5000);

    // go to deep sleep mode ------------------<<<<<<<<<<<<<<
    WiFi.setSleep(true); 
    Serial.println(WiFi.status());
    Serial.println("");
    ESP.restart();
    //Serial.flush();
    //esp_deep_sleep_start();
  }
}

//Battery Voltage Level
float GetBatteryVoltage(){
  return analogReadMilliVolts(2)*0.00147;
  //return map(analogRead(batteryPin),0,2715.0,0,391.0)/100.0;
}

//Light Levels
int GetLuxLevel(){
  int lux = veml.readLux();
  if (lux > 15000){
    lux = lux * lightScale2;
  } else {
    lux = lux * lightScale1;
  }
  delay(500);
  return lux;
}


// Wind direction
int GetWindDegree(){
  int windDegree = as5600.rawAngle() * AS5600_RAW_TO_DEGREES + directionOffset;
  if (windDegree > 360){
    windDegree -= 360;
  } else if (windDegree < 0){
    windDegree += 360;
  }
  return windDegree;
}

// Wind cardinal direction
String GetWindCardinalDirection(int windDegree){
  String cardinal = "ERROR";
  if (windDegree >= 338 || windDegree < 23)
    cardinal = "N";
  if (windDegree >= 23 && windDegree < 68)
    cardinal = "NE";
  if (windDegree >= 68 && windDegree < 113)
    cardinal = "E";
  if (windDegree >= 113 && windDegree < 158)
    cardinal = "SE";
  if (windDegree >= 158 && windDegree < 203)
    cardinal = "S";
  if (windDegree >= 203 && windDegree < 248)
    cardinal = "SW";
  if (windDegree >= 248 && windDegree < 293)
    cardinal = "W";
  if (windDegree >= 293 && windDegree < 338)
    cardinal = "NW";
  return cardinal;
}

// function to connect to google sheets
bool ConnectToSheets(){
    //Configure time
  configTime(0, 0, ntpServer);

  GSheet.printf("ESP Google Sheet Client v%s\n\n", ESP_GOOGLE_SHEET_CLIENT_VERSION);
// Connect to Wi-Fi
  WiFi.setAutoReconnect(true);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to Wi-Fi");
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 15) {
    Serial.print(".");
    delay(1000);
    attempts++;
  }

  if(WiFi.status() != WL_CONNECTED){
    Serial.println("\nWiFi Failed. Continuing offline...");
    return false;
  }

  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  // Set the callback for Google API access token generation status (for debug only)
  GSheet.setTokenCallback(tokenStatusCallback);

  // Set the seconds to refresh the auth token before expire (60 to 3540, default is 300 seconds)
  GSheet.setPrerefreshSeconds(10 * 60);

  // Begin the access token generation for Google API authentication
  GSheet.begin(CLIENT_EMAIL, PROJECT_ID, PRIVATE_KEY);
  return true;
}

// gives status of google sheet connetions
void tokenStatusCallback(TokenInfo info){
  
    if (info.status == token_status_error){
        GSheet.printf("Token info: type = %s, status = %s\n", GSheet.getTokenType(info).c_str(), GSheet.getTokenStatus(info).c_str());
        GSheet.printf("Token error: %s\n", GSheet.getTokenError(info).c_str());
        delay(10000);
        WiFi.setSleep(true); 
        Serial.println(WiFi.status());
        Serial.println("");
        ESP.restart();
    }
    else{
        GSheet.printf("Token info: type = %s, status = %s\n", GSheet.getTokenType(info).c_str(), GSheet.getTokenStatus(info).c_str());
    }
}

// function used to reduce json size to append to sheets, for memory reduction for sending.
void addFloat(FirebaseJson &json, const char *path, float value) {
  char buf[16];
  snprintf(buf, sizeof(buf), "%.2f", value);
  json.set(path, buf);
}
