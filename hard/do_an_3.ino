#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
                          
#define FIREBASE_HOST "esp8266-f04a3-default-rtdb.firebaseio.com"                     //Your Firebase Project URL goes here without "http:" , "\" and "/"
#define FIREBASE_AUTH "nhNbC1VqHgdX0Ix4re5F76nrHxHChXjQK4aD0N2i" //Your Firebase Database Secret goes here

#define WIFI_SSID "Cafe Clyna"                                               //WiFi SSID to which you want NodeMCU to connect
#define WIFI_PASSWORD "0977964124"                                      //Password of your wifi network 
 
  

// Declare the Firebase Data object in the global scope
FirebaseData firebaseData;

// Declare global variable to store value
String val= "";
String fanStatus = "";
int ledPin = 13;
int fanPin = 12;


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

// Firebase Error Handling And Writing Data At Specifed Path************************************************

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
  }else {
    Serial.println(firebaseData.errorReason());
  }

  if(Firebase.getString(firebaseData, "/fan_state")){
      if(firebaseData.dataType() == "string"){
         fanStatus = firebaseData.stringData();
         Serial.println(fanStatus);
          Serial.println("\n Change value at firebase console to see changes here."); 
         if(fanStatus == "turn_on_fan"){
          digitalWrite(fanPin, HIGH);
         }else if(fanStatus == "turn_off_fan"){
          digitalWrite(fanPin, LOW);
         }
      }
  }

}
