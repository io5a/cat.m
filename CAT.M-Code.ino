#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_NeoPixel.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define NUMPIXELS        1

unsigned status;

int max_screen_nr = 2;
int curr_screen = 1;

Adafruit_BMP280 bmp;
Adafruit_SSD1306 display1(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
Adafruit_NeoPixel pixels(NUMPIXELS, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);

#define I2C_ADDRESS 0x3C

const char* ssid = "CENSORED";
//const char* ssid = "Wokwi-GUEST";
const char* password = "CENSORED";
const char* weatherserver = "http://api.open-meteo.com/v1/forecast?latitude=47.1667&longitude=27.6&daily=temperature_2m_min,temperature_2m_max,sunset,sunrise,precipitation_hours&hourly=temperature_2m,precipitation,precipitation_probability&current=temperature_2m&timezone=auto&forecast_days=1";
unsigned long lastTime = 0;
unsigned long timerDelay = 600000; //10 min
unsigned long debounceTime = 0;
unsigned long debounceDelay = 100;
//unsigned long timerDelay = 5000; //5 sec

int ChooseNr; // the number that signifies what data i want to retrieve
int MainNr;
String MainString;
double MainData[24]; // mix used array for hourly 

int buttonValue;

int CurrHour;

bool start=true;


String weather;
float weatherArr[3];

const unsigned char cat_bitmap [] PROGMEM = {
  0x00, 0xf8, 0x00, 0xfc, 0x00, 0x00, 0x01, 0x8c, 0x01, 0x86, 0x00, 0x00, 0x03, 0x06, 0x01, 0x32, 
  0x00, 0x00, 0x02, 0x73, 0x02, 0x7a, 0x00, 0x00, 0x06, 0xf9, 0x02, 0xfb, 0x00, 0x00, 0x04, 0xfd, 
  0x82, 0xf9, 0x00, 0x00, 0x05, 0xfc, 0xfe, 0xfd, 0x00, 0x00, 0x05, 0xfc, 0x54, 0xfd, 0x00, 0x00, 
  0x05, 0xfc, 0x54, 0x7d, 0x00, 0x00, 0x04, 0xf0, 0x00, 0x1d, 0x00, 0x00, 0x04, 0x00, 0x00, 0x01, 
  0x00, 0x00, 0x04, 0x00, 0x00, 0x01, 0x00, 0x00, 0x04, 0x00, 0x00, 0x01, 0x81, 0xfc, 0x04, 0x00, 
  0x00, 0x00, 0x82, 0xaa, 0x04, 0x0e, 0x00, 0xe0, 0x82, 0xaa, 0x0c, 0x1f, 0x01, 0xf0, 0xc2, 0xaa, 
  0x08, 0x0e, 0x00, 0xe0, 0x42, 0x02, 0x08, 0x00, 0x30, 0x00, 0x42, 0x02, 0x08, 0x00, 0x30, 0x00, 
  0x42, 0x02, 0x08, 0x00, 0x00, 0x00, 0x42, 0x02, 0x08, 0x00, 0x00, 0x00, 0x42, 0x02, 0x08, 0x02, 
  0x31, 0x00, 0x42, 0x02, 0x08, 0x03, 0x7b, 0x00, 0x42, 0x02, 0x0c, 0x01, 0xce, 0x00, 0x42, 0x02, 
  0x04, 0x00, 0x00, 0x00, 0x82, 0x02, 0x06, 0x00, 0x00, 0x00, 0x82, 0x02, 0x03, 0x00, 0x00, 0x00, 
  0x82, 0x02, 0x01, 0x00, 0x00, 0x03, 0x82, 0x06, 0x03, 0x80, 0x00, 0x0e, 0xc6, 0x04, 0x06, 0x60, 
  0x00, 0x00, 0x4c, 0x04, 0x04, 0x00, 0x00, 0x00, 0x58, 0x04, 0x0c, 0x00, 0x00, 0x00, 0x70, 0x0c, 
  0x18, 0x00, 0x00, 0x00, 0x00, 0x08, 0x10, 0x00, 0x00, 0x00, 0x00, 0x18, 0x30, 0x00, 0x00, 0x00, 
  0x00, 0x30, 0x60, 0x00, 0x00, 0x00, 0x00, 0x60, 0x40, 0xc0, 0x00, 0x00, 0x00, 0xc0, 0xc0, 0x70, 
  0x00, 0x00, 0x07, 0x80, 0x80, 0x1c, 0x00, 0x00, 0x1c, 0x00, 0x80, 0x04, 0x00, 0x00, 0x20, 0x00, 
  0x40, 0x24, 0x00, 0x00, 0x20, 0x00, 0x60, 0x94, 0x00, 0x00, 0x20, 0x00, 0x32, 0x4c, 0x00, 0x00, 
  0x20, 0x00, 0x19, 0x3c, 0x00, 0x00, 0x30, 0x00, 0x08, 0x98, 0x00, 0x00, 0x18, 0x00, 0x1c, 0xe0, 
  0x00, 0x00, 0x0c, 0x00, 0x37, 0x98, 0x00, 0x07, 0xe4, 0x00, 0x20, 0x0c, 0x00, 0x1c, 0x34, 0x00, 
  0x20, 0x04, 0x00, 0x30, 0x16, 0x00, 0x45, 0x44, 0x00, 0x65, 0x52, 0x00, 0x45, 0x44, 0x00, 0x45, 
  0x52, 0x00, 0x45, 0x44, 0x00, 0x45, 0x5a, 0x00, 0x40, 0x04, 0x00, 0x40, 0x0a, 0x00, 0x63, 0x84, 
  0x00, 0x40, 0x0e, 0x00, 0x23, 0x88, 0x00, 0x43, 0x8c, 0x00, 0x33, 0x88, 0x00, 0x43, 0x8c, 0x00, 
  0x18, 0x7f, 0xff, 0xc3, 0x98, 0x00, 0x0f, 0xc0, 0x00, 0xe0, 0x30, 0x00, 0x00, 0x00, 0x00, 0x3f, 
  0xe0, 0x00
};

void DisplayClock(int x,int y){
  //7,7
  display1.drawCircle(x, y, 5, SSD1306_WHITE);
  display1.drawLine(x, y, x, y-3, SSD1306_WHITE);
  display1.drawLine(x, y, x+2, y, SSD1306_WHITE);

}
void DisplayInTherm(int x,int y){
    // 7,19
    //x4,y22 -> x4,y16
    display1.drawLine(x-3,y+3,x-3,y-3,SSD1306_WHITE);
    //x8,y20 -> x8,y15
    display1.drawLine(x+1,y+1,x+1,y-4,SSD1306_WHITE);
    //x11
    display1.drawLine(x+4,y+1,x+4,y-4,SSD1306_WHITE);
    //x7,y21->x7,y23
    display1.drawLine(x,y+2,x,y+4,SSD1306_WHITE);
    //x12
    display1.drawLine(x+5,y+2,x+5,y+4,SSD1306_WHITE);
    //x8,y24->x11,y24
    display1.drawLine(x+1,y+5,x+4,y+5,SSD1306_WHITE);
    //x9,y14->x10,y14
    display1.drawLine(x+2, y-5,x+3,y-5, SSD1306_WHITE);
    //x10,y16
    display1.drawPixel(x+3, y-3, SSD1306_WHITE);
    //x10,y18
    display1.drawPixel(x+3, y-1, SSD1306_WHITE);
    //x10,y20
    display1.drawPixel(x+3, y+1,SSD1306_WHITE);
}

void DisplayOutTherm(int x,int y){
    //x7,y31
    display1.drawLine(x+1,y+1,x+1,y-4,SSD1306_WHITE);
    //x11
    display1.drawLine(x+4,y+1,x+4,y-4,SSD1306_WHITE);
    //x7,y21->x7,y23
    display1.drawLine(x,y+2,x,y+4,SSD1306_WHITE);
    //x12
    display1.drawLine(x+5,y+2,x+5,y+4,SSD1306_WHITE);
    //x8,y24->x11,y24
    display1.drawLine(x+1,y+5,x+4,y+5,SSD1306_WHITE);
    //x9,y14->x10,y14
    display1.drawLine(x+2, y-5,x+3,y-5, SSD1306_WHITE);
    //x10,y16
    display1.drawPixel(x+3, y-3, SSD1306_WHITE);
    //x10,y18
    display1.drawPixel(x+3, y-1, SSD1306_WHITE);
    //x10,y20
    display1.drawPixel(x+3, y+1,SSD1306_WHITE);
    //x2,y29->x2,y33
    display1.drawLine(x-5,y-2,x-5,y+2,SSD1306_WHITE);
    //x5,y29->x5,y33
    display1.drawLine(x-2,y-2,x-2,y+2,SSD1306_WHITE);
    //x3,y28->x4,y28
    display1.drawLine(x-4,y-3,x-3,y-3,SSD1306_WHITE);
    //x3,y34->x4,y34
    display1.drawLine(x-4,y+3,x-3,y+3,SSD1306_WHITE);
}

void DisplayMoon(int x,int y){
    //x7,y43

    display1.drawCircle(x,y,5,SSD1306_WHITE);
    //x4,y41
    display1.drawRect(x-3, y-2, 2, 2, SSD1306_WHITE);
    //x8,y43
    display1.drawRect(x+1,y,3,2,SSD1306_WHITE);
    //x9,y41
    display1.drawRect(x+2,y-2,2,2,SSD1306_WHITE);
    //x5,y46->x6,y45
    display1.drawLine(x-2,y+3,x-1,y+2,SSD1306_WHITE);
    //x5,x45
    display1.drawPixel(x-2,y+2,SSD1306_WHITE);

}

void DisplaySun(int x,int y){
  //x7,y55
  
  //x3,y59->x11,y51
  display1.drawLine(x-4,y+4,x+4,y-4,SSD1306_WHITE);
  //x3,y51->x11,y59
  display1.drawLine(x-4,y-4,x+4,y+4,SSD1306_WHITE);
  //x7,y50->x7,y60
  display1.drawLine(x,y-5,x,y+5,SSD1306_WHITE);
  //x2,y55->x12,y55
  display1.drawLine(x-5,y,x+5,y,SSD1306_WHITE);

  display1.fillCircle(x,y,3,SSD1306_WHITE);
  display1.fillCircle(x,y,2,SSD1306_BLACK);

}

void DisplayCloud(int x,int y){
  //x96,y5


  //x91,y10->x101,y10
  display1.drawLine(x-5,y+5,x+5,y+5,SSD1306_WHITE);
  //x91,y9->x91,y7
  display1.drawLine(x-5,y+4,x-5,y+2,SSD1306_WHITE);
  //x92,y5->x92,y6
  display1.drawLine(x-4,y,x-4,y+1,SSD1306_WHITE);
  //x93,y4->x93,y5
  display1.drawLine(x-3,y-1,x-3,y,SSD1306_WHITE);
  //x94,y4->x97,y4
  display1.drawLine(x-2,y-1,x+1,y-1,SSD1306_WHITE);
  //x98,y6->x98,y5
  display1.drawLine(x+2,y+1,x+2,y,SSD1306_WHITE);
  //x99,y6->x99,y7
  display1.drawLine(x+3,y+1,x+3,y+2,SSD1306_WHITE);
  //x100,y7->x101,y7
  display1.drawLine(x+4,y+2,x+5,y+2,SSD1306_WHITE);
  //x101,y9->x101,y8
  display1.drawLine(x+5,y+4,x+5,y+3,SSD1306_WHITE);
  //x93,y7->x93,y8
  display1.drawLine(x-3,y+2,x-3,y+3,SSD1306_WHITE);
  //x94,y6
  display1.drawPixel(x-2,y+1,SSD1306_WHITE);
}

void NextHourTherm(int x,int y){
    //96 17
    //therm
    display1.drawLine(x+1,y+1,x+1,y-4,SSD1306_WHITE);
    display1.drawLine(x+4,y+1,x+4,y-4,SSD1306_WHITE);
    display1.drawLine(x,y+2,x,y+4,SSD1306_WHITE);
    display1.drawLine(x+5,y+2,x+5,y+4,SSD1306_WHITE);
    display1.drawLine(x+1,y+5,x+4,y+5,SSD1306_WHITE);
    display1.drawLine(x+2, y-5,x+3,y-5, SSD1306_WHITE);
    display1.drawPixel(x+3, y-3, SSD1306_WHITE);
    display1.drawPixel(x+3, y-1, SSD1306_WHITE);
    display1.drawPixel(x+3, y+1,SSD1306_WHITE);
    //n
    //x91,y12->x91,y22
    display1.drawLine(x-5,y-5,x-5,y+5,SSD1306_WHITE);
    //x94,y13->x94,y22
    display1.drawLine(x-2,y-5,x-2,y+5,SSD1306_WHITE);
    //x92,y16->x92,y17;
    display1.drawLine(x-4,y-1,x-4,y,SSD1306_WHITE);
    //x93,y17->x93,y18;
    display1.drawLine(x-3,y,x-3,y+1,SSD1306_WHITE);

}

void DisplayCATM(int x,int y){
  //border
  //89,44
  //x89,y44->x127,y44
  display1.drawLine(x,y,x+38,y,SSD1306_WHITE);
  //x89,y45->x89,y63
  display1.drawLine(x,y+1,x,y+19,SSD1306_WHITE);
  //text
  display1.setTextSize(1);
  display1.setTextColor(SSD1306_WHITE);
  display1.setCursor(x+2,y+2);
  display1.print("CAT");  
  display1.setCursor(x+21,y+2);
  display1.drawPixel(x+19,y+8,SSD1306_WHITE);
  display1.print("M");  
  //119,50
  display1.setCursor(x+32,y+2);
  display1.print("1");
  //v
  display1.drawLine(x+28,y+6,x+28,y+7,SSD1306_WHITE);
  display1.drawLine(x+29,y+7,x+29,y+8,SSD1306_WHITE);
  display1.drawLine(x+30,y+6,x+30,y+7,SSD1306_WHITE);

  //by ioa
  display1.setCursor(x+2,y+12);
  display1.print("by ioa");
}

void DisplayCat(int x,int y){
  display1.drawBitmap(x, y, cat_bitmap, 47, 59, SSD1306_WHITE);
}

void DisplayText(int Size,int x,int y,String Text){
    display1.setTextColor(SSD1306_WHITE);

    display1.setTextSize(Size);
    display1.setCursor(x, y);
    display1.print(Text);
}

void AllDesigns(){
    DisplayClock(7,7);
    DisplayInTherm(7,19);
    DisplayOutTherm(7,31);
    DisplayMoon(7,43);
    DisplaySun(7,55);
    DisplayCloud(102,5);
    NextHourTherm(102,17);
    DisplayCATM(89,44);
    DisplayCat(47,3);

}

void FirstScreen(){
  AllDesigns();
  //Time(15 min intervals for now)
  parseWeather(weather,12,MainNr,MainString,MainData); // THE CHOOSENR IS BACKWARDS
  CurrHour=MainString.substring(0,2).toInt();
  DisplayText(1,15,4,MainString); //maybe use actual time, this is only the time for the temps which is not intuitive
  //Out Temp
  parseWeather(weather,32,MainNr,MainString,MainData);
  DisplayText(1,15,28,String(MainNr)+"C");
  //Sunset
  parseWeather(weather,34,MainNr,MainString,MainData);
  DisplayText(1,15,40,MainString);
  //Sunrise
  parseWeather(weather,24,MainNr,MainString,MainData);
  DisplayText(1,15,52,MainString);
  //NextHour
  parseWeather(weather,23,MainNr,MainString,MainData);
  if(CurrHour+1>23) //this is a temp fix since i dont yet have the setup for the next day aka getting weekly
    DisplayText(1,110,16,String(int(MainData[0]))+"C");
  else
    DisplayText(1,110,16,String(int(MainData[CurrHour+1]))+"C");
  
  //Precipitation L/m2
  parseWeather(weather,42,MainNr,MainString,MainData);
  DisplayText(1,110,4,String(MainNr)+"mm");

  //ClearPortion(15,16,37,23);
  //DisplayText(1,15,16,String(bmp.readTemperature()-1).substring(0,2)+"C");
}

void SecondScreen(){
  DisplayText(1,60,30,"Test");
}

void ScreenChooser(int x){
  if(x==1)
    FirstScreen();
  if(x==2)
    SecondScreen();

}

void setup(){
/*
  #if defined(NEOPIXEL_POWER)
    pinMode(NEOPIXEL_POWER, OUTPUT);
    digitalWrite(NEOPIXEL_POWER, HIGH);
  #endif
  pixels.begin(); 
  pixels.setBrightness(20);
*/
  // START CONNECT WIFI
    Serial.begin(115200);
    delay(1000);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    //WiFi.begin(ssid);
    Serial.println("\nConnecting");

    while(WiFi.status() != WL_CONNECTED){
        Serial.print(".");
        delay(100);
    }

    Serial.println("\nConnected to the WiFi network");
    Serial.print("Local ESP32 IP: ");
    Serial.println(WiFi.localIP());
  // END CONNECT WIFI
  
  Wire.begin();  // to remove the args, just for the simulation
  delay(200);
  
  // Initialize Display
  Serial.println("Initializing Display at 0x3C...");
  if (display1.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    display1.clearDisplay();
    Serial.println("✓ Display OK");
    delay(2000);
  } 
  else {
    Serial.println("✗ Display failed");
  }
    //Init bmp sensor;
    status = bmp.begin(0x76);
    bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500);
    //button
    pinMode(A0,INPUT_PULLUP);
    
    weather = httpGETRequest(weatherserver);
    ScreenChooser(curr_screen);
    display1.display();
}

void loop() {
  if(WiFi.status() == WL_CONNECTED){
    if ((millis() - lastTime) > timerDelay) {
        weather = httpGETRequest(weatherserver);
        //11 - timezone abrev
        //12 - curr time
        //22 - curr interval
        //32 - curr temp
        //42 - curr precip
        //13 - hourly time
        //23 - hourly temp
        //33 - hourly precip
        //43 - hourly precip prob
        //14 - daily time
        //24 - daily sunrise
        //34 - daily sunset
        lastTime = millis();
    }
    //next screen
    buttonValue=digitalRead(A0);
    if(buttonValue==LOW && (millis() - debounceTime) > debounceDelay){
      curr_screen++;
      debounceTime=millis();
    }
    //looping screens
    if(max_screen_nr<curr_screen){
        curr_screen=1;
        Serial.println("reset");
    }
    display1.clearDisplay();
    if(curr_screen==1)
      DisplayText(1,15,16,String(bmp.readTemperature()-1).substring(0,2)+"C");
    ScreenChooser(curr_screen);
    display1.display();
    /*Serial.println(buttonValue);
    if(buttonValue==LOW){
      pixels.fill(0xFF0000);
      pixels.show(); 
    }
    else{
      pixels.fill(0x000000);
      pixels.show();
    }  //debug for button
    */
  }
  else {
    pixels.fill(0xFF0000);
    pixels.show();
  }
}

void parseWeather(String jsonString,int ChooseNr, int& ReturnNr, String& ReturnString,double date[]) {
  JSONVar root = JSON.parse(jsonString);

  if (JSON.typeof(root) == "undefined") {
    Serial.println("Parsing input failed!");
    return;
  }

  // if json doesnt work, try it as an array
  if(ChooseNr%10==1){
    String TimezoneAbrev = (const char*) root["timezone_abbreviation"][0];
    ReturnString=TimezoneAbrev;
    return;
  }
  else if(ChooseNr%10==2){
    ChooseNr/=10;
    if(ChooseNr%10==1){
      String time=(const char*) root["current"]["time"];
      String timeOnly = time.substring(11, 16);
      ReturnString=timeOnly;
      return;
    }
    else if(ChooseNr%10==2){
      String interval=(const char*) root["current"]["interval"];
      ReturnString=interval;
      return;
    }
    else if(ChooseNr%10==3){
      int temper=int(root["current"]["temperature_2m"]);
      ReturnNr=temper;
      return;
    }
    else if(ChooseNr%10==4){
      int precip=int(root["current"]["precipitation"]);
      ReturnNr=precip;
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
        return;
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
  else if(ChooseNr%10==4){
    ChooseNr/=10;
    if(ChooseNr%10==1){
      String time = (const char*) root["daily"]["time"][0];
      ReturnString=time;
      return;
    }
    if(ChooseNr%10==2){
      String sunrise = (const char*) root["daily"]["sunrise"][0];
      String sunriseTrunc = sunrise.substring(11, 16);
      ReturnString=sunriseTrunc;
      return;
    }
    else{
      String sunset = (const char*) root["daily"]["sunset"][0];
      String sunsetTrunc = sunset.substring(11, 16);
      ReturnString= sunsetTrunc;
      return;
    }
  }
}

String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;
    
  http.begin(client, weatherserver);
  
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
  http.end();

  return payload;
}

void ClearPortion(int x1,int y1,int x2,int y2){
  display1.fillRect(x1, y1, x2-x1, y2-y1, SSD1306_BLACK);
}
