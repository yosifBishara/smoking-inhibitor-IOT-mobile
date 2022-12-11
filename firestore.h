
#include <C:/Users/ali.abbas/AppData/Local/Arduino15/packages/esp32/hardware/esp32/2.0.3/libraries/WiFi/src/WiFi.h>


#include <C:/Users/ali.abbas/Documents/Arduino/libraries/Firebase-ESP-Client-main/src/Firebase_ESP_Client.h>

// Provide the token generation process info.
#include <C:/Users/ali.abbas/Documents/Arduino/libraries/Firebase-ESP-Client-main/src/addons/TokenHelper.h>

#include <time.h>
#include <stdlib.h>
#include <string.h>

/* 1. Define the WiFi credentials */
#define WIFI_SSID "TechPublic"
#define WIFI_PASSWORD "WIFI_PASSWORD"

/* 2. Define the API Key */
#define API_KEY "AIzaSyBAi7m7Ka3JY4-WBmYl5_xacsmJ2RQPJHo"

/* 3. Define the project ID */
#define FIREBASE_PROJECT_ID "iot-smoking-inhibitor"

/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "7ot@walla.co.il"
#define USER_PASSWORD "7ot7ot"

// Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;


// The Firestore payload upload callback function
//int fcsUploadCallback(CFS_UploadStatusInfo info){
//    if (info.status == fb_esp_cfs_upload_status_init)
//    {
//        Serial.printf("\nUploading data (%d)...\n", info.size);
//    }
//    else if (info.status == fb_esp_cfs_upload_status_upload)
//    {
//        Serial.printf("Uploaded %d%s\n", (int)info.progress, "%");
//    }
//    else if (info.status == fb_esp_cfs_upload_status_complete)
//    {
//        Serial.println("Upload completed ");
//    }
//    else if (info.status == fb_esp_cfs_upload_status_process_response)
//    {
//        Serial.print("Processing the response... ");
//    }
//    else if (info.status == fb_esp_cfs_upload_status_error)
//    {
//        Serial.printf("Upload failed, %s\n", info.errorMsg.c_str());
//    }
//    return 0;
//}

void firestore_setup(){

    WiFi.begin(WIFI_SSID);
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(300);
    }
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();

    Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

    /* Assign the api key (required) */
    config.api_key = API_KEY;

    /* Assign the user sign in credentials */
    auth.user.email = USER_EMAIL;
    auth.user.password = USER_PASSWORD;

    /* Assign the callback function for the long running token generation task */
    config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h
    
    // Limit the size of response payload to be collected in FirebaseData
    fbdo.setResponseSize(2048);

    Firebase.begin(&config, &auth);

    Firebase.reconnectWiFi(true);

    // For sending payload callback
    // config.cfs.upload_callback = fcsUploadCallback;
}

void firestore_write(double acc_avg, int heart_rate, int tvoc, int eco2, String dt, String tim, boolean cigarette)
{

    // Firebase.ready() should be called repeatedly to handle authentication tasks.

    // For the usage of FirebaseJson, see examples/FirebaseJson/BasicUsage/Create.ino
    FirebaseJson content;

    // We will create the nested document in the parent path "a0/b0/c0
    // a0 is the collection id, b0 is the document id in collection a0 and c0 is the collection id in the document b0.
    // and d? is the document id in the document collection id c0 which we will create.
     //get ESP32 MAC Address
    uint64_t chipid;
    chipid = ESP.getEfuseMac(); //The chip ID is essentially its MAC address(length: 6 bytes).
    Serial.printf("ESP32 Chip ID = %04X", (uint16_t)(chipid >> 32)); //print High 2 bytes
    Serial.printf("%08X\n", (uint32_t)chipid); //print Low 4bytes.
    char esp32Mac[18];
    snprintf(esp32Mac, 18, "%04X%08X", (uint16_t)(chipid >> 32), (uint32_t)chipid);
    Serial.print(esp32Mac);

    // If the document path contains space e.g. "a b c/d e f"
    // It should encode the space as %20 then the path will be "a%20b%20c/d%20e%20f"

    // double
    content.set("fields/Acceleration/doubleValue", acc_avg);

    // boolean
    content.set("fields/cigarette/booleanValue", cigarette);

    // integer
    content.set("fields/BPM/integerValue", heart_rate);
    content.set("fields/TVOC/integerValue", tvoc);
    content.set("fields/eCO2/integerValue", eco2);

    // string 
    content.set("fields/time/stringValue", tim.c_str());
    content.set("fields/date/stringValue", dt.c_str());
    Serial.print("Create a document... ");
	const String alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890_";
	while (1) {
		String sample_id = "";
		srand(time(NULL));
		for (int i=0 ; i < 20 ; i++) {
		  int randomIdx = rand() % (63);
		  sample_id += alphabet[randomIdx];
		}
		
		String documentPath = "Users/" + (String)(esp32Mac) + "/Samples/" + (String)(sample_id);
		if (Firebase.Firestore.createDocument(&fbdo, FIREBASE_PROJECT_ID, "" /* databaseId can be (default) or empty */, documentPath.c_str(), content.raw())){
			Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
			break;
		}
		else
			Serial.println(fbdo.errorReason());
    }
}

void firestore_save_next_wakeup(String next_wakeup_params, int sleep_duration) {
	FirebaseJson content;

    // We will create the nested document in the parent path "a0/b0/c0
    // a0 is the collection id, b0 is the document id in collection a0 and c0 is the collection id in the document b0.
    // and d? is the document id in the document collection id c0 which we will create.
     //get ESP32 MAC Address
    uint64_t chipid;
    chipid = ESP.getEfuseMac(); //The chip ID is essentially its MAC address(length: 6 bytes).
    Serial.printf("ESP32 Chip ID = %04X", (uint16_t)(chipid >> 32)); //print High 2 bytes
    Serial.printf("%08X\n", (uint32_t)chipid); //print Low 4bytes.
    char esp32Mac[18];
    snprintf(esp32Mac, 18, "%04X%08X", (uint16_t)(chipid >> 32), (uint32_t)chipid);
    Serial.print(esp32Mac);
    
    String documentPath = "Users/" + (String)(esp32Mac);

    // If the document path contains space e.g. "a b c/d e f"
    // It should encode the space as %20 then the path will be "a%20b%20c/d%20e%20f"

    // double
    content.set("fields/next_wakeup_param/stringValue", next_wakeup_params.c_str());
	content.set("fields/duration_seconds/integerValue", sleep_duration);
	if (Firebase.Firestore.patchDocument(&fbdo, FIREBASE_PROJECT_ID, "" /* databaseId can be (default) or empty */, documentPath.c_str(), content.raw(), ""))
        Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
    else
        Serial.println(fbdo.errorReason());
}

void get_duration_in_seconds_firestore(int* duration_seconds) {

  // If the document path contains space e.g. "a b c/d e f"
  // It should encode the space as %20 then the path will be "a%20b%20c/d%20e%20f"
  uint64_t chipid;
  chipid = ESP.getEfuseMac(); //The chip ID is essentially its MAC address(length: 6 bytes).
  Serial.printf("ESP32 Chip ID = %04X", (uint16_t)(chipid >> 32)); //print High 2 bytes
  Serial.printf("%08X\n", (uint32_t)chipid); //print Low 4bytes.
  char esp32Mac[18];
  snprintf(esp32Mac, 18, "%04X%08X", (uint16_t)(chipid >> 32), (uint32_t)chipid);
  Serial.println(esp32Mac);
  
  Serial.println("Get a document... ");
  
  String documentPath = "Users/" + (String)(esp32Mac);
  String mask = "";
  if (Firebase.Firestore.getDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath.c_str(), mask.c_str())) {
    Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
    FirebaseJson payload;
    payload.setJsonData(fbdo.payload().c_str());
    FirebaseJsonData jsonData;
    payload.get(jsonData, "fields/duration_seconds/integerValue", true);
    (*duration_seconds) = jsonData.intValue;
    Serial.println(jsonData.intValue);
  }
  else
    Serial.println(fbdo.errorReason());
}

void firestore_get_expected_wakeup_time(String *result) {
	uint64_t chipid;
  chipid = ESP.getEfuseMac(); //The chip ID is essentially its MAC address(length: 6 bytes).
  Serial.printf("ESP32 Chip ID = %04X", (uint16_t)(chipid >> 32)); //print High 2 bytes
  Serial.printf("%08X\n", (uint32_t)chipid); //print Low 4bytes.
  char esp32Mac[18];
  snprintf(esp32Mac, 18, "%04X%08X", (uint16_t)(chipid >> 32), (uint32_t)chipid);
  Serial.println(esp32Mac);
  
  Serial.println("Get a document... ");
  
  String documentPath = "Users/" + (String)(esp32Mac);
  String mask = "";
  if (Firebase.Firestore.getDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath.c_str(), mask.c_str())) {
    Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
    FirebaseJson payload;
    payload.setJsonData(fbdo.payload().c_str());
    FirebaseJsonData jsonData;
    payload.get(jsonData, "fields/next_wakeup_param/stringValue", true);
    (*result) = jsonData.stringValue;
    Serial.println(jsonData.stringValue);
  }
  else
    Serial.println(fbdo.errorReason());
}
