#include <Arduino.h>
#line 1 "C:\\Users\\anton\\Desktop\\proyecto_guante_midi\\proyecto_guante_midi.ino"
#include <HCSR04.h>

// Define los pines para el sensor
const int triggerPin = 4;
const int echoPin = 3;
float maxHeight = 0;
float minHeight = 0;

// Array de 10 muestras para el promedio
float samples[10];
int sampleIndex = 0;

float distance = 0;
float lastDistance = 0;

// Crea una instancia del sensor
UltraSonicDistanceSensor distanceSensor(triggerPin, echoPin);


#line 20 "C:\\Users\\anton\\Desktop\\proyecto_guante_midi\\proyecto_guante_midi.ino"
void setup();
#line 41 "C:\\Users\\anton\\Desktop\\proyecto_guante_midi\\proyecto_guante_midi.ino"
void loop();
#line 20 "C:\\Users\\anton\\Desktop\\proyecto_guante_midi\\proyecto_guante_midi.ino"
void setup()
{
    // Inicia la comunicación serial
    Serial.begin(9600);
    
    distance = distanceSensor.measureDistanceCm();
    if (distance < 1) {distance = 1;}
    maxHeight = distance;
    minHeight = distance;

    // Inicializa el array de muestras
    for (int i = 0; i < 10; i++)
    {
        distance = distanceSensor.measureDistanceCm();
        if (distance < 1) {distance = 1;}
        samples[i] = distance;
        delay(10);
    }

}

void loop()
{
    // Lee la distancia en centímetros
    lastDistance = distance;
    distance = distanceSensor.measureDistanceCm();
    if (distance < 1) {distance = 1;}
    
    if (distance > lastDistance + 10) {distance = lastDistance;} // Evita cambios bruscos para que no desconfigure el promedio

    // Calcula el promedio de las muestras
    samples[sampleIndex] = distance;
    sampleIndex++;
    if (sampleIndex > 10) {sampleIndex = 0;}

    float sum = 0;
    for (int i = 0; i < 10; i++)
    {
        sum += samples[i];
    }
    distance = sum / 10;

    // Actualiza los valores máximos y mínimos
    if (distance > maxHeight) {maxHeight = distance;}
    if (distance < minHeight) {minHeight = distance;}

    // Devuelve numeros entre 0 y 127 basados en la distancia
    int modWheel = (distance - minHeight) / (maxHeight - minHeight) * 127;

    // Imprime la distancia en la consola serial
    Serial.println(modWheel);



    // Normaliza los valores máximos y mínimos
    maxHeight = maxHeight + 0.001*(distance - maxHeight); //negativo
    minHeight = minHeight + 0.001*(distance - minHeight); //positivo

    delay(10);

    // Espera un poco antes de la próxima medición
     // 10 ms, latencia típica de instrumentos MIDI
}

