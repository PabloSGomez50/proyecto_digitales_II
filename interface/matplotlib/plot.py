import serial
import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

def format_df(df: pd.DataFrame):
    df['mot_angle'] = df["mot_angle"] / 10
    df['angle'] = np.pi * df["mot_angle"] / 180
    # df['diff_mot'] = df['mot_angle'].diff(-1)
    df["x"] = df["distance"] * np.cos(df['angle'])
    df["y"] = df["distance"] * np.sin(df['angle'])
    df["cuadrant"] = (df["mot_angle"] // 900) + 1 

    return df

def get_initial_df():
    df = pd.read_csv("lidar_data.csv")
    df = format_df(df)
    df.to_csv("lidar_data_clean.csv")
    return df


def read_serial(ser):
    return ser.readline().decode('utf-8').strip() if ser.in_waiting > 0 else None

def format_lidar_data(data: str):

    keys = [
        "range",
        "distance",
        "signal_rate",
        "ambient_light",
        "spad_num",
        "mot_angle",
        "lap_num"
    ]
    if isinstance(data, str) and data[0] == '$':
        return {key: int(item) for key, item in zip(keys, data[1:].split(', '))}
    else:
        return dict()