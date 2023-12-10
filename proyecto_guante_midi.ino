#include <HCSR04.h>

// Define los pines para el sensor
const int triggerPin = 4;
const int echoPin = 3;
float maxHeight = 0;
float minHeight = 0;

// Crea una instancia del sensor
UltraSonicDistanceSensor distanceSensor(triggerPin, echoPin);

void setup()
{
    // Inicia la comunicación serial
    Serial.begin(9600);
    maxHeight = distanceSensor.measureDistanceCm();
    minHeight = maxHeight;

}

void loop()
{
    // Lee la distancia en centímetros
    float distance = distanceSensor.measureDistanceCm();

    // Actualiza los valores máximos y mínimos
    if (distance > maxHeight)
    {
        maxHeight = distance;
    }
    if (distance < minHeight)
    {
        minHeight = distance;
    }

    // Devuelve numeros entre 0 y 127 basados en la distancia
    int modWheel = (distance - minHeight) / (maxHeight - minHeight) * 127;

    // Imprime la distancia en la consola serial
    Serial.println(modWheel);

    // Espera un poco antes de la próxima medición
    delay(10); // 10 ms, latencia típica de instrumentos MIDI
}
