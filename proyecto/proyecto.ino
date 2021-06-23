#include <DHT.h> // Librería sensores DHT
#include "ThingSpeak.h" // Include ThingSpeak Library
#include <ESP8266WiFi.h> // Include ESP8266wifi Library
#define DHTPIN 2// pin al que hemos conectado el DHT 11
#define DHTTYPE DHT11 //DHT21, DHT22
#include <Servo.h> //servomotor
Servo myservo;
int IN3 = 12; 
int IN4 = 15;
int lestado1 = 0;  // Digital pin D2
int lestado2 = 16;  // Digital pin D2
int sensor = 4;  // Digital pin D6
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
    pinMode(sensor, INPUT);   // declare sensor as input
    pinMode(IN4, OUTPUT);    // Input4 conectada al pin 5
    pinMode(IN3, OUTPUT);    // Input3 conectada al pin 6
    pinMode(lestado1, OUTPUT);  // declare LED as output
    pinMode(lestado2, OUTPUT);  // declare LED as output
    WiFi.begin(ssid, pass); // Inicia WiFi
    ThingSpeak.begin(client); // Inicia ThingSpeak
    Serial.begin(115200); // Velocidad puerto serie
    Serial.println();
    myservo.attach(13); // attaches the servo on pin D7 to the servo object
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
       Serial.print(temperatura, 0);
       Serial.println("°C");
       Serial.print("Humedad: "); 
       Serial.print(" %\t");
       Serial.print(humedad, 2);
       Serial.println("%");
       Serial.print(" %\t");
       Serial.println("**********************************************************************************");
       Serial.println();
       delay(5000);
      long estado = digitalRead(sensor);
        if(estado == HIGH) {
          digitalWrite (lestado2, HIGH);
          Serial.println("Se detecto movimiento!");
          //mover servomotor pos 180ª
          myservo.write(180);
          delay(1000);
        }
        else {
          digitalWrite (lestado1, LOW);
          Serial.println("No se detecto movimiento!");
          //mover servomotor pos 0ª
          myservo.write(0);
          delay(1000);
          }
        if(temperatura>16){ //Condición para mantener el ambiente fresco.
          Serial.println("Ventiladores Encendidos");
          Serial.println("Revisar sistema de refrigeracion");
          digitalWrite (IN4, HIGH);
          digitalWrite (IN3, HIGH); 
          Serial.println("Mover servo");
          //mover servomotor pos 180ª
          myservo.write(180);
          temperatura= dht.readTemperature(); //Volvemos a leer la temperatura
          delay(1000);           
          digitalWrite(lestado2, HIGH);
          delay(10); 
        }
        else
          {
          Serial.println("Ventiladores Apagados");
          digitalWrite (IN3, LOW); 
          digitalWrite (IN4, LOW); 
          Serial.println("Mover servo");          
          Serial.println("Sistema de refrigeracion estable");
          //mover servomotor pos 0ª
          myservo.write(0);
          delay(10); 
          digitalWrite(lestado1, HIGH);
          }
//Carga los valores a enviar
ThingSpeak.setField(1, temperatura);
ThingSpeak.setField(2, humedad);
ThingSpeak.setField(3, estado);
//Escribe todos los campos a la vez.
ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
Serial.println("¡Datos enviados a ThingSpeak!");
//Añadimos un retraso de 5 minutos para limtitar el número de escrituras en Thinhspeak
delay (300000);
}
