#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FS.h> // For Filesystem

bool loadFromSpiffs(String path);
void cresedentails_received();


//ESP-WLAN Konfiguration
const char *ssid = "Gasanalyser Config";
const char *password = "12345678";

//ESP-Web-Server Konfiguration
ESP8266WebServer server(80);

void handleWebRequests()
{
  if (loadFromSpiffs(server.uri()))
    return;
  else
    Serial.println("Error loading SPIFF files");
}

//get index.html
void handleRoot()
{
  server.sendHeader("Location", "/index.html", true);
  server.send(302, "text/plane", "");
}

//get submit.html and call  cresedentails_received
void handleSubmit()
{
  cresedentails_received();
  server.sendHeader("Location", "/submit.html", true);
}

//get cresedentails got "get"
void cresedentails_received()
{
  Serial.print("ssid: ");
  Serial.println(server.arg("ssid"));

  Serial.print("password: ");
  Serial.println(server.arg("password"));
}

//initialise filesystem
void initialise_filesystem()
{
  SPIFFS.begin();
  Serial.println("Filesystem Initialised");
}

//connect to wifi
void connect_wifi()
{
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Connected to Wifi");
  Serial.println("IP-Addresse: ");
  Serial.println(WiFi.localIP());
}

void create_accesspoint()
{
  Serial.println("Setting up Accesspoint");
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  Serial.println("Accesspoint is set up");

  
}

//initialise webserver
void initialise_webserver()
{
  Serial.println("Setting up webserver");
  server.on("/", handleRoot);
  server.on("/get", handleSubmit);
  server.onNotFound(handleWebRequests);
  server.begin();
  Serial.println("Webserver is set up");
}

void setup()
{
  delay(1000);
  Serial.begin(9600);
  Serial.println();

  initialise_filesystem();
  //connect_wifi();
  create_accesspoint();
  initialise_webserver();


  IPAddress ip_adress = WiFi.softAPIP();
  Serial.print("Connect to: ");
  Serial.println(ip_adress);
  Serial.println();
}

void loop()
{
  server.handleClient();
}

//load files from spiffs
bool loadFromSpiffs(String path)
{
  Serial.print("Path: ");
  Serial.print(path);
  Serial.println("");
  String dataType = "text/plain";
  if (path.endsWith("/"))
    path += "index.htm";

  if (path.endsWith(".src"))
    path = path.substring(0, path.lastIndexOf("."));
  else if (path.endsWith(".html"))
    dataType = "text/html";
  else if (path.endsWith(".htm"))
    dataType = "text/html";
  else if (path.endsWith(".css"))
    dataType = "text/css";
  else if (path.endsWith(".png"))
    dataType = "image/png";

  File dataFile = SPIFFS.open(path.c_str(), "r");
  if (server.hasArg("download"))
    dataType = "application/octet-stream";
  if (server.streamFile(dataFile, dataType) != dataFile.size())
  {
  }

  dataFile.close();
  return true;
}