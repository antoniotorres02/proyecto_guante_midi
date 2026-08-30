# Guante MIDI por ultrasonidos

Controlador MIDI gestual construido con Arduino y un sensor ultrasónico HC-SR04. La distancia entre la mano y el sensor se transforma en valores de **0 a 127** y se envía como **MIDI Control Change 1 (Mod Wheel)** a un instrumento virtual o DAW.

El proyecto está orientado a Windows: Arduino transmite las lecturas por USB/serie, un programa en Python las convierte a MIDI y [loopMIDI](https://www.tobias-erichsen.de/software/loopmidi.html) ofrece el puerto MIDI virtual que recibe el DAW.

```text
Mano -> HC-SR04 -> Arduino -> USB/serie -> Python -> loopMIDI -> DAW o sintetizador
         distancia    0-127       9600 baudios       MIDI CC1
```

## Cómo funciona

El firmware toma muestras del HC-SR04 aproximadamente cada 10 ms y aplica una media móvil de diez lecturas para suavizar el movimiento. A partir de los valores mínimo y máximo observados, adapta la distancia al rango MIDI completo (0–127). Esos límites se reajustan lentamente durante el uso para que el control conserve sensibilidad aunque cambie la posición de la mano.

[`interfaz_guante_programa.py`](interfaz_guante_programa.py) lee cada valor del puerto serie y lo envía al primer puerto cuyo nombre contenga `loopMIDI`, en el canal MIDI 1 y como controlador CC1.

## Material necesario

- Arduino Mega 2560 (la compilación incluida se generó para esta placa).
- Sensor ultrasónico HC-SR04.
- Cables y conexión USB para Arduino.
- Python 3.
- Arduino IDE y la librería [`HCSR04`](https://github.com/Martinsos/arduino-lib-hc-sr04).
- [loopMIDI](https://www.tobias-erichsen.de/software/loopmidi.html).
- Un DAW, sintetizador o monitor capaz de recibir MIDI.

## Conexiones

| HC-SR04 | Arduino Mega |
|---|---|
| VCC | 5V |
| GND | GND |
| TRIG | Pin digital 4 |
| ECHO | Pin digital 3 |

## Instalación

1. Clona el repositorio:

   ```bash
   git clone https://github.com/antoniotorres02/proyecto_guante_midi.git
   cd proyecto_guante_midi
   ```

2. Instala las dependencias de Python:

   ```bash
   python -m pip install pyserial python-rtmidi
   ```

3. Abre [`proyecto_guante_midi.ino`](proyecto_guante_midi.ino) en Arduino IDE, instala la librería `HCSR04`, selecciona **Arduino Mega or Mega 2560** y carga el programa en la placa.

4. Abre loopMIDI y crea un puerto virtual. El nombre debe contener `loopMIDI`, por ejemplo `loopMIDI Port`.

5. Comprueba en el Administrador de dispositivos qué puerto COM utiliza Arduino. Si no es `COM3`, cambia esta línea en [`interfaz_guante_programa.py`](interfaz_guante_programa.py):

   ```python
   ser = serial.Serial('COM3', 9600, timeout=1)
   ```

6. Ejecuta el puente serie-MIDI:

   ```bash
   python interfaz_guante_programa.py
   ```

7. En el DAW o instrumento virtual, activa como entrada el puerto creado en loopMIDI. Al mover la mano frente al sensor deberían recibirse mensajes **CC1 / Mod Wheel** con valores entre 0 y 127.

> Al arrancar, mueve la mano por el recorrido que quieras utilizar. El sistema aprende los extremos mínimo y máximo durante el funcionamiento.

## Utilidades de prueba

- [`serial_monitor.py`](serial_monitor.py): muestra en consola los valores enviados por Arduino. Resulta útil para comprobar el sensor y la conexión serie antes de introducir MIDI.
- [`emulator.py`](emulator.py): busca el puerto de loopMIDI y envía una nota Do central breve. Permite verificar la ruta MIDI sin conectar Arduino.

Antes de ejecutar `serial_monitor.py`, ajusta también su puerto `COM3` si es necesario. No ejecutes el monitor serie y el programa principal a la vez: solo una aplicación puede ocupar normalmente el puerto COM.

## Resolución de problemas

### No aparece ningún puerto MIDI

Abre loopMIDI, crea el puerto y mantenlo en ejecución. El script solo abre puertos cuyo nombre contiene literalmente `loopMIDI`.

### `SerialException: could not open port`

Verifica el número de puerto COM, cierra el monitor serie de Arduino IDE y cualquier otro programa que pueda estar usando la placa.

### No llegan datos al DAW

Ejecuta primero `serial_monitor.py`. Si aparecen números, Arduino y el sensor funcionan y el problema está entre loopMIDI y la configuración MIDI del DAW. Si no aparecen, revisa el cableado, el puerto COM y que ambos lados usen **9600 baudios**.

### Los valores saltan o responden mal

Evita objetos cercanos que puedan producir ecos, coloca el sensor perpendicular al movimiento y reinicia Arduino para comenzar una calibración nueva.

## Estructura del proyecto

| Archivo | Función |
|---|---|
| `proyecto_guante_midi.ino` | Lectura, filtrado y normalización del sensor en Arduino. |
| `interfaz_guante_programa.py` | Conversión de datos serie a MIDI CC1. |
| `serial_monitor.py` | Diagnóstico de la comunicación serie. |
| `emulator.py` | Prueba independiente de la salida MIDI. |
| `build/` | Artefactos de una compilación para Arduino Mega 2560. |

## Personalización

El mensaje MIDI se define en la función `mod_wheel`:

```python
mod_wheel = [0xB0, 1, value]
```

- `0xB0`: Control Change en el canal MIDI 1.
- `1`: número del controlador (Mod Wheel).
- `value`: lectura normalizada entre 0 y 127.

Puedes cambiar el segundo byte para controlar otro parámetro MIDI CC, o modificar el canal en el primer byte (`0xB0` a `0xBF` para los canales 1 a 16).
