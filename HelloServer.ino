#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Adafruit_NeoPixel.h>
#include "font5x7.h"

#define PIN_BUTTON   0
#define PIN_LED     14
#define NUM_LEDS    35

const char* ssid = "Red_Mountain_Makers";
const char* password = "luckyplanet397";

String scrolltext = " Hello World!";
uint16_t scrollwait = 75;
uint32_t scrollcolor = 0x002000;
byte scrollindex = 0; // scroll character index

ESP8266WebServer server(80);

// Neopixel object
Adafruit_NeoPixel leds = Adafruit_NeoPixel(NUM_LEDS, PIN_LED, NEO_GRB + NEO_KHZ800);

byte demo_index = 0;
byte brightness = 48;       // default brightness: 48
uint32_t pixels[NUM_LEDS];  // pixel buffer. this buffer allows you to set arbitrary
// brightness without destroying the original color values

const int led = 13;

// Forward declare functions
String get_ap_name();
//void button_handler();
void on_status();
void on_change_color();
void on_homepage();
void show_leds();

void show_leds() {
  uint32_t r, g, b;
  uint32_t c;
  for (byte i = 0; i < NUM_LEDS; i++) {
    r = (pixels[i] >> 16) & 0xFF;
    g = (pixels[i] >> 8) & 0xFF;
    b = (pixels[i]) & 0xFF;
    r = r * brightness / 255;
    g = g * brightness / 255;
    b = b * brightness / 255;
    c = (r << 16) + (g << 8) + b;
    leds.setPixelColor(i, c);
  }
  leds.show();
}

void handleNotFound(){
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

void setup(void){
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");
    // Set pin mode
  pinMode(PIN_BUTTON, INPUT);
  pinMode(PIN_LED,    OUTPUT);


// Initialize LEDs
  leds.begin();
  leds.show();

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/",   on_homepage);
  server.on("/js", on_status);
  server.on("/cc", on_change_color);

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");

}

void loop(void){
      server.handleClient();
}


/* ----------------
    WebServer Demo
   ---------------- */

#include "html.h"
void on_homepage() {
  String html = FPSTR(index_html);
  server.send(200, "text/html", html);
}

// this returns device variables in JSON, e.g.
// {"pixels":xxxx,"blink":1}
void on_status() {
  String html = "";
  html += "{\"brightness\":";
  html += brightness;
  html += "{\"color\":";
  html += scrollcolor;
  html += "{\"pixel\":";
//  html += val;
  html += "}";
  server.send(200, "text/html", html);
}

void on_change_color() {
  uint16_t i;
  if (server.hasArg("pixels")) {
    String val = server.arg("pixels");
    for (i = 0; i < NUM_LEDS; i++) {
      // every pixel color is 6 bytes storing the hex value
      // pixels are specified in row-major order
      // here we need to flip it to column-major order to
      // match the physical connection of the leds
      byte r = i / 5, c = i % 5;
      pixels[c * 7 + r] = strtol(val.substring(i * 6, i * 6 + 6).c_str(), NULL, 16);
    }
    scrolltext = "";
  }
  if (server.hasArg("clear")) {
    for (i = 0; i < NUM_LEDS; i++) {
      pixels[i] = 0;
    }
    scrolltext = "";
  }
  if (server.hasArg("brightness")) {
    brightness = server.arg("brightness").toInt();
    scrolltext = "";
  }
    if (server.hasArg("color")) {
      scrollcolor = strtol(server.arg("color").c_str(), NULL, 16);
      server.send(200, "text/html", "{\"result\":1}");
    }
  }
//  if (scrolltext.length() == 0){
//    show_leds();
//  server.send(200, "text/html", "{\"result\":1}");
//  }

