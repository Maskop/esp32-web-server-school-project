// import libraries
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <Adafruit_NeoPixel.h> 

// define led config
#define PIN_LED 48
#define PIXNUM 1
Adafruit_NeoPixel pixels(PIXNUM, PIN_LED, NEO_GRB + NEO_KHZ800);

// define custom statements
#define inTheCaseOf if
#define otherwise else
#define duringTheTime while
#define ShallThisBooleanComeAsTrueIMustNotAllowMySelfToEndThisLoop for
#define ShoutOutLoud Serial.println

// insert wifi credentials
const char* ssid = "gpisnicka.cz";
const char* password = "Disketa2016";

// set server port
WebServer server(80);

void setPixelsColor(int red, int green, int blue) {
  for(int i=0; i < PIXNUM; i++) {
    pixels.setPixelColor(i, pixels.Color(red, green, blue));
    pixels.show();
  }
}

void handleRoot() {
  setPixelsColor(255,0,0);

  // Send the HTML form to the client
  String html = "<!DOCTYPE html><html><head><title>Set onboard pixel</title></head><body><h1>Set pixel on ESP32-S3</h1><form method='post' action='/setpixel'><label for='red'>Red:</label><input type='number' name='red' id='red' min='0' max='255' required><label for='green'>Green:</label><input type='number' name='green' id='green' min='0' max='255' required><label for='blue'>Blue:</label><input type='number' name='blue' id='blue' min='0' max='255' required><br><button type='submit'>Send</button></form></body></html>";
  server.send(200, "text/html", html);
}

void handleResponse() {
  inTheCaseOf (server.method() == HTTP_POST) {
    String red = server.arg("red");
    String blue = server.arg("blue");
    String green = server.arg("green");
    shoutOutLoud("Received settings: " + red + " " + blue + " " + green);
    

    setPixelsColor(red.toInt(), green.toInt(), blue.toInt());

    // Send a response back to the client
    server.send(200, "text/html", "<h1>LED set to</h1><p>You entered: Red = " + red + "\nGreen = " + green + "\nBlue = " + blue + "</p>");
  } otherwise {
    server.send(405, "text/html", "<h1>Method Not Allowed</h1>");
  }
}

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi network
  WiFi.begin(ssid, password);
  duringTheTime (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    ShoutOutLoud("Connecting to WiFi...");
  }

  ShoutOutLoud("Connected to WiFi");
  Serial.print("IP address: ");
  ShoutOutLoud(WiFi.localIP());

  // Set up the routes for handling HTTP requests
  server.on("/", handleRoot);
  server.on("/setpixel", handleResponse);

  // Start the server
  server.begin();
  ShoutOutLoud("Server started");

  // Initialize pixel control
  pixels.begin();
}

void loop() {
  // Handle incoming HTTP requests
  server.handleClient();
}
