import rtmidi
from time import sleep


##Funcion que envia una nota MIDI a traves del puerto MIDI, orientada a pruebas de funcionamiento
def note_on(midiout):
    note_on = [0x90, 60, 112] # channel 1, middle C, velocity 112
    note_off = [0x80, 60, 0]
    midiout.send_message(note_on)
    sleep(0.5)
    midiout.send_message(note_off)

def open_loopMIDI_port(midiout):
    available_ports = midiout.get_ports()
    print(available_ports)
    for i in range(len(available_ports)):
        if "loopMIDI" in available_ports[i]:
            print("Found loopMIDI at index: " + str(i))
            midiout.open_port(i)
            break

def mod_wheel(midiout, value):
    mod_wheel = [0xB0, 1, value]
    midiout.send_message(mod_wheel)


midiout = rtmidi.MidiOut()

open_loopMIDI_port(midiout)



import serial


ser = serial.Serial('COM3', 9600, timeout=1)



#Bucle que lee el puerto serial y gestiona los datos recibidos
while True:
    while (ser.inWaiting() == 0):
        pass
    line = ser.readline()
    line = line.decode("utf-8")
    line = line.rstrip()
    parsed_num = int(line)
    mod_wheel(midiout, parsed_num)
    ##print(line) #Descomentar para ver los datos recibidos en la consola






