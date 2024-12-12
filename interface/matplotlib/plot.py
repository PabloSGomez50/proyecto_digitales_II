import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

def format_df(df: pd.DataFrame):
    df['angle'] = np.pi * df["mot_angle"] / 1800
    df['diff_mot'] = df['mot_angle'].diff(-1)
    index_lap = 0
    laps = list()
    cuadrant = list()
    df["x"] = df["distance"] * np.cos(df['angle'])
    df["y"] = df["distance"] * np.sin(df['angle'])

    for idx, item in df.iterrows():
        if item["diff_mot"] > 0:
            index_lap += 1
        laps.append(index_lap)
        if item["x"] >= 0:
            if item["y"] >= 0:
                cuadrant.append(1)
            else:
                cuadrant.append(4)
        elif item["x"] < 0:
            if item["y"] >= 0:
                cuadrant.append(2)
            else:
                cuadrant.append(3)

    df["laps"] = laps
    df["cuadrant"] = cuadrant
    return df

def get_initial_df():
    df = pd.read_csv("lidar_data.csv").loc[:315]
    df = format_df(df)
    df.to_csv("lidar_data_clean.csv")
    return df