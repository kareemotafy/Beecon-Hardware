#include "firebase.h"
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

FirebaseAuth auth;
FirebaseConfig config;
String PROJECT_ID;

void wifi_connect(const char *ssid, const char *password)
{
  WiFi.begin(ssid, password);
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
}

void disableWiFi()
{
  WiFi.disconnect(true); // Disconnect WiFi and release resources
  WiFi.mode(WIFI_OFF);   // Turn off WiFi module
  Serial.println("WiFi disabled");
}

void enableWiFi(const char *ssid, const char *password)
{
  WiFi.mode(WIFI_STA);        // Enable WiFi module
  WiFi.begin(ssid, password); // Reconnect to WiFi
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Reconnecting to WiFi...");
  }
  Serial.println("WiFi reconnected!");
}

void firebase_init(const char *apiKey, const char *dbUrl, const char *projectId)
{
  config.api_key = apiKey;
  config.database_url = dbUrl;
  PROJECT_ID = projectId;

  bool signupOK = false;

  if (Firebase.signUp(&config, &auth, "", ""))
  {
    Serial.println("Firebase sign up successful");
    signupOK = true;
  }
  else
  {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  config.token_status_callback = tokenStatusCallback;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

// Generic function to store sensor data in Firebase
void store_sensor_data(const char *sensorName, float sensorValue)
{
  FirebaseData fbdo;

  // Create a JSON object to store the sensor data
  FirebaseJson sensorNode;
  sensorNode.set("sensorData", sensorValue);
  sensorNode.set("Ts/.sv", "timestamp");

  // Check if the sensor node is pushed to Firebase
  if (Firebase.RTDB.setJSON(&fbdo, String("/") + sensorName, &sensorNode))
  {
    Serial.print(sensorName);
    Serial.println(" pushed to Firebase");
  }
  else
  {
    Serial.println(fbdo.errorReason());
  }
}

void store_camera_data(const char *sensorName, String sensorValue)
{
  FirebaseData fbdo;

  // Create a JSON object to store the sensor data
  FirebaseJson sensorNode;
  sensorNode.set("sensorData", sensorValue);
  sensorNode.set("Ts/.sv", "timestamp");

  // Check if the sensor node is pushed to Firebase
  if (Firebase.RTDB.setJSON(&fbdo, String("/") + sensorName, &sensorNode))
  {
    Serial.print(sensorName);
    Serial.println(" pushed to Firebase");
  }
  else
  {
    Serial.println(fbdo.errorReason());
  }
}

String generateRandomString(size_t length)
{
  const String charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
  String randomString;

  for (size_t i = 0; i < length; ++i)
  {
    randomString += charset[random(0, charset.length())];
  }

  return randomString;
}

void firestoreDataUpdate(const char *sensorName, float sensorValue)
{
  FirebaseData fbdo;

  // Generate a random id for each sensor reading
  String id = generateRandomString(16);

  // The dyamic array of write object firebase_firestore_document_write_t.
  std::vector<struct firebase_firestore_document_write_t> writes;

  // A write object that will be written to the document.
  struct firebase_firestore_document_write_t update_write;

  // Set the write object write operation type.
  update_write.type = firebase_firestore_document_write_type_update;

  // Set the document content to write (transform)
  FirebaseJson content;
  String documentPath = "sensors/" + String(sensorName);

  content.set("fields/" + id + "/mapValue/fields/value/doubleValue", String(sensorValue).c_str());

  // Set the update document content
  update_write.update_document_content = content.raw();
  update_write.update_masks = id;

  // Set the update document path
  update_write.update_document_path = documentPath.c_str();

  // Add a write object to a write array.
  writes.push_back(update_write);

  // A write object that will be written to the document.
  struct firebase_firestore_document_write_t transform_write;

  // Set the write object write operation type.
  transform_write.type = firebase_firestore_document_write_type_transform;

  // Set the document path of document to write (transform)
  transform_write.document_transform.transform_document_path = documentPath;

  // Set a transformation of a field of the document.
  struct firebase_firestore_document_write_field_transforms_t field_transforms;

  // Set field path to write.
  field_transforms.fieldPath = id + ".timestamp";

  // Set the transformation type.
  field_transforms.transform_type = firebase_firestore_transform_type_set_to_server_value;

  // Set the transformation content, server value for this case.
  field_transforms.transform_content = "REQUEST_TIME"; // set timestamp to timestamp field

  // Add a field transformation object to a write object.
  transform_write.document_transform.field_transforms.push_back(field_transforms);

  // Add a write object to a write array.
  writes.push_back(transform_write);

  if (Firebase.Firestore.commitDocument(&fbdo, PROJECT_ID, "", writes, ""))
    Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
  else
    Serial.println(fbdo.errorReason());
}
