#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>



//const char* ssid = "CENSORED";
const char* ssid = "Wokwi-GUEST";
//const char* password = "CENSORED";
const char* weatherserver = "http://api.open-meteo.com/v1/forecast?latitude=47.1667&longitude=27.6&daily=temperature_2m_min,temperature_2m_max,sunset,sunrise,precipitation_hours&hourly=temperature_2m,precipitation,precipitation_probability&current=temperature_2m&timezone=auto&forecast_days=1";
unsigned long lastTime = 0;
//unsigned long timerDelay = 600000; //10 min
unsigned long timerDelay = 5000; //5 sec

int ChooseNr; // the number that signifies what data i want to retrieve
double MainNr;
String MainString;
double MainData[24]; // mix used array for hourly 


String weather;
float weatherArr[3];

void setup(){

  // START CONNECT WIFI
    Serial.begin(115200);
    delay(1000);

    WiFi.mode(WIFI_STA); //Optional
    //WiFi.begin(ssid, password);
    WiFi.begin(ssid);
    Serial.println("\nConnecting");

    while(WiFi.status() != WL_CONNECTED){
        Serial.print(".");
        delay(100);
    }

    Serial.println("\nConnected to the WiFi network");
    Serial.print("Local ESP32 IP: ");
    Serial.println(WiFi.localIP());
  // END CONNECT WIFI
}

void loop() {
  //Send an HTTP POST request every 10 minutes
  if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      weather = httpGETRequest(weatherserver);
      parseWeather(weather,13,MainNr,MainString,MainData); // THE CHOOSENR IS BACKWARDS
      Serial.println(MainNr);
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();


  }
}

void parseWeather(String jsonString,int ChooseNr, double& ReturnNr, String& ReturnString,double date[]) {
  JSONVar root = JSON.parse(jsonString);

  if (JSON.typeof(root) == "undefined") {
    Serial.println("Parsing input failed!");
    return;
  }
  if(ChooseNr%10==1){
    String TimezoneAbrev = (const char*) root["timezone_abbreviation"];
    ReturnString=TimezoneAbrev;
    return;
  }
  else if(ChooseNr%10==2){
    ChooseNr/=10;
    if(ChooseNr%10==1){
      String time=(const char*) root["current"]["time"];
      ReturnString=time;
      Serial.println(time);
      return;
    }
    else if(ChooseNr%10==2){
      String interval=(const char*) root["current"]["interval"];
      ReturnString=interval;
      return;
    }
    else if(ChooseNr%10==3){
      double temper=double(root["current"]["temperature_2m"]);
      ReturnNr=temper;
      return;
    }
    else{
      String precip=(const char*) root["current"]["precipitation"];
      ReturnString=precip;
      return;
    }
  }
  else if(ChooseNr%10==3){
      ChooseNr/=10;
      if(ChooseNr%10==1){
        for(int i=0;i<=23;i++){
          const char* time = (const char*) root["hourly"]["time"][i];
          const char* sani_time=time+11;
          date[i]=atof(sani_time);
          //TODO eliminia 2025-08-whatever ca sa poti sa faci cu un singur array cu tipu de date int ca altfelai avea neovie is de string
        }
      }
      if(ChooseNr%10==2){
        //array temp
        for(int i=0;i<=23;i++){
          date[i]=double(root["hourly"]["temperature_2m"][i]);
        }
      }
      if(ChooseNr%10==3){
        //array percep
        for(int i=0;i<=23;i++){
          date[i]=double(root["hourly"]["precipitation"][i]);
        }
      }
      else{
        //array precep prob
        for(int i=0;i<=23;i++){
          date[i]=double(root["hourly"]["precipitation_probability"][i]);
        }
      }
    }
  else{
    ChooseNr/=10;
    if(ChooseNr%10==1){
      String time = (const char*) root["daily"]["time"];
      ReturnString=time;
      return;
    }
    if(ChooseNr%10==2){
      String sunrise = (const char*) root["daily"]["sunrise"];
      ReturnString = sunrise;
      return;
    }
    else{
      String sunset = (const char*) root["daily"]["sunset"];
      ReturnString= sunset;
      return;
    }
  }
}

  /*
  // --- Top-level fields ---
  double latitude = double(root["latitude"]);
  double longitude = double(root["longitude"]);
  String timezone = (const char*) root["timezone"];

  Serial.println("Top-level:");
  Serial.print("Latitude: "); Serial.println(latitude);
  Serial.print("Longitude: "); Serial.println(longitude);
  Serial.print("Timezone: "); Serial.println(timezone);

  // --- Current weather ---
  JSONVar current = root["current"];
  String currentTime = (const char*) current["time"];
  double tempNow = double(current["temperature_2m"]);
  double precipNow = double(current["precipitation"]);

  Serial.println("\nCurrent:");
  Serial.print("Time: "); Serial.println(currentTime);
  Serial.print("Temp: "); Serial.println(tempNow);
  Serial.print("Precipitation: "); Serial.println(precipNow);

  // --- Hourly data ---
  JSONVar hourly = root["hourly"];
  JSONVar hourlyTimes = hourly["time"];
  JSONVar hourlyTemps = hourly["temperature_2m"];

  Serial.println("\nHourly:");
  for (int i = 0; i < hourlyTimes.length(); i++) {
    Serial.print((const char*) hourlyTimes[i]);
    Serial.print(" - ");
    Serial.println(double(hourlyTemps[i]));
  }

  // --- Daily data ---
  JSONVar daily = root["daily"];
  String sunrise = (const char*) daily["sunrise"][0];
  String sunset = (const char*) daily["sunset"][0];

  Serial.println("\nDaily:");
  Serial.print("Sunrise: "); Serial.println(sunrise);
  Serial.print("Sunset: "); Serial.println(sunset);
  */

String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;
    
  // Your Domain name with URL path or IP address with path
  http.begin(client, weatherserver);
  
  // If you need Node-RED/server authentication, insert user and password below
  //http.setAuthorization("REPLACE_WITH_SERVER_USERNAME", "REPLACE_WITH_SERVER_PASSWORD");
  
  // Send HTTP POST request
  int httpResponseCode = http.GET();
  
  String payload = "{}"; 
  
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}
