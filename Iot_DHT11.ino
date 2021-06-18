#include <DHT.h> // Librería sensores DHT
#include "ThingSpeak.h" // Include ThingSpeak Library
#include <ESP8266WiFi.h> // Include ESP8266wifi Library
#define DHTPIN 2// pin al que hemos conectado el DHT 11
#define DHTTYPE DHT11 //DHT21, DHT22
int fan = A0;
char ssid[] = "ZTE-ec1596"; // Nombre del WiFi (nombre del router)
char pass[] = "Home789*Aa"; // WiFi router password
unsigned long myChannelNumber = 1399943; // Thingspeak número de canal
const char * myWriteAPIKey = "455X3MNZ8XFJL4KW"; // ThingSpeak write API Key
DHT dht(DHTPIN, DHT11, 27);

int status = WL_IDLE_STATUS;

WiFiClient client;
float temperatura; 
float humedad;
void setup() {
//pinMode(FAN_PIN, OUTPUT);
WiFi.begin(ssid, pass); // Inicia WiFi
ThingSpeak.begin(client); // Inicia ThingSpeak
Serial.begin(115200); // Velocidad puerto serie

Serial.println();
Serial.print("Conectando a ");
Serial.print(ssid);

while (WiFi.status() != WL_CONNECTED)
{
delay(500);
Serial.print(".");
}

Serial.println();
Serial.println("Conectado a WiFi");
Serial.print("Dirección IP: ");
Serial.println(WiFi.localIP());
dht.begin(); // Inicia el sensor
}

void loop() {

// Hacemos pausa de dos segundos antes de cada nueva medición
//al sensor le cuesta 250ms leer estos datos
delay(3500);

//leemos humedad
float humedad = dht.readHumidity();

// usamos variables tipo float para guardar valores decimales
// Leemos temperatura
float temperatura = dht.readTemperature();

// Comprobamos si las lecturas pueden dar algún fallo mediante la función isnan()
// Esta función devuelve un 1 en caso de que el valor no sea numérico
// Los caracteres || son como un OR. Si se cumple una de las dos condiciones mostramos error

if (isnan(humedad) || isnan(temperatura)) {
Serial.println("Fallo en la lectura del sensor DHT!");
return;
}
Serial.println("**********************************************************************************");
   Serial.print("Temperatura: ");
   Serial.print(temperatura);
   Serial.println("°C");
   Serial.print("Humedad: "); 
   Serial.print(" %\t");
   Serial.print(humedad, 2);
   Serial.println("%");
   Serial.print(" %\t");
   Serial.println("**********************************************************************************");
   Serial.println();
   delay(5000);

if(temperatura>=19){ //Condición para mantener el ambiente fresco.
Serial.println("Ventiladores Encendidos");
analogWrite(fan,500); //Encendemos el ventilador
temperatura= dht.readTemperature(); //Volvemos a leer la temperatura
delay(2000);
}
else{
  digitalWrite(fan,0);
}
// Carga los valores a enviar
ThingSpeak.setField(1, temperatura);
ThingSpeak.setField(2, humedad);

// Escribe todos los campos a la vez.
ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

//Serial.println("¡Datos enviados a ThingSpeak!");

// Añadimos un retraso de 5minutos para limtitar el número de escrituras en Thinhspeak
delay (300000);
}//
