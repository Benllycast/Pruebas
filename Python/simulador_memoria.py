import sys 
import os
import threading
import struct
import time
import argparse
import serial

if sys.version_info >= (3, 0):
    def character(b):
        return b.decode('latin1')
else:
    def character(b):
        return b

class Simulador(object):
    """docstring for Simulador"""
    def __init__(self, puerto = 'COM4', baudrate = 9600):
        super(Simulador, self).__init__()
        self.port = puerto
        self.baudrate = baudrate
        self.serial = None
        self.primera_lectura = True
    
    def _start_reader(self):
        """Start reader thread"""
        self._reader_alive = True
        # start serial->console thread
        #self.receiver_thread = threading.Thread(target=self.reader)
        #self.receiver_thread = threading.Thread(target=self.iniciar_simulacio)
        #self.receiver_thread.setDaemon(True)
        #self.receiver_thread.start()

    def _stop_reader(self):
        """Stop reader thread only, wait for clean exit of thread"""
        self._reader_alive = False
        self.receiver_thread.join()

    def start(self):
        self.alive = True
        self._start_reader()
        # enter console->serial loop
        #self.transmitter_thread = threading.Thread(target=self.writer)
        #self.transmitter_thread.setDaemon(True)
        #self.transmitter_thread.start()

    def stop(self):
        self.alive = False
        self._stop_reader()

    def join(self, transmit_only=False):
        #self.transmitter_thread.join()
        if not transmit_only:
            self.receiver_thread.join()

    def iniciar_simulacio(self):
        data = ''
        try:
            self.iniciar_puerto()
            while self.alive and self._reader_alive:

                while True:
                    dumy = self.serial.read(1)
                    sys.stdout.write(dumy)
                    if dumy == '\r':
                        break;
                    else:
                        data = data + dumy
                sys.stdout.write('leido %s ...\n\r' % data)
                if data == '\x04':
                    sys.stdout.write('terminando...')
                    self.alive = False
                else:
                    respuesta = '\n\rack '+ data
                    self.serial.write(respuesta)
                    data = ''        
        except serial.SerialException, e:
            self.alive = False
            raise

    def log_reader(self, caracteres = 1):
        c = self.serial.read(caracteres)
        sys.stdout.write('\n>%s ' % str(c))
        sys.stdout.flush()
        return c

    def log_writer(self, caracteres):
        for c in caracteres:
            sys.stdout.write('\n<%s (%s)' % (str(c), ord(c)))
            self.serial.write(c)
            time.sleep(0.2)
        sys.stdout.flush()
        
    def reader(self):
        data = ''
        try:
            self.iniciar_puerto()
            while self.alive and self._reader_alive:
                sys.stdout.write('\n\r---Nueva Operacio---\n\r ')
                data = self.log_reader()
                # autobaudrate
                if data == '\x55':
                    self.log_writer('\x06')
                    self.primera_lectura = True
                elif data == '\x40':
                    # extende command
                    comando = self.log_reader()
                    if comando == '\x61':   
                        # comando de lectura
                        self.leer_memoria()
                    elif comando == '\x74':
                        # comando de escritura
                        self.escribir_memoria()
                    elif comando == '\x69':
                        # comando de inicializacion
                        self.log_writer('\x06')
                    else:
                        self.log_writer('\x15')
                else:
                    self.log_writer('\x15')
                self.serial.flush()
                data = ''
        except serial.SerialException, e:
            self.alive = False
            raise

    def leer_memoria(self):
        respuesta = 'holamundo'
        tamano = struct.pack('!I',len(respuesta))
        for c in tamano:
            sys.stdout.write('\n-- %s' % ord(c))
        nombre = ''
        c = ''
        handshakin = self.log_reader(1)
        while True:
            c = self.log_reader(1)
            if c == '\x00':
                break
            else:
                nombre = nombre + c
        if self.primera_lectura:
            self.log_writer('\x00')
            self.log_writer('\x00')
            self.primera_lectura = False
        sys.stdout.write('\nLectura: File: %s HS: %s' % (nombre, ord(handshakin)) )
        self.log_writer(tamano)
        ack = self.log_reader(1)
        if ack == '\x06':
            self.log_writer(respuesta)
            self.log_writer('\x06')

    def escribir_memoria(self):
        datos = ''
        opciones = ord(self.log_reader(1))
        handshking = opciones & 0x3f
        append = (opciones & 0x80) >> 7
        performance = (opciones & 0x40) >> 6
        nombre = ''
        c = ''
        while True:
            c = self.log_reader(1)
            if c == '\x00':
                break
            else:
                nombre = nombre + c
        tamano = self.log_reader(4)
        tamano = int(struct.unpack('!I',tamano)[0])
        sys.stdout.write('\nEscritura: FL: %s TM: %d HS: %d AP: %d PER: %d\n' % (nombre, tamano, handshking, append, performance))
        # self.log_writer('\x15')
        self.log_writer('\x06')
        while tamano > 0:
            datos = datos + self.log_reader(1)
            tamano -= 1
        sys.stdout.write('\nDatos: %s' % datos)
        self.log_writer('\x06')

    def inciar_memoria(self):
        pass

    def writer(self):
        while self.alive:
            sys.stdout.write('escribiendo datos\n')

    def iniciar_puerto(self):
        if not self.serial:
            try:
                self.serial = serial.Serial(port=self.port, baudrate=self.baudrate)
            except serial.SerialException, e:
                print str(e)
                raise

     

def main():
    parser = argparse.ArgumentParser(description='simuladore de memoria virtual', prog='SIMULADOR DE MEMORIA')
    parser.add_argument('-p', '--port', dest='puerto', default='COM6')
    parser.add_argument('-b','--baudrate', dest='baudrate', default=9600, type=int)
    comando = parser.parse_args()
    print "--port  = %s, --baudrate = %d " % (comando.puerto, comando.baudrate)
    print 'inciando simulacion'
    simulador = Simulador(puerto = comando.puerto, baudrate = comando.baudrate)
    simulador.start()
    simulador.reader()
    #simulador.iniciar_simulacio()
    #simulador.stop()
    print 'fin de ejecucion'

if __name__ == "__main__":
    main()
