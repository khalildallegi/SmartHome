#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"
#define API_KEY "AIzaSyBSmsRSzyNy2l58LMspjdJDDafy1nQK320"
#define DATABASE_URL "https://esp82realtime-default-rtdb.firebaseio.com/"
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;
const int ledPin = 16;
void setup() {
     WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
    // it is a good practice to make sure your code sets wifi mode how you want it.

    // put your setup code here, to run once:
    Serial.begin(115200);
      pinMode(ledPin, OUTPUT); // Set LED pin as output
  digitalWrite(ledPin, LOW); 

    //WiFiManager, Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wm;

    // reset settings - wipe stored credentials for testing
    // these are stored by the esp library
     wm.resetSettings();

    // Automatically connect using saved credentials,
    // if connection fails, it starts an access point with the specified name ( "AutoConnectAP"),
    // if empty will auto generate SSID, if password is blank it will be anonymous AP (wm.autoConnect())
    // then goes into a blocking loop awaiting configuration and will return success result

    bool res;
    // res = wm.autoConnect(); // auto generated AP name from chipid
    // res = wm.autoConnect("AutoConnectAP"); // anonymous ap
    res = wm.autoConnect("AutoConnectAP","password"); // password protected ap

    if(!res) {
        Serial.println("Failed to connect");
        // ESP.restart();
    } 
    else {
        //if you get here you have connected to the WiFi    
        Serial.println("connected...yeey :)");
    }

}

void loop() {
      config.api_key = API_KEY;
    config.database_url = DATABASE_URL;
    
    if (Firebase.signUp(&config, &auth, "", "")){
      Serial.println("Firebase sign up successful");
    }
    else{
      Serial.printf("Firebase sign up failed: %s\n", config.signer.signupError.message.c_str());
    }

    Firebase.begin(&config, &auth);
    Firebase.reconnectWiFi(true);

    // Main logic to handle LED status
    while (true) {  
      if (Firebase.ready()){
        // Check for new LED status from Firebase
        if (Firebase.RTDB.getInt(&fbdo, "led/status")){
          int ledStatus = fbdo.intData();
          Serial.print("LED Status: ");
          Serial.println(ledStatus);
          if (ledStatus == 1){
            digitalWrite(ledPin, HIGH); // Turn LED on
          } else {
            digitalWrite(ledPin, LOW); // Turn LED off
          }
        } else {
          Serial.println("Failed to get LED status");
          Serial.println("REASON: " + fbdo.errorReason());
        }
      }
      delay(1000); // Add a delay to avoid flooding Firebase with requests
    }  
}
