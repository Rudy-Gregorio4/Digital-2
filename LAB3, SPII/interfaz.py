from tkinter import Tk, Label, Button, Entry
import serial
import time
import threading

#------------
# GUI design
#------------

root = Tk()
root.title("Laboratorio 3")
root.resizable(0,0)  # Evitar redimensionar
root.geometry("400x400")

#---------------------------------------------------------

Runpot = True
cambio = 1  # Variable de multiplexación de potenciómetros
bloqueoUART = 1  # Variable de bloqueo de UART normal

#------------
# Reading potenciometers
#------------

entry1 = Entry(root, bg='#fdf2e9')
entry1.place(x=200, y=70, width=30, height=30)

entry2 = Entry(root, bg='#fdf2e9')
entry2.place(x=200, y=120, width=30, height=30)

def pots():
    global cambio
    global bloqueoUART
    
    while Runpot:
        try:
            if bloqueoUART == 0:
                arduino.write(bytes('R', 'utf-8'))
                bloqueoUART = 1
                time.sleep(0.3)
            else:
                arduino.write(bytes('Q', 'utf-8'))
                time.sleep(0.1)
                raw_mensaje = arduino.readline()
                print(f"Raw mensaje: {raw_mensaje}")  # Imprimir los datos en bruto
                mensaje = raw_mensaje.decode('utf-8', errors='ignore')  # Ignorar errores de decodificación

            if cambio == 0:
                entry1.delete(0, 'end')
                entry1.insert(0, mensaje)
                cambio = 1
            else:
                entry2.delete(0, 'end')
                entry2.insert(0, mensaje)
                cambio = 0
        except UnicodeDecodeError as e:
            print(f"Error de decodificación: {e}")
        except Exception as e:
            print(f"Error: {e}")

#---------------------------------------------------------

#------------
# Configure arduino
#------------
hilo1 = threading.Thread(target=pots, daemon=True)

arduino = serial.Serial('COM4', 9600)
hilo1.start()

#---------------------------------------------------------

#------------
# Contador de 8 bits
#------------
def contador():  # Función que se activa cuando se presiona el botón del contador
    n1 = entry.get()
    n2 = 0
    
    if n1.isdigit():  # Verifica si n1 es un número
        n2 = int(n1)
    else:
        lbl158.delete(0, 'end')
        lbl158.insert(0, "Debe ser un número")  # Mensaje de error
        return  # Sale de la función si la entrada no es válida
    if n2 > 255:
        lbl158.delete(0, 'end')
        lbl158.insert(0, "El número debe ser de 0 a 255")
        return
    
    # Dividir el número en 3 números individuales
    centenas = int(n2 / 100)
    decenas = int((n2 - (centenas * 100)) / 10)
    unidades = int(n2 - (centenas * 100 + decenas * 10))
    
    arduino.write(bytes('A', 'utf-8'))  # Decirle a Arduino que se va a enviar algo al contador
    time.sleep(0.1)
    
    # Enviar el valor del número separado a Arduino
    arduino.write(bytes(str(centenas), 'utf-8'))
    time.sleep(0.1)
    arduino.write(bytes(str(decenas), 'utf-8'))
    time.sleep(0.1)
    arduino.write(bytes(str(unidades), 'utf-8'))
    
    lbl158.delete(0, 'end')
    lbl158.insert(0, "Mostrando número")

#---------------------------------------------------------

#------------
# Widgets in GUI
#------------





lbl1 = Label(root, text="Valor Potenciómetro 1:", bg="yellow")
lbl1.place(x=50, y=70, width=130, height=30)

lbl1 = Label(root, text="Valor Potenciómetro 2:", bg="yellow")
lbl1.place(x=50, y=120, width=130, height=30)

btn = Button(root, text="Enviar dato al contador", command=contador, bg="yellow")
btn.place(x=100, y=170, width=200, height=30)

entry = Entry(root)
entry.place(x=170, y=210, width=50, height=30)

lbl158 = Entry(root, bg='#fdf2e9')
lbl158.place(x=100, y=250, width=200, height=30)

root.mainloop()
