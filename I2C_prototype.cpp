#include <Arduino.h>
#include <Wire.h>
///////////////////MASTER

float values[] = {3.0, 3.1, 3.2, 3.3, 3.4, 3.5, 3.6, 3.7, 3.8, 3.9, 4.0, 4.1, 4.2, 4.3, 4.4, 4.5, 4.6, 4.7, 4.8, 4.9, 5.0};
int index = 0;

float read(byte address) {
  byte data[4];
  Wire.beginTransmission(address);
  Wire.requestFrom(address, 4);

  int i = 0;
  while (Wire.available()) {
    data[i++] = Wire.read();
  }
  Wire.endTransmission();

  float value;
  memcpy(&value, data, sizeof(value));
  return value;
}

void send(byte address, float value) {
  byte data[4];
  memcpy(data, &value, sizeof(value)); 

  Wire.beginTransmission(address);
  Wire.write(data, sizeof(data)); 
  Wire.endTransmission();
}

void setup() {
  Wire.begin(); 
  Serial.begin(115200);
}

void loop() {
  float receivedValue = read(2);
  Serial.print("Received from slave: ");
  Serial.println(receivedValue);

  float valueToSend = values[index];
  send(2, valueToSend);

  index = (index + 1) % (sizeof(values) / sizeof(values[0]));
  delay(100);
}



///////////////////SLAVE


float values[] = {-1.0, -0.9, -0.8, -0.7, -0.6, -0.5, -0.4, -0.3, -0.2, -0.1, 0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0};
int index = 0;
float receivedValue = 0.0;

void setup() {
  Wire.begin(2); 
  Wire.onRequest(requestEvent); 
  Wire.onReceive(receiveEvent); 
  Serial.begin(115200); 
}

void loop() {
  Serial.print("Received from master: ");
  Serial.println(receivedValue); 
  delay(100); 
}

void requestEvent() {
  float value = values[index];
  Wire.write((byte*)&value, sizeof(value)); 
  index = (index + 1) % (sizeof(values) / sizeof(values[0])); 
}

void receiveEvent(int howMany) {
  byte data[4];
  int i = 0;
  while (Wire.available() && i < 4) {
    data[i++] = Wire.read(); 
  }
  memcpy(&receivedValue, data, sizeof(receivedValue));
}

/////////////////////////////////////////////
/////////MASTER
#include <Wire.h>
//float valueToSend = -1.54;
struct data
{
  float send_encoder;
  float send_loadcell;
};data A;
void setup() {
  Wire.begin(); 
  Serial.begin(115200);
}

void loop() {
  float asd=A.send_encoder;
  float sdf=A.send_loadcell;
  Wire.beginTransmission(8);
  Wire.write((byte*)&A, sizeof(data)); 
  Wire.endTransmission();
  
  //delay(100); 

  // Wire.requestFrom(8, sizeof(float)); 
  // if (Wire.available() == sizeof(float)) {
  //   float receivedValue;
  //   Wire.readBytes((char*)&receivedValue, sizeof(receivedValue));
  //   Serial.print("Received: ");
  //   Serial.println(receivedValue);
  // }


  //delay(100); 
}

//////////SLAVE

#include <Wire.h>
struct data
{
  float receive_encoder;
  float receive_loadcell;
};

float valueToSend = -2.45;
float receivedValue;

void setup() {
  Wire.begin(8);
  Wire.onReceive(receiveEvent); 
  //Wire.onRequest(requestEvent); 
  Serial.begin(115200);
}

void loop() {
}

// void receiveEvent(int howMany) {
//   if (Wire.available() == sizeof(float)) {
//     Wire.readBytes((char*)&receivedValue, sizeof(receivedValue));
//     Serial.print("Received: ");
//     Serial.println(receivedValue);
//   }
// }
void receiveEvent(int howMany) {
  if (howMany == sizeof(data)) {
    data A;
    Wire.readBytes((char*)&A, sizeof(data));
    float asd=A.receive_encoder;
    float sdf=A.receive_loadcell;
    Serial.println(asd);
    Serial.println(sdf);
  }
}

// void requestEvent() {
//   Wire.write((byte*)&valueToSend, sizeof(valueToSend)); 
// }
