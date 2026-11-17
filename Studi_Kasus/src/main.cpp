#include <Arduino.h>
#include <Wire.h>
#include <HTTPClient.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_INA219.h>
#include <WiFi.h>

Adafruit_SSD1306 display(128, 64, &Wire, -1);
Adafruit_INA219 ina219;

const int PotensiometerPin = 35;
const int LedPin = 32;
const int pwmPin = 33;

const char* ssid = "TelkomV";
const char* password = "22233334";

String apiKey = "...";

const unsigned long intervalThingspeak = 20000;
unsigned long lastSend = 0;

const int pwmChannel = 0;
const int pwmFreq = 5000;
const int pwmResolution = 8;

void setup() {
  Serial.begin(115200);

  Wire.begin();
  ina219.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  pinMode(LedPin, OUTPUT);
  pinMode(PotensiometerPin, INPUT);
  ledcSetup(pwmChannel, pwmFreq, pwmResolution);
  ledcAttachPin(pwmPin, pwmChannel);

  Serial.println("Koneksi ke WiFi...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Menghubungkan ke WiFi...");
  }
  Serial.println("WiFi terhubung!");
}

void loop() {
  int Pot = analogRead(PotensiometerPin);
  int pwmValue = map(Pot, 0, 4095, 0, 255);
  int persen_pwm = map(pwmValue, 0, 255, 0, 100);
  ledcWrite(pwmChannel, pwmValue);

  float	arus_mA	=	ina219.getCurrent_mA();
  float	tegangan_V	=	ina219.getBusVoltage_V();

  display.print("Arus:	"); display.print(arus_mA);
  display.print("	mA	|	Tegangan:	"); display.println(tegangan_V);
  display.print("PWM:	"); display.print(persen_pwm); display.println("%");

  if (millis() - lastSend	>= intervalThingspeak) {
    lastSend	=	millis();
    Serial.print("Persen PWM:	"); Serial.print(persen_pwm); Serial.println("%");
    Serial.print("Arus:	"); Serial.print(arus_mA); Serial.println(" mA");
    Serial.print("Tegangan:	"); Serial.print(tegangan_V); Serial.println(" V");
    
    HTTPClient	http;
    String	url	=	"http://api.thingspeak.com/update?api_key="	+	apiKey + "&field1="	+	String(persen_pwm) + "&field2="	+	String(arus_mA)	+ "&field3="	+	String(tegangan_V);
		http.begin(url);
		int	httpCode	=	http.GET();
		String	respon	=	http.getString();
		http.end();

		if	(httpCode	==	200	&&	respon	!=	"0") {
			Serial.println("Data	terkirim	ke	ThingSpeak,	entry	ke-"	+	respon);
		}else{
			Serial.println("Gagal	kirim	ke	ThingSpeak	(kode	"	+	String(httpCode) +	")");
    }
  }
  delay (1000);
}

