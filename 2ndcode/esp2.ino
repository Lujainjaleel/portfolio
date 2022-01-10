////////////////////////////////////////////////////////////////
// Task 1
// NodeMCU (ESP8266) Resource Manager
//Lujain Jaleel  Name / Date 26/06/21
////////////////////////////////////////////////////////////////

#include <Wire.h>
#include <ESP8266WiFi.h> // Include the Wi-Fi library
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

const char *ssid = "Lujain's AP 2021"; // Your chosen access point name
const char *password = "password9"; // Your password


int T; 
 char tilt;

// This is the 'root'/'index page. This can *typically* be 
// accessed at http://192.168.4.1.
const char INDEX_HTML[] = // PLACE YOUR index.html STRING HERE
"<!DOCTYPE html>"
"<html>"
"<head>"
"<title>Red Web Page</title>"
"</head>"
"<body style=\"background-color:red;\">"
"<h1>Press a button</h1>"
"<form action=\"/\">"
"<input type=\"submit\" value=\"Red\">"
"</form>"
"<form action=\"/page1\">"
"<input type=\"submit\" value=\"Green\">"
"</form>"
"<form action=\"/page2\">"
"<input type=\"submit\" value=\"Blue\">"
"</form>"
"</body>"
"</html>";

// webpages
char page_to_send[2048];
const char PAGE1_HTML[] = //"page"
"<!DOCTYPE html>"
"<html>"
"<head>"
"<title>Red Web Page</title>"
"</head>"
"<body style=\"background-color:turquoise;\">"
"<h1>Press a button</h1>"
"<form action=\"/\">"
"<META HTTP-EQUIV=\"refresh\" CONTENT=\"5\">"
"<form action=\"/tle\">"
  "<input type=\"submit\" value=\"Traffic lights equal priority\">" 
"</form>"

"<form action=\"/tl1\">"
  "<input type=\"submit\" value=\"Traffic lights set 1 priority\">"
"</form>"

"<form action=\"/tl2\">"
  "<input type=\"submit\" value=\"Traffic lights set 2 priority\">"
"</form>"

"<form action=\"/maint\">"
  "<input type=\"submit\" value=\"Maintanence\">"
"</form>" 

"<form action=\"/trioff\">"
  "<input type=\"submit\" value=\"Tri colour LED off\">"
"</form>"

"<form action=\"/triamb\">"
  "<input type=\"submit\" value=\"Amber\">"
"</form>"


"<form action=\"/triblue\">"
 "<input type=\"submit\" value=\"Blue\">"
"</form>"


"<form action=\"/trigreen\">"
  "<input type=\"submit\" value=\"Green\">"
"</form>"
 

"<form action=\"/trired\">"
  "<input type=\"submit\" value=\"Red\">"
"</form>"


"<form action=\"/fruit\">"
  "<input type=\"submit\" value=\"Fruit machine\">"
"</form>"


"<form action=\"/status\">"
  "<input type=\"submit\" value=\"what is the statusss\">"
"</form>";
//"<p>T=%d&#8451;</p>"
//"</body>"
//"</html>";
//

const char ERROR_HTML[] =
"<!DOCTYPE html>"
"<html>"
"<head>"
"<title>404 Error</title>"
"</head>"
"<body>"
"<h1>404 Error</h1>"
"<p>Unsupported webpage</p>"
"</body>"
"</html>";


// Handshaking lines
#define GRANTED   D2
#define DEMAND    D1

 int count = 0;
// Using the built-in LED on the ESP8266.
#define LED       D4   
#define LED_ON    digitalWrite(LED, LOW);
#define LED_OFF   digitalWrite(LED, HIGH);

ESP8266WebServer server(80); // PORT 80 for HTTP
int status_code;


void sendWebpage()
{
  char html[2048];
  char temp[80];

  strcpy(html, PAGE1_HTML);

  sprintf(temp, "<p>T=%d&#8451;</p>", T);
  strcat(html, temp);
  sprintf(temp, "<p>tilt=%c;</p>", tilt);
  strcat(html, temp);
  strcat(html, "</body>");  
  strcat(html, "</html>");  

  server.send(200, "text/html", html);
}

//void sendWebpagetilt()
//{
//  char html[2048];
//  char  temp[80];
//
//  strcpy(html, PAGE1_HTML);
//
//  sprintf(temp, "<p>tilt=%c;</p>", tilt);
//  strcat(html, temp);
//  strcat(html, "</body>");  
//  strcat(html, "</html>");  
//
//  server.send(200, "text/html", html);
//}

//


//

// This function is called whenever the root Webpage
// is required. This displays the Webpage
void handleRoot()
{
  //sprintf(page_to_send, PAGE1_HTML,T);
  //server.send(200, "text/html", page_to_send);
  sendWebpage();
 // sendWebpagetilt();
  status_code = 0x61;
}

// This function is called whenever Page1 Webpage
// is required. This displays the Webpage
void handlePage1()
{
  //server.send(200, "text/html", PAGE1_HTML);
  sendWebpage();
 // sendWebpagetilt();
  status_code = 0x62; 
}

// This function is called whenever Page2 Webpage
// is required. This displays the Webpage
void handlePage2()
{
  sendWebpage();
 // sendWebpagetilt();
  server.send(200, "text/html", PAGE1_HTML);
  status_code = 0x63;
}


void handleTLequal()
{
  sendWebpage();
 // sendWebpagetilt();
  server.send(200, "text/html", PAGE1_HTML);
  status_code = 0x61; 
}

void handleTLset1()
{
  sendWebpage();
//  sendWebpagetilt();
  server.send(200, "text/html", PAGE1_HTML);
  status_code = 0x62; 
}


void handleTLset2()
{
  sendWebpage();
//  sendWebpagetilt();
  server.send(200, "text/html", PAGE1_HTML);
  status_code = 0x63; 
}

void handleTLmaintanence()
{
  sendWebpage();
//  sendWebpagetilt();
  server.send(200, "text/html", PAGE1_HTML);
  status_code = 0x64; 
}

void handleTLtriLEDOFF()
{
  sendWebpage();
 // sendWebpagetilt();
  server.send(200, "text/html", PAGE1_HTML);
  status_code = 0x65; 
}

void handleTLamber()
{
  sendWebpage();
//  sendWebpagetilt();
  server.send(200, "text/html", PAGE1_HTML);
  status_code = 0x66; 
}

void handleTLblue()
{
  sendWebpage();
 // sendWebpagetilt();
  server.send(200, "text/html", PAGE1_HTML);
  status_code = 0x67; 
}

void handleTLgreen()
{
  sendWebpage();
//  sendWebpagetilt();
  server.send(200, "text/html", PAGE1_HTML);
  status_code = 0x68; 
}

void handleTLred()
{
  sendWebpage();
//  sendWebpagetilt();
  server.send(200, "text/html", PAGE1_HTML);
  status_code = 0x69; 
}

void handleTLfruitmachine()
{
  sendWebpage();
//  sendWebpagetilt();
  server.send(200, "text/html", PAGE1_HTML);
  status_code = 0x6A; 
}

void handleTLgetstatus()
{
  sendWebpage();
//  sendWebpagetilt();
  server.send(200, "text/html", PAGE1_HTML);
  status_code = 0x6B; 
}

//
//void handletemp()
//{
//    char html[2048];
//    char temp[80];
//
//    // Copy the string INDEX_HTML into the string html
//    strcpy(html, PAGE1_HTML);
//
//    // Create a formatted string and append it
//    sprintf(temp, "<p>T=%d</p>", T);
//    strcat(html, temp);     
//
//    // Create another formatted string an append it
//   // sprintf(temp, "<p>Count squared=%d</p>", count * count);
//    strcat(html, temp);     
//
//    // Append the Webpage terminators
//    strcat(html, "</body>");
//    strcat(html, "</html>");
//
//   // count++;    // Increment the "hit-counter"
//  
//    server.send(200, "text/html", PAGE1_HTML);
//}





// This function is called whenever an unsupported
// Webpage is accessed. This displays the error Webpage
void handleNotFound()
{
  server.send(404, "text/html", ERROR_HTML);
  status_code = 0x64;
}




#define NoTriggerNotGranted  0   // idle state
#define TriggerNotGranted    1   // trigger detected, 
#define TriggerIamMaster     2   // we are master after a successful claim
#define NoTriggerGranted     3   // we do not want to be a master any more but
                                 // the GRANTED line is still active
int state;
bool trigger;                                 
                                 

// Module clock initializers
bool init_module1_clock;

//==============================================================
// granted()
//
// This function checks the status of the GRANTED line.
// If this line is low, it means the resource has been 
// granted and the function returns true.
// Otherwise, the resource has not been granted and the
// function returns false.
//==============================================================
bool granted()
{
  return (!digitalRead(GRANTED));
}

//==============================================================
// leaveHigh()
//
// This function implements a safe method of asserting a 
// logic 1. The required pin is passed as a parameter.
//==============================================================
void leaveHigh (unsigned char pin) {
  pinMode(pin,INPUT_PULLUP);
}

//==============================================================
// pullLow()
//
// This function implements a safe method of asserting a 
// logic 0. The required pin is passed as a parameter.
//==============================================================
void pullLow (unsigned char pin) {
  digitalWrite(pin, LOW);
  pinMode(pin, OUTPUT);
}

void setup() {

//
Serial.begin(115200);
  WiFi.softAP(ssid, password);
  Serial.println("");
  Serial.println(""); Serial.print("Connected to ");
  Serial.println(ssid); Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());
 
  server.on("/", handleRoot);
  server.on("/page1", handlePage1);
  server.on("/page2", handlePage2);
  
  server.on("/tle", handleTLequal);
  server.on("/tl1", handleTLset1);
  server.on("/tl2", handleTLset2);
  server.on("/maint", handleTLmaintanence);
  server.on("/trioff", handleTLtriLEDOFF); 
  server.on("/triamb", handleTLamber);
  server.on("/triblue", handleTLblue);
  server.on("/trigreen", handleTLgreen);
  server.on("/trired", handleTLred);
  server.on("/fruit",handleTLfruitmachine);
  server.on("/status", handleTLgetstatus);
  
  server.onNotFound(handleNotFound);  
  server.begin();
 
  Serial.print("Connect to http://");
  Serial.println(WiFi.softAPIP());

  status_code = 0;
  pinMode(LED, OUTPUT);
  LED_OFF;
//

  pinMode(LED, OUTPUT);
  leaveHigh(DEMAND);
  pinMode(GRANTED, INPUT_PULLUP);

  // Initialize state and trigger to sensible values.
  state = NoTriggerNotGranted;
  trigger = false;
  
  
  Wire.begin(D6, D7); /* join i2c bus with SDA=D6 and SCL=D7 of NodeMCU */  

  // Start the 17ms timer
  init_module1_clock = true;  
}

void loop() {
  static boolean  firstTime = true;
  int             AcX, AcY, AcZ;
  
  // This is the code for the Resource Manager. This has the highest
  // priority and must run every time the loop() function is executed.
  switch(state)
  {
  case NoTriggerNotGranted:
    leaveHigh(DEMAND);
    LED_OFF;
    if (!trigger)
    {
      state = NoTriggerNotGranted;
    }
    else
    {
      state = TriggerNotGranted;
    }
    break;

  case TriggerNotGranted:
    pullLow(DEMAND);
    LED_OFF;
    if (!trigger)
    {
      state = NoTriggerNotGranted;
    }
    else if (granted())
    {
      state = TriggerIamMaster;
    }
    else
    {
      state = TriggerNotGranted;
    }
    break;

  case TriggerIamMaster:
    pullLow(DEMAND);
    LED_ON;
    if (trigger & granted())
    {
      state = TriggerIamMaster;
    }
    else
    {
      state = NoTriggerNotGranted;
    }
    break;

  case NoTriggerGranted:
    break;
  }

  // This will implement an FSM containing 4 states.
  // State 0: Set a delay time of 17 ms, then move immediately to state 1
  // State 1: Stay here until the delay time has elapsed, 
  //          activate the trigger, then move to state 2 
  // State 2: Stay here until the Resource Manager reaches the IamMaster
  //          state. Then move to state 3.
  // State 3: Read the orientation data from the Nano, disable the trigger, 
  //          output the data to the serial port, and return back to state 0.
  { // module 1
    static unsigned long module_time, module_delay;
    static unsigned long timeout;
    static bool module_doStep;
    static unsigned char module_i; // state variable for module 1

    if (init_module1_clock) {
      module_delay = 1;
      module_time=millis();
      module_doStep=false;
      init_module1_clock=false;
      module_i=0;
    }
    else {
      unsigned long m;
      m=millis();
      if ((long)(m - module_time) >= module_delay) {
        module_time = m; module_doStep = true;
      }
      else module_doStep = false;
    }

    if (module_doStep) {
      // Act on the current state (module_i).
      switch(module_i)
      {
      case 0:
        // Acquire the current time
        timeout = millis();
        module_i = 1;
        break;

      case 1:
        // Stay in State 1 until the 17ms tumeout has elapsed
        if ((long)(millis() - timeout) >= 17)
        {
          module_i = 2;
          trigger = true;
        }
        break;

      case 2:
        // Stay in State 2 until Resource Manager ends up in
        // the IamMaster state
        if (state == TriggerIamMaster)
        {
          module_i = 3;
        }
        break;
      
      case 3:
        // Now master, it is safe to access the I2C bus.
        // Read the data from the Arduino Nano.
        Wire.requestFrom(8, 2); /* request & read data of size 1 from slave */  // two because two piece of data. tilt and temp
      //  int T ;       
        T = Wire.read();
        tilt = Wire.read();
        Serial.println(T);
        Serial.println(tilt);
        
        while(Wire.available()){
          char c = Wire.read(); // do I change char  ??
          //Serial.println(c);
        }
 
        // Now release the resouce and go back to the start
        trigger = false;
        module_i = 0;

      default:
        module_i = 0;
      }
    }
  } 
  server.handleClient();      // something about  trigger true  and iammaster

  if (status_code != 0)
  {
    Serial.print("Jumped to ");
    Serial.println(status_code, HEX);
  

    trigger = true;

     if (state == TriggerIamMaster)
     {
      Wire.beginTransmission(8);
      Wire.write(status_code);
      Wire.endTransmission();    

      //Request 1 byte
      // Read 1 byte
      
      status_code = 0;
      trigger = false;
      }

 }
}
