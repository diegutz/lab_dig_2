// Adafruit IO Digital Output Example
// Tutorial Link: https://learn.adafruit.com/adafruit-io-basics-digital-output
//
// Adafruit invests time and resources providing this open source code.
// Please support Adafruit and open source hardware by purchasing
// products from Adafruit!
//
// Written by Todd Treece for Adafruit Industries
// Copyright (c) 2016 Adafruit Industries
// Licensed under the MIT license.
//
// All text above must be included in any redistribution.

/************************** Configuration ***********************************/

// edit the config.h tab and enter your Adafruit IO credentials
// and any additional configuration needed for WiFi, cellular,
// or ethernet clients.
#include "config.h"

/************************ Example Starts Here *******************************/
#define RXD2 16
#define TXD2 17

// digital pin 5
int led_1;
int led_2;
char mensaje;
String valores;
// set up the 'digital' feed
AdafruitIO_Feed *led1Feed = io.feed("led1");
AdafruitIO_Feed *led2Feed = io.feed("led2");
AdafruitIO_Feed *valoresFeed = io.feed("valores");

void setup() {
  
  
  // start the serial connection
  Serial.begin(38400);
  Serial2.begin(38400, SERIAL_8N1, RXD2, TXD2);
  // wait for serial monitor to open
  while(! Serial);

  // connect to io.adafruit.com
  Serial.print("Connecting to Adafruit IO");
  io.connect();

  // set up a message handler for the 'digital' feed.
  // the handleMessage function (defined below)
  // will be called whenever a message is
  // received from adafruit io.
  led1Feed->onMessage(handleMessage);
  led2Feed->onMessage(handleMessage_2);

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());
  led1Feed->get();
  led2Feed->get();

}

void loop() {

  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();

  if(led_1 == 0 && led_2 == 0){
    Serial2.println('0');
  }
  if(led_1 == 0 && led_2 == 1){
    Serial2.println('1');
  }
  if(led_1 == 1 && led_2 == 0){
    Serial2.println('2');
  }
  if(led_1 == 1 && led_2 == 1){
    Serial2.println('3');
  }

  if (Serial2.available())
   {
      String data = Serial2.readStringUntil('\n');
      Serial.println(data);
   }
   valores = "gx  gy  gz";
  Serial.print("sending valores -> ");
  Serial.println(valores);
  valoresFeed->save(valores);
  delay(3000);
}

// this function is called whenever an 'digital' feed message
// is received from Adafruit IO. it was attached to
// the 'digital' feed in the setup() function above.
void handleMessage(AdafruitIO_Data *data) {

  Serial.print("received led1 <- ");
  Serial.println(data->value());
  if(data->toString() == "ON"){
    led_1 = 1;
  }
  if(data->toString() == "OFF"){
    led_1 = 0;
  }
  
}
void handleMessage_2(AdafruitIO_Data *data) {

  Serial.print("received led2 <- ");
  Serial.println(data->value());
  if(data->toString() == "ON"){
    led_2 = 1;
  }
  if(data->toString() == "OFF"){
    led_2 = 0;
  }
}
