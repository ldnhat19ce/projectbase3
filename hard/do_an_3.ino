#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
                          
#define FIREBASE_HOST "esp8266-f04a3-default-rtdb.firebaseio.com"                     //Your Firebase Project URL goes here without "http:" , "\" and "/"
#define FIREBASE_AUTH "nhNbC1VqHgdX0Ix4re5F76nrHxHChXjQK4aD0N2i" //Your Firebase Database Secret goes here

#define WIFI_SSID "VNPT_VIET"                                               //WiFi SSID to which you want NodeMCU to connect
#define WIFI_PASSWORD "0974921551"                                      //Password of your wifi network 
///0977964124
// Declare the Firebase Data object in the global scope
FirebaseData firebaseData;

// Declare global variable to store value
String val= "";
String fanStatus = "";
int ledPin = 13;
int fanPin = 12;
float fanTime = -1;
float lightTime = -1;
float currentTime = 0;
String serverName = "http://192.168.1.6:8080/APITime_war_exploded/api/time";


void setup() {

  Serial.begin(115200);  
  // Select the same baud rate if you want to see the datas on Serial Monitor
  pinMode(ledPin,OUTPUT);
  pinMode(fanPin, OUTPUT);

  Serial.println("Serial communication started\n\n");  
           
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD); //try to connect with wifi
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);


  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP());  //print local IP address
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);   // connect to firebase

  Firebase.reconnectWiFi(true);
  delay(1000);
}

void loop() { 
  WiFiClient client;
  HTTPClient http;
  
  http.begin(client, serverName.c_str());
      
  // Send HTTP GET request
  int httpResponseCode = http.GET();

  if (httpResponseCode>0) {
      String payload = http.getString();
      Serial.println("current_time_string: "+payload);
      currentTime = payload.toFloat();
      Serial.println(currentTime);
  }else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  handleLight();
  handleFan();

  delay(3000);
}

void handleLight(){
  

  if (Firebase.getFloat(firebaseData, "/light_time")) {                           // On successful Read operation, function returns 1  
      lightTime = firebaseData.floatData();
      if(lightTime == -1){
          if (Firebase.getString(firebaseData, "/light_state")) {                           // On successful Read operation, function returns 1  
        
            if (firebaseData.dataType() == "string") {                            // print read data if it is integer
        
              val = firebaseData.stringData();
              Serial.println(val);
            
              if(val == "turn_on_light"){
                digitalWrite(ledPin, HIGH);
              }else if(val == "turn_off_light"){
                digitalWrite(ledPin, LOW);
              }
            }
        }
      }else{
        if((lightTime - currentTime) < 0){
          Serial.println("light time over");
          Firebase.setFloat(firebaseData,"/light_time", -1);
          Firebase.setString(firebaseData,"/light_state", "turn_off_light");
           digitalWrite(ledPin, LOW);
        }else{
          digitalWrite(ledPin, HIGH);
        }
      }
  }else {
    Serial.println(firebaseData.errorReason());
  }
}

void handleFan(){
  if(Firebase.getFloat(firebaseData, "/fan_time")){
    fanTime = firebaseData.floatData();
    if(fanTime == -1){
        if(Firebase.getString(firebaseData, "/fan_state")){
          if(firebaseData.dataType() == "string"){
           fanStatus = firebaseData.stringData();
           Serial.println(fanStatus);
            Serial.println("\n fan not time."); 
           if(fanStatus == "turn_on_fan"){
            digitalWrite(fanPin, HIGH);
           }else if(fanStatus == "turn_off_fan"){
            digitalWrite(fanPin, LOW);
           }
        }
      }
    }else{
        if((fanTime - currentTime) < 0){
          Serial.println("fan time over");
          Firebase.setFloat(firebaseData,"/fan_time", -1);
          Firebase.setString(firebaseData,"/fan_state", "turn_off_fan");
          digitalWrite(fanPin, LOW);
        }else{
          digitalWrite(fanPin, HIGH);
        }
        
    }
  }
  
}
