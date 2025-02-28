
import serial
import click
import pandas as pd
# import datetime
import time
from plot import read_serial, format_lidar_data

port = 'COM4'  # Revisar en el administrador de dispositivos
baud_rate = 9600


@click.group()
def cli():
    pass

@cli.command()
def get_available_ports():
    import serial.tools.list_ports
    ports = []
    for port in serial.tools.list_ports.comports():
        print("Port:", port.device)
        try:
            with serial.Serial(port.device, baud_rate, timeout=0.1) as ser:
                ser.write("STATUS\n".encode('utf-8'))
                
                if value := read_serial(ser):
                    print(f"{port.device} - {value}")
                    ports.append(port.device)
        except serial.SerialException:
            print(f"Error in {port.device}")
    return ports

@cli.command()
@click.option('--port', '-p', default=port,  help='Puerto serial')
@click.option('--baud', '-b', default=baud_rate, help='Bits por segundos marcados en arduino', type=int)
def read_sensor(port: str, baud: int):
    print("--Start--")
    data = list()
    with serial.Serial(port, baud, timeout=1) as ser:
        print('--Connection--')
        ser.write("ACTIVE\n".encode('utf-8'))
        value = None
        try:
            while True:
                value = read_serial(ser)
                if value := format_lidar_data(value):
                    data.append(value)
        except KeyboardInterrupt:
            print('Finish comunication')
            df = pd.DataFrame(data)
            df.to_csv('lidar_data.csv', index=False)
            ser.write("IDLE\n".encode('utf-8'))


@cli.command()
@click.option('--port', '-p', default=port,  help='Puerto serial')
@click.option('--baud', '-b', default=baud_rate, help='Bits por segundos marcados en arduino', type=int)
def test_sensor(port: str, baud: int ):
    # Iniciar secuencia de calibracion
    print("--Start--")
    with serial.Serial(port, baud, timeout=1) as ser:
        ser.write("ACTIVE\n".encode('utf-8'))
        value = None
        while value is None :
            value = read_serial(ser)
        print("--Info received--:", value)
        time.sleep(3)
        ser.write("IDLE\n".encode('utf-8'))
    print("--End--")

@cli.command()
@click.option('--port', '-p', default=port,  help='Puerto serial')
@click.option('--baud', '-b', default=baud_rate, help='Bits por segundos marcados en arduino', type=int)
def stop(port, baud):
    with serial.Serial(port, baud, timeout=1) as ser:
        ser.write("IDLE\n".encode('utf-8'))

@cli.command()
@click.option('--steps', '-s', type=int)
@click.option('--port', '-p', default=port,  help='Puerto serial')
@click.option('--baud', '-b', default=baud_rate, help='Bits por segundos marcados en arduino', type=int)
def steps(steps, port, baud):
    with serial.Serial(port, baud, timeout=1) as ser:
        ser.write(f"M{steps}.".encode('utf-8')) 


if __name__ == '__main__':
    print('Boot')
    cli()