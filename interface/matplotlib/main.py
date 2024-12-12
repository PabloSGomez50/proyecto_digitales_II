import serial
import click
import pandas as pd
# import datetime
import time


port = 'COM3'  # Revisar en el administrador de dispositivos
baud_rate = 9600

@click.group()
def cli():
    pass

def read_serial(ser):
    return ser.readline().decode('utf-8').strip() if ser.in_waiting > 0 else None

def format_lidar_data(data: str):

    keys = [
        "range",
        "distance",
        "signal_rate",
        "ambient_light",
        "spad_num",
        "mot_angle"
    ]
    if isinstance(data, str) and data[0] == '$':
        return {key: item for key, item in zip(keys, data[1:].split(', '))}
    else:
        return dict()

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

# test_sensor()

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