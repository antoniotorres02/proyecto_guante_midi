import rtmidi
from time import sleep


midiout = rtmidi.MidiOut()
available_ports = midiout.get_ports()
print(available_ports)

#Busca el puerto de loopMIDI, la API de Windows no permite emular puertos MIDI, por lo que se debe usar loopMIDI para

for i in range(len(available_ports)):
    if "loopMIDI" in available_ports[i]:
        print("Found loopMIDI at index: " + str(i))
        midiout.open_port(i)
        break



note_on = [0x90, 60, 112] # channel 1, middle C, velocity 112
note_off = [0x80, 60, 0]
midiout.send_message(note_on)
sleep(0.5)
midiout.send_message(note_off)

