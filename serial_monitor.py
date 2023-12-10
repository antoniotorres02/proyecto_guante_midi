import serial

ser = serial.Serial('COM3', 9600, timeout=1)



while True:
    while (ser.inWaiting() == 0):
        pass
    line = ser.readline()
    line = line.decode("utf-8")
    line = line.rstrip()
    
    print(line)



