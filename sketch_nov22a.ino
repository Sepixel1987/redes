#include <WiFi.h>
#include <WiFiUdp.h>


const char* ssid = "Grupo_RPi"; 
const char* password = "certamen123";                  
const int tcpPort = 5000;                     
const int udpPort = 5001;                     


WiFiClient client;
WiFiUDP udp;
bool usandUDP = false; 

void setup() {
  Serial.begin(115200);
  delay(10);

  
  Serial.println();
  Serial.print("Conectando a ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("IP del ESP32: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  
  int largo = random(10, 513);
  String mensaje = generarTextoAleatorio(largo);

  if (!usandUDP) {
    
    
    if (!client.connected()) {
      Serial.println("Conectando al servidor TCP...");
      if (client.connect(host, tcpPort)) {
        Serial.println("Conectado a TCP");
      } else {
        Serial.println("Fallo conexión TCP, reintentando...");
        delay(1000);
        return;
      }
    }

    
    client.print(mensaje);
    Serial.println("Enviado por TCP (Largo " + String(largo) + ")");

    
    if (client.available()) {
      String respuesta = client.readStringUntil('\n');
      
      
      if (respuesta.indexOf("SWITCH_TO_UDP") >= 0) { 
        Serial.println("!!! ORDEN RECIBIDA: CAMBIANDO A UDP !!!");
        client.stop(); 
        usandUDP = true; 
      }
    }

  } else {
    
    
    udp.beginPacket(host, udpPort);
    udp.write((const uint8_t*)mensaje.c_str(), mensaje.length());
    udp.endPacket();
    Serial.println("Enviado por UDP (Largo " + String(largo) + ")");
  }

  delay(1000);
}

  
String generarTextoAleatorio(int longitud) {
  String txt = "";
  const char alphanum[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
  for (int i = 0; i < longitud; i++) {
    txt += alphanum[random(sizeof(alphanum) - 1)];
  }
  return txt;
}