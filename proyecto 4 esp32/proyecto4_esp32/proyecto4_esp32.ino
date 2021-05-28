/* Alumno: Diego Gonzalez López
 * Carnet: 15700 
 * proyecto 4 Webserver
 * Documento: proyecto 4 esp32 programación
 * 
 * Creador original del programa: Rui Santos
 * 
  Complete project details at https://randomnerdtutorials.com  
  
  Modificado para su uso por Diego Gonzalez
 */

#include <WiFi.h>

// Replace with your network credentials
const char* ssid = "Snicker";                 //ingresamos nombre del wifi
const char* password = "mi mascota actual";   //ingresamos la contraseña de wifi

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// las variable para las salidas de los botones
String output26State = "off";
String output27State = "off";
String estado_p1 = "off";
String estado_p2 = "off";
String estado_p3 = "off";
String estado_p4 = "off";
int entrada = 0;
int rebote = 0;
int suma = 0;
String num4 = "4";
String num3 = "3";
String num2 = "2";
String num1 = "1";
String num0 = "0";

//banderas para las salidas
bool cont = 0;
bool valpin1 = 0;
bool valpin2 = 0;
bool valpin3 = 0;
bool valpin4 = 0;

// asignamos los valores de los pines a un nombre para un mejor manejo
const int output26 = 26;
const int output27 = 27;
const int output0 = 14;
const int output1 = 18;
const int output2 = 19;
const int output3 = 2;
const int output4 = 12;
const int output5 = 16;
const int output6 = 26;
const int output7 = 27;
const int outputdp = 4;
const int inputpin1 = 32;
const int inputpin2 = 35;
const int inputpin3 = 34;
const int inputpin4 = 39;
//const int out_pin[] = {14, 18, 19, 2, 12, 16, 26, 27, 4};
const int val_0[] = {1,1,1,1,1,1,0,1};
const int val_1[] = {0,1,1,0,0,0,0,1};
const int val_2[] = {1,1,0,1,1,0,1,1};
const int val_3[] = {1,1,1,1,0,0,1,1};
const int val_4[] = {0,1,1,0,0,1,1,1};
// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);   
  // inicializamos las salidas
  pinMode(output26, OUTPUT);
  pinMode(output27, OUTPUT);
  pinMode(output0, OUTPUT);
  pinMode(output1, OUTPUT);
  pinMode(output2, OUTPUT);
  pinMode(output3, OUTPUT);
  pinMode(output4, OUTPUT);
  pinMode(output5, OUTPUT);
  pinMode(output6, OUTPUT);
  pinMode(output7, OUTPUT);
  pinMode(outputdp, OUTPUT);

  //declaramos los pines como entradas
  pinMode(inputpin1, INPUT);
  pinMode(inputpin2, INPUT);
  pinMode(inputpin3, INPUT);
  pinMode(inputpin4, INPUT);

  //seteamos los pines a LOW
  digitalWrite(output26, LOW);
  digitalWrite(output27, LOW);
  digitalWrite(output0, LOW);
  digitalWrite(output1, LOW);
  digitalWrite(output2, LOW);
  digitalWrite(output3, LOW);
  digitalWrite(output4, LOW);
  digitalWrite(output5, LOW);
  digitalWrite(output6, LOW);
  digitalWrite(output7, LOW);
  digitalWrite(outputdp, LOW);

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // imprimimos la direccion serial
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){
  //leemos los valores mandados de la tiva c
  valpin1 = digitalRead(inputpin1);
  valpin2 = digitalRead(inputpin2);
  valpin3 = digitalRead(inputpin3);
  valpin4 = digitalRead(inputpin4);

  if(valpin1 == 0){
    estado_p1 = "LIBRE";
  }
  else if(valpin1 == 1){
    estado_p1 = "OCUPADO";
  }
  if(valpin2 == 0){
    estado_p2 = "LIBRE";
  }
  else if(valpin2 == 1){
    estado_p2 = "OCUPADO";
  }
  if(valpin3 == 0){
    estado_p3 = "LIBRE";
  }
  else if(valpin3 == 1){
    estado_p3 = "OCUPADO";
  }
  if(valpin4 == 0){
    estado_p4 = "LIBRE";
  }
  else if(valpin4 == 1){
    estado_p4 = "OCUPADO";
  }
  suma = valpin1 + valpin2 + valpin3 + valpin4;
  //leemos las entradas de la tiva c y dependiendo de la secuencia encendemos los pines necesarios para mostrar los valores en el display de 7seg
  //Tambien cambiamos el valor de la variable 'entrada' dependiendo del valor leido
  //if (entrada == 0 ){
  if (suma == 0){
  digitalWrite(output26,LOW);
  digitalWrite(output1, HIGH);
  digitalWrite(output4, HIGH);
  digitalWrite(output3, LOW);
  digitalWrite(output27,LOW);
  digitalWrite(output0, HIGH);
  digitalWrite(output5, HIGH);
  digitalWrite(outputdp,HIGH);
  entrada = 4;
  
            }
 // else if(entrada == 1){
    else if(suma == 1){
  
  digitalWrite(output26,HIGH);
  digitalWrite(output1, HIGH);
  digitalWrite(output4, HIGH);
  digitalWrite(output3, HIGH);
  digitalWrite(output27,LOW);
  digitalWrite(output0, LOW);
  digitalWrite(output5, HIGH);
  digitalWrite(outputdp,HIGH);
  entrada = 3;
            }
//  else if(entrada == 2){
  else if(suma == 2){
  digitalWrite(output26,HIGH);
  digitalWrite(output1, HIGH);
  digitalWrite(output4, LOW);
  digitalWrite(output3, HIGH);
  digitalWrite(output27,HIGH);
  digitalWrite(output0, LOW);
  digitalWrite(output5, HIGH);
  digitalWrite(outputdp,HIGH);
  entrada = 2;
            }
//  else if(entrada == 3){
  else if(suma == 3){
  digitalWrite(output26,LOW);
  digitalWrite(output1, HIGH);
  digitalWrite(output4, HIGH);
  digitalWrite(output3, LOW);
  digitalWrite(output27,LOW);
  digitalWrite(output0, LOW);
  digitalWrite(output5, LOW);
  digitalWrite(outputdp,HIGH);
  entrada = 1;
            }
//  else if(entrada == 4){
  else if(suma == 4){
  digitalWrite(output26,HIGH);
  digitalWrite(output1, HIGH);
  digitalWrite(output4, HIGH);
  digitalWrite(output3, HIGH);
  digitalWrite(output27,HIGH);
  digitalWrite(output0, HIGH);
  digitalWrite(output5, LOW);
  digitalWrite(outputdp,HIGH);
  entrada = 0;
            }
  else{
  digitalWrite(output26,LOW);
  digitalWrite(output1, LOW);
  digitalWrite(output4, LOW);
  digitalWrite(output3, LOW);
  digitalWrite(output27,LOW);
  digitalWrite(output0, LOW);
  digitalWrite(output5, LOW);
  digitalWrite(outputdp,HIGH);
            }
  WiFiClient client = server.available();   // leemos si hay cambios en la pagina
  
  if (client) {                             // si se conecta un cliente
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          // imprimimos el mensaje que hay nuevo cliente
    String currentLine = "";                // hacemos una variable para guardar la entrada
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // empezamos un loop mientras el cliente este conectado
      currentTime = millis();
      if (client.available()) {             // si hay variable que leer las leemos
        char c = client.read();             // leemos el valor
        Serial.write(c);                    // y lo imprimimos en serial
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            //Esta parte es por si queremos cambiar valores del programa atravez del servidor 
            // turns the GPIOs on and off
            if (header.indexOf("GET /26/on") >= 0) {
              Serial.println("GPIO 26 on");
              output26State = "on";
              //digitalWrite(output26, HIGH);
            } else if (header.indexOf("GET /26/off") >= 0) {
              Serial.println("GPIO 26 off");
              output26State = "off";
              //digitalWrite(output26, LOW);
            } else if (header.indexOf("GET /27/on") >= 0) {
              Serial.println("GPIO 27 on");
              output27State = "on";
              //digitalWrite(output27, HIGH);
            } else if (header.indexOf("GET /27/off") >= 0) {
              Serial.println("GPIO 27 off");
              output27State = "off";
              //digitalWrite(output27, LOW);
            }
            
            // Desplegamos el codigo html
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            
            client.println("<p><a href=\"/27/on\"><button class=\"button\">Refresh</button></a></p>");
            // Web Page Heading
            client.println("<body><h1>ESP32 Web Server</h1>");

            //aqui se imprime el texto de los lugares ocupados dependiendo del valor de entrada
            if (entrada == 0){
              client.println("<p>NUMERO DE LUGARES LIBRES   " + num0 + "</p>");
            }
            else if(entrada == 1){
              client.println("<p>NUMERO DE LUGARES LIBRES  " + num1 + "</p>");
            }
            else if(entrada == 2){
              client.println("<p>NUMERO DE LUGARES LIBRES  " + num2 + "</p>");
            }
            else if(entrada == 3){
              client.println("<p>NUMERO DE LUGARES LIBRES  " + num3 + "</p>");
            }
            else if(entrada == 4){
              client.println("<p>NUMERO DE LUGARES LIBRES  " + num4 + "</p>");
            }
              client.println("<p>Parqueo1   " + estado_p1 + "</p>");
              client.println("<p>Parqueo2   " + estado_p2 + "</p>");
              client.println("<p>Parqueo3   " + estado_p3 + "</p>");
              client.println("<p>Parqueo4   " + estado_p4 + "</p>");
            
            // Display current state, and ON/OFF buttons for GPIO 26  
//            client.println("<p>GPIO 26 - State " + output26State + "</p>");
            // If the output26State is off, it displays the ON button       
//            if (output26State=="off") {
//              client.println("<p><a href=\"/26/on\"><button class=\"button\">ON</button></a></p>");
//            } else {
//              client.println("<p><a href=\"/26/off\"><button class=\"button button2\">OFF</button></a></p>");
//            } 
               
            // Display current state, and ON/OFF buttons for GPIO 27  
//            client.println("<p>GPIO 27 - State " + output27State + "</p>");
//            client.println("<p>GPIO 27 - numero es " + output27State + "</p>");
//             If the output27State is off, it displays the ON button       
//            if (output27State=="off") {
//              client.println("<p><a href=\"/27/on\"><button class=\"button\">ON</button></a></p>");
//            } else {
//              client.println("<p><a href=\"/27/off\"><button class=\"button button2\">OFF</button></a></p>");
//            }
            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
