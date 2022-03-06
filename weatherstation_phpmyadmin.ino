/* Load Wi-Fi library
-> VCC => 3.3V 
-> GND => GND
-> SCL => D22
-> SDA => D21
-> CSB => not connected
-> SDD => not connected
   //DHT11
-> VCC => 3.3V
-> OUT=> D4
-> GND => GND
   //LEDs
-> LED 1 => D26
-> LED 2 => D27
   //SENSOR DE LLUVIA
-> VCC => 3.3V
-> GND => GND
-> AO => D34
-> DO => D35
*/

#include <WiFi.h>
#include <Wire.h>
#include <WiFiClient.h>
#include <Adafruit_BMP280.h>
#include "DHT.h"

#define SEALEVELPRESSURE_HPA (1013.25)
#define DHTTYPE DHT11

#define rainAnalog 35
#define rainDigital 34

Adafruit_BMP280 bmp; // I2C

// Reemplace con sus credenciales de red
const char* ssid     = "TU RED WIFI";
const char* password = "CONTRASEÑA WIFI";

////eg: 192.168.0.222
char serverName[] = "YOUR IP";   

// Establecer el número de puerto del servidor web en 80
WiFiClient client;
WiFiServer server(80);

// DHT Sensor
uint8_t DHTPin = 4;

// Initialize DHT sensor.
DHT dht(DHTPin, DHTTYPE);              


// Variable para almacenar la solicitud HTTP
String header;

// Auxiliar variables to store the current output state
String output26State = "off";
String output27State = "off";

// Assign output variables to GPIO pins
const int output26 = 26;
const int output27 = 27;

// Current time
unsigned long currentTime = millis();

// Hora actual 
unsigned long previousTime = 0; 

// Definir el tiempo de espera en milisegundos (ejemplo: 2000ms = 2s) 
const long timeoutTime = 1000;

void setup() {
  Serial.begin(115200);
  bool status;
  
  pinMode(rainDigital,INPUT);
  
// configuración predeterminada 

  // Inicializar las variables de salida como salidas
  pinMode(output26, OUTPUT);
  pinMode(output27, OUTPUT);
  // Establecer salidas en LOW
  digitalWrite(output26, LOW);
  digitalWrite(output27, LOW);
  
  pinMode(DHTPin, INPUT);

  dht.begin(); 
  
  //status = bmp.begin();
    
  if (!bmp.begin(0x76)) {
    Serial.println("No se pudo encontrar un sensor BMP280 válido, verifique el cableado!");
    while (1);
  }
  
// Conéctese a la red Wi-Fi con SSID y contraseña
  Serial.print("Conectando a ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  // Imprime la dirección IP local e inicia el servidor web 
  Serial.println("");
  Serial.println("WiFi conectado.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

   
void loop(){

  int rainAnalogVal = analogRead(rainAnalog);
  int rainDigitalVal = digitalRead(rainDigital);

  Sending_To_phpmyadmindatabase();  
    delay(10000); // interval
    
  WiFiClient client = server.available();   // Escuche los clientes entrantes
  if (client) {                               // Si se conecta un nuevo cliente           
    currentTime = millis(); 
    previousTime = currentTime;
    Serial.println("Nuevo cliente.");          // imprime un mensaje en el puerto serie 
    String currentLine = "";                // crea una cadena para contener los datos entrantes del cliente 
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // bucle mientras el cliente está conectado
      currentTime = millis();
      if (client.available()) {             // si hay bytes para leer del cliente, 
        char c = client.read();             // lee un byte, luego
        Serial.write(c);                    // si el byte es un carácter de nueva línea 
        header += c;
        if (c == '\n') {                    // si la línea actual está en blanco, tienes dos caracteres de nueva línea seguidos. 
          // ese es el final de la solicitud HTTP del cliente, así que envíe una respuesta:
          if (currentLine.length() == 0) {
            // Los encabezados HTTP siempre comienzan con un código de respuesta (por ejemplo, HTTP/1.1 200 OK) 
            // y un content-type para que el cliente sepa lo que viene, luego una línea en blanco: 
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();


              // Funciones de los LEDs de Alerta 
            // LED D26 ROJO
              if(bmp.readTemperature()>=20 && bmp.readTemperature()<=25){
                Serial.println("La temperatura esta entre 20 y 25 c°");
                Serial.println("GPIO26 LED is ON");
                  output26State = "on";
                  digitalWrite(output26, HIGH);             
              }else {
                  Serial.println("GPIO26 LED is OFF");
                    output26State = "off";
                    digitalWrite(output26, LOW);
              }
              
              // LED D27 BLANCO
              if(bmp.readTemperature()>=25 && bmp.readTemperature()<=30){
                  Serial.println("La temperatura esta entre 25 y 30 c°");
                  Serial.println("GPIO27 LED is ON");
                  output27State = "on";
                  digitalWrite(output27, HIGH);
              }else{
                  Serial.println("GPIO27 LED is OFF");
                  output27State = "off";
                  digitalWrite(output27, LOW);
              }

              //Serial.print(rainAnalogVal);
              Serial.print("lluvia ->");
              Serial.println(rainDigitalVal);
              delay(200);
            
  
            // Mostrar la página web HTML
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            
            //// CSS para aplicar estilo a la tabla
            
            client.println("<style>body { text-align: center; font-family: Helvetica}");
            client.println("table { border-collapse: collapse; margin-left:auto; margin-right:auto; }");
            client.println("th { padding: 12px; background-color: #3c8b91; color: write; }");
            client.println("tr { border: 1px solid #ddd; padding: 12px; }");
            client.println("tr:hover { background-color: #3c8b91; }");
            client.println("td { border: none; padding: 12px; }");
            client.println(".sensor { color:black; font-weight: bold; padding: 1px; }");
            
            // Encabezado de página web         
            client.println("</style></head><body><h1>¡¡¡WEATHER STATION!!!</h1>");
            client.println("<table><tr><th>MEDICION</th><th>VALOR</th></tr>");
            client.println("<tr><td>Temp. Celsius</td><td><span class=\"sensor\">");
            client.println(bmp.readTemperature());
            client.println(" *C</span></td></tr>");  
            client.println("<tr><td>Humedad</td><td><span class=\"sensor\">");
            client.println(dht.readHumidity());
            client.println(" % </span></td></tr>");
            client.println("<tr><td>Presion</td><td><span class=\"sensor\">");
            client.println(bmp.readPressure() / 100.0F);
            client.println(" hPa</span></td></tr>");
            client.println("<tr><td>Aprox. Altitud</td><td><span class=\"sensor\">");
            client.println(bmp.readAltitude(SEALEVELPRESSURE_HPA));
            client.println(" m</span></td></tr>");  
            client.println("<tr><td>Lluvia</td><td><span class=\"sensor\">");
            client.println(rainDigitalVal);
            client.println("</span></td></tr>"); 
            client.println("</body></html>");

            
            // La respuesta HTTP termina con otra línea en blanco 
            client.println();
            // Salir del ciclo while
            break;
          } else { // si obtuvo una nueva línea, borre currentLine 
            currentLine = "";
          }
        } else if (c != '\r') {  // si obtuvo algo más que un carácter de retorno de carro, 
          currentLine += c;      // agregarlo al final de currentLine 
        }
      }
    }
    // Borrar la variable de 
    header = "";
    
    // Cerrar la conexión
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}       

//CONECCION CON MYSQL
     
 void Sending_To_phpmyadmindatabase()   
 {
   if (client.connect(serverName, 80)) {
    Serial.println("connected");
    // Haz una solicitud HTTP:
    Serial.print("GET /testcode/dht.php?humidity=");
    client.print("GET /testcode/dht.php?humidity=");     //TU URL
    Serial.println(dht.readHumidity());
    client.print(dht.readHumidity());
    client.print("&temperature=");
    Serial.println("&temperature=");
    client.print(bmp.readTemperature());
    Serial.println(bmp.readTemperature());
    client.print("&pressure=");
    Serial.println("&pressure=");
    client.print(bmp.readPressure() / 100.0F);
    Serial.println(bmp.readPressure() / 100.0F);
    client.print("&altitude=");
    Serial.println("&altitude=");
    client.print(bmp.readAltitude(SEALEVELPRESSURE_HPA));
    Serial.println(bmp.readAltitude(SEALEVELPRESSURE_HPA));
    client.print("&lluvia=");
    Serial.println("&lluvia=");
    client.print(digitalRead(rainDigital));
    Serial.println(digitalRead(rainDigital));
    client.print(" ");      //SPACE BEFORE HTTP/1.1
    client.print("HTTP/1.1");
    client.println();
    client.println("Host: YOUR HOST");
    client.println("Connection: close");
    client.println();
  } else {
    // si no obtuviste una conexión con el servidor:
    Serial.println("connection failed");
  }
 }
