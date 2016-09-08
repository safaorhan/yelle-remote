/**
 * yelle-remote.ino
 * 
 * An arduino program runs on ESP8266 (NodeMCU)
 * which controls an Arcelik Air Conditioner
 * sending proper IR signals.
 * 
 * This program pulls the last command from server once in a while and executes it converting to IR signals.
 * 
 * @author Ahmet Safa Orhan (http://github.com/safaorhan)
 */

#include <SPI.h>
#include <ESP8266WiFi.h>
#include <IRremoteESP8266.h>

#include "config.h"

// Hardware config
const int IR_LED = 5;

// Sensitive info
char ssid[] = WIFI_SSID;            // Network SSID (name)       [config.h]
char pass[] = WIFI_PASS;            // Network password          [config.h]
char server[] = SERVER_NAME;        // Server address            [config.h]
int server_port = SERVER_PORT;      // Server port               [config.h]

// App. logic
const unsigned long postingInterval = 2L * 1000L; // delay between updates, in milliseconds

// API Constants
const int CMD_OFF = 100;            // NO PARAMS
const int CMD_ON = 101;             // NO PARAMS
const int CMD_TEMP = 102;           // TWO DIGIT TEMPERATURE
const int CMD_HEAT = 103;           // PARAM_COOLING, PARAM_HEATING
const int PARAM_COOLING = 0;        // CMD_HEAT
const int PARAM_HEATING = 1;        // CMD_HEAT

unsigned long lastConnectionTime = 0;             // last time you connected to the server, in milliseconds

WiFiClient client;
IRsend irsend(IR_LED);


/*
 * ---- BEGINNING of air-conditioner IR code generation
 * 
 * It's generally for LG AC.
 * However basic functions work for Arcelik AC as well.
 * 
 * Thanks chaeplin (https://github.com/chaeplin) for this example:
 * https://github.com/z3t0/Arduino-IRremote/blob/master/examples/LGACSendDemo/LGACSendDemo.ino
 */

const int AC_TYPE  = 0;
// 0 : TOWER
// 1 : WALL

int AC_HEAT = 0;
// 0 : cooling
// 1 : heating

int AC_POWER_ON    = 0;
// 0 : off
// 1 : on

int AC_AIR_ACLEAN  = 0;
// 0 : off
// 1 : on --> power on

int AC_TEMPERATURE = 27;
// temperature : 18 ~ 30

int AC_FLOW        = 1;
// 0 : low
// 1 : mid
// 2 : high
// if AC_TYPE =1, 3 : change
//


const int AC_FLOW_TOWER[3] = {0, 4, 6};
const int AC_FLOW_WALL[4]  = {0, 2, 4, 5};

unsigned long AC_CODE_TO_SEND;

void ac_send_code(unsigned long code)
{
  Serial.print("code to send : ");
  Serial.print(code, BIN);
  Serial.print(" : ");
  Serial.println(code, HEX);

  irsend.sendLG(code, 28);
}


int lastTemperature = 24;

void ac_activate() {
  ac_activate(lastTemperature, 1);
}

void ac_activate(int temperature, int air_flow)
{
  lastTemperature = temperature;

  int AC_MSBITS1 = 8;
  int AC_MSBITS2 = 8;
  int AC_MSBITS3 = 0;
  int AC_MSBITS4 ;
  if ( AC_HEAT == 1 ) {
    // heating
    AC_MSBITS4 = 4;
  } else {
    // cooling
    AC_MSBITS4 = 0;
  }
  int AC_MSBITS5 = temperature - 15;
  int AC_MSBITS6 ;

  if ( AC_TYPE == 0) {
    AC_MSBITS6 = AC_FLOW_TOWER[air_flow];
  } else {
    AC_MSBITS6 = AC_FLOW_WALL[air_flow];
  }

  int AC_MSBITS7 = (AC_MSBITS3 + AC_MSBITS4 + AC_MSBITS5 + AC_MSBITS6) & B00001111;

  AC_CODE_TO_SEND =  AC_MSBITS1 << 4 ;
  AC_CODE_TO_SEND =  (AC_CODE_TO_SEND + AC_MSBITS2) << 4;
  AC_CODE_TO_SEND =  (AC_CODE_TO_SEND + AC_MSBITS3) << 4;
  AC_CODE_TO_SEND =  (AC_CODE_TO_SEND + AC_MSBITS4) << 4;
  AC_CODE_TO_SEND =  (AC_CODE_TO_SEND + AC_MSBITS5) << 4;
  AC_CODE_TO_SEND =  (AC_CODE_TO_SEND + AC_MSBITS6) << 4;
  AC_CODE_TO_SEND =  (AC_CODE_TO_SEND + AC_MSBITS7);

  ac_send_code(AC_CODE_TO_SEND);

  AC_POWER_ON = 1;
  AC_TEMPERATURE = temperature;
  AC_FLOW = air_flow;
}

void ac_change_air_swing(int air_swing)
{
  if ( AC_TYPE == 0) {
    if ( air_swing == 1) {
      AC_CODE_TO_SEND = 0x881316B;
    } else {
      AC_CODE_TO_SEND = 0x881317C;
    }
  } else {
    if ( air_swing == 1) {
      AC_CODE_TO_SEND = 0x8813149;
    } else {
      AC_CODE_TO_SEND = 0x881315A;
    }
  }

  ac_send_code(AC_CODE_TO_SEND);
}

void ac_power_down()
{
  AC_CODE_TO_SEND = 0x88C0051;

  ac_send_code(AC_CODE_TO_SEND);

  AC_POWER_ON = 0;
}

//This is not used (It's kept for future reference)

//void ac_air_clean(int air_clean)
//{
//  if ( air_clean == 1) {
//    AC_CODE_TO_SEND = 0x88C000C;
//  } else {
//    AC_CODE_TO_SEND = 0x88C0084;
//  }
//
//  ac_send_code(AC_CODE_TO_SEND);
//
//  AC_AIR_ACLEAN = air_clean;
//}


// ---- END of air-conditioner code generation part.

/**
 * Interprets a command in format TIMESTAMP_COMMAND_PARAM
 * and executes it.
 * 
 * @param s Command to be executed.
 */
void executeCommand(String s) {

  int index1 = s.indexOf('_');
  int index2 = s.lastIndexOf('_');

  String timestamp = s.substring(0, index1);
  int cmd = s.substring(index1 + 1, index2).toInt();
  int param = s.substring(index2 + 1).toInt();

  Serial.println("TIME: " + timestamp);
  Serial.println("CMD: " + cmd);
  Serial.println("PARAM: " + param);

  switch (cmd) {
    case CMD_OFF:
      ac_power_down();
      break;
    case CMD_ON:
      ac_activate();
      break;
    case CMD_TEMP:
      ac_activate(param, 1);
      break;
    case CMD_HEAT:
      AC_HEAT = param == PARAM_COOLING ? 0 : 1;
      ac_activate();
      break;
    default:
      Serial.println("INVALID CODE");
      digitalWrite(16, LOW); delay(800);
      break;
  }

  digitalWrite(16, LOW); delay(200);
  digitalWrite(16, HIGH);
}


/**
 * Makes a GET request to given endpoint. It appends param and value to the query.
 * 
 * Example: GET /endpoint?param=value HTTP/1.1
 */
void httpRequest(String endpoint, String param, String value) {
  // close any connection before send a new request.
  // This will free the socket on the WiFi shield
  client.stop();

  // if there's a successful connection:
  if (client.connect(server, server_port)) {
    Serial.print("*");
    // send the HTTP PUT request:
    client.print("GET " + endpoint);
    client.print("?" + param + "=" + value);
    client.println(" HTTP/1.1");
    client.println("User-Agent: ArduinoWiFi/1.1");
    client.println("Connection: close");
    client.println();

    // note the time that the connection was made:
    lastConnectionTime = millis();
  } else {
    // if you couldn't make a connection:
    Serial.println("connection failed");
    delay(1000);
  }
}

//This is for debugging purposes
void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}


// If postingInterval amount of time passed since lastConnection time,
// make an httpRequest to server.
void makeRequestIfItsTime() {
  if (millis() - lastConnectionTime > postingInterval) {
    httpRequest("/last-command", "test", "1");
  }
}


// If last command (response of the last GET request) is changed, execute it
String lastCommand = "";
void queryLastCommandIfAvailable() {
  String command = "";

  while (client.available()) {
    command = client.readStringUntil('\r');
    command.trim();
  }

  if (command != "" && command != lastCommand) {
    Serial.print("New command: ");
    Serial.println(command);
    lastCommand = command;
    executeCommand(command);
  }
}

//If WiFi connection is interrupted or ended somehow, try to reconnect.
void connectToWifiIfNeeded() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      delay(500);
    }
    // you're connected now, so print out the status:
    printWifiStatus();
  }
}

void setup()
{
  Serial.begin(115200);
  irsend.begin();
  pinMode(16, OUTPUT);
  delay(10);
}

void loop()
{
  connectToWifiIfNeeded();
  makeRequestIfItsTime();
  queryLastCommandIfAvailable();
}
