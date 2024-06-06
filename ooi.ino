//libraries
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

//definitions
#define OLED_WIDTH 128
#define OLED_HEIGHT 32

//constants
const char* ssid = "your ssid";
const char* password = "your password";

//declare oled object
Adafruit_SSD1306 oled(OLED_WIDTH, OLED_HEIGHT, &Wire, -1);

//declare class 
ESP8266WebServer server(80);

//html form 
const char* htmlForm = R"rawliteral(
  <!DOCTYPE HTML>
  <HTML>
  <body>
    <h2>Enter the message to display</h2>
    <form action="/submit" method="POST">
     Enter Message: <input type = "text" name="message">
     Enter Name: <input type = "text" name="name">
     <input type="submit" value="submit">
    </form?
  </body>
  </HTML>
)rawliteral";

//send the html page from above
void handleRoot(){
  server.send(200,"text/html",htmlForm);
}

//handle the form submit
void handleformsubmit(){
  if(server.hasArg("message")){
    String message = server.arg("message");
    String name = server.arg("name");
    oled.clearDisplay();
    oled.setCursor(0,10);
    oled.println("message from " + name + " : "+message);
    oled.display();
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  //initialise the oled
  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3c)){
    Serial.println("allocation failed");
    while (true);
  }
  delay(2000); //ensure oled is initialised
  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setTextColor(WHITE);
  oled.setCursor(0,10);
  //oled.println("test");
  //oled.display();

  //setup the web sever
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.println("either connecting to wifi or something is broken");
  }
  Serial.println("connected!!!");
  Serial.println(WiFi.localIP());
  server.on("/", HTTP_GET, handleRoot); //if client connects send the html form
  server.on("/submit",HTTP_POST, handleformsubmit); //if user submits get the data
  server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();
}
