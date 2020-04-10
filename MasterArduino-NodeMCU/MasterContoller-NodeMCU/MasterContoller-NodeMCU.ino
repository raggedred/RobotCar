// This Arduino sketch is setup to connect to my local WiFi set it self up as a server. 
 // It waits to recieve a json packet via wifi websockets, then reads the command and passes that to Second Ardunio via two-wire I2C
 
 #include <SPI.h>
 #include <Wire.h>
 #include <ESP8266WiFi.h>
 #include <ArduinoJson.h>

  WiFiServer server(5001);            //What ever port your wish to use 
  
  const char* ssid     = "YOUR_WIFI_NAME";
  const char* password = "WIFI_PASSWORD";
  
  boolean alreadyConnected = false; // whether or not the client was connected previously

  int led = 2;   //Set LED pin number on NodeMCU
  
  const size_t capacity = JSON_OBJECT_SIZE(4) + 20;
  DynamicJsonDocument doc(capacity);
  const char* json = "{\"action\":0}";

 void setup() {
   pinMode(led, OUTPUT);
   //Initialize serial and wait for port to open:
   Serial.begin(9600); 
   
   Serial.println("MASTER I2C!");
   Wire.begin(1);   //Give number so data can be returned.
      
   Serial.println(F("WIFI Start!"));
   WiFi.begin(ssid, password);
   while (WiFi.status() != WL_CONNECTED) {
    delay(500);
     Serial.print(".");
  }
   Serial.println();
   // you're connected now, so print out the status:
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
   Serial.println("----------------------------------------");
   server.begin(); 
   Serial.println(F("WIFI Server Started!")); 
  }
 
 
 void loop() {
   
   WiFiClient client = server.available();   // Wait for a new client 
    if (client) {   // if a client is available
        
        lightOn();  // switch arduino light on
        
        String saveString = client.readStringUntil(10); // 10 equals the ASCII Code for line feed in byte, or at least there ends the information we want to interpret
                                                        // can use client.readStringUntil('\n'), this takes longer.
        
        deserializeJson(doc, saveString);  //convert JSON string to objects.
        
        int action = doc["action"]; //Get the object data into an integer.

        //Send the action to thee second Arduino via Two-Wire i2c.
        sendToWireSlave(9, action);
    
        client.println(1);  //Return something to the client to recognition or recipt or message. 
                            //This is done after the action so that the arduino is not overwhelmed by recieving data.
        client.flush(); 
        
        lightOff();// switch arduino light off

     } 
 }
 
 void lightOn() {
   digitalWrite(led, LOW);
 }
 
 void lightOff() {
   digitalWrite(led, HIGH);
 }

 void sendToWireSlave(int address, int action) {
      Wire.beginTransmission(address);
      Wire.write(action);    //Add more Wire.write to send addtional data items. Remembering to add the Wire.Read on the second Arduino
      Wire.endTransmission();
}
