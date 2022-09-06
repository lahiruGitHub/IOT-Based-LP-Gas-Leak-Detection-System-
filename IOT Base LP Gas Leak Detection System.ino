
#include "DHT.h"
#define DHTPIN 25
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);



#define BLYNK_TEMPLATE_ID "TMPLOsKuygUF"
#define BLYNK_DEVICE_NAME "LPGDS"
#define BLYNK_AUTH_TOKEN "-U3DC3CY-6eJIOzLAVQpax64UkyF7HIa"
// Your WiFi Credentials.
// Set password to "" for open networks.
char ssid[] = "Dialog 4G 516";
char pass[] = "Lahiru@99";

#define MQ2_SENSOR    35
#define Buzzer        5
#define SOLENOID_PIN  18
#define GREEN_LED     4
#define RED_LED       14
#define WIFI_LED      2

#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <WiFiClient.h>

#include <BlynkSimpleEsp32.h>
LiquidCrystal_I2C lcd(0x27, 16,2);
BlynkTimer timer;


int count =0;
int MQ2_SENSOR_Value = 0;


bool isconnected = false;
char auth[] = BLYNK_AUTH_TOKEN;

#define VPIN_BUTTON_1    V1 
#define VPIN_BUTTON_2    V2

void checkBlynkStatus() { // called every 2 seconds by SimpleTimer
  isconnected = Blynk.connected();
  if (isconnected == true) {
    digitalWrite(WIFI_LED, HIGH);
     sendData();
    Serial.println("Blynk Connected");
  }
  else{
    digitalWrite(WIFI_LED, LOW);
    Serial.println("Blynk Not Connected");
  }
}

void getSensorData()
{
  MQ2_SENSOR_Value = map(analogRead(MQ2_SENSOR), 0, 4095, 0, 100);
 
  if (MQ2_SENSOR_Value > 40 ){
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, HIGH);
      digitalWrite(Buzzer, HIGH);
    digitalWrite(SOLENOID_PIN, LOW);
    
     lcd.setCursor(0, 1);
      lcd.print("Gas value: High");
      
            delay(1000);
      lcd.clear();//
         
  } 
 
  else {
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(RED_LED, LOW);
     digitalWrite(Buzzer, LOW);
    digitalWrite(SOLENOID_PIN,  HIGH);
     
      lcd.setCursor(0, 1);
      lcd.print("Gas value: Nomal ");
      delay(3000);

      float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));


    
  }
   //lcd.clear();// clear previous values from screen
  float hif = dht.computeHeatIndex(f, h);
  float hic = dht.computeHeatIndex(t, h, false); 

  lcd.setCursor(0,1);
  Serial.print(F("Humidity:"));
  Serial.print(h);
   lcd.print("Humidity:");
     lcd.print(h);
           delay(2000);
   lcd.setCursor(0,1);
  Serial.print(F("%  Temperature: "));
    lcd.print("Temperature:");
     lcd.print(t);
      delay(2000);


    
  }

  
}

void sendData()
{  
  Blynk.virtualWrite(VPIN_BUTTON_1, MQ2_SENSOR_Value);
  if (MQ2_SENSOR_Value > 40 )
  {
    Blynk.logEvent("gas", "Gas Detected!");
  }
  
}

void setup()
{
  Serial.begin(9600);
 
  pinMode(MQ2_SENSOR, INPUT);
  
 pinMode(SOLENOID_PIN, OUTPUT);
  pinMode(Buzzer, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(WIFI_LED, OUTPUT);
  
  Serial.println(F("DHTxx test!"));


  lcd.init();  
  lcd.backlight();



  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, LOW);
  digitalWrite(WIFI_LED, LOW);
  digitalWrite(Buzzer, LOW);
  digitalWrite(SOLENOID_PIN, LOW);


  WiFi.begin(ssid, pass);
  timer.setInterval(2000L, checkBlynkStatus); // check if Blynk server is connected every 2 seconds
  Blynk.config(auth);
  delay(1000);
}

void loop()
{
    lcd.print("Gas Leak Detector");
  lcd.setCursor(0,1);
      lcd.print("Gas value : ");
  lcd.print(MQ2_SENSOR_Value);  
  delay(2000); 
    dht.begin();
  getSensorData();
  Blynk.run();
  timer.run();

   
  

  
  return;
}
