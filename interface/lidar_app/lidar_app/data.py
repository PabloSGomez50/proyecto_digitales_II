import pandas as pd
import numpy as np
import plotly.express as px
import plotly.graph_objects as go

def generate_wall_data():
    # Define the corner points of the rectangular room (x, y)
    size = 100
    corners = np.array([
        [-size, -size],       # Bottom-left corner
        [size, -size],      # Bottom-right corner
        [size, size],      # Top-right corner
        [-size, size],       # Top-left corner
        [0, 0]        # Closing the loop back to the bottom-left corner
    ])
    
    # Define the number of points per wall segment (increase for more detail)
    points_per_wall = 50
    
    # Create walls by interpolating points between each pair of corners
    wall_points = []
    for i in range(len(corners) - 1):
        x_values = np.linspace(corners[i][0], corners[i+1][0], points_per_wall)
        y_values = np.linspace(corners[i][1], corners[i+1][1], points_per_wall)
        wall_segment = np.column_stack((x_values, y_values))
        
        # Optional: add slight random variation to simulate imperfections
        wall_segment += np.random.normal(0, 0.1, wall_segment.shape)
        
        wall_points.append(wall_segment)
    
    # Combine all wall points into a single array
    walls = np.vstack(wall_points)
    return walls


def generate_random_data():
        # Generate linspace array from 0 to 360 with steps of 1.8
    linspace_array = np.arange(0, 360 + 1.8, 1.8)
    
    # Generate an array of random values between 20 and 300, same length as linspace_array
    random_array = np.random.uniform(20, 300, len(linspace_array))
    
    # Stack them together as two rows
    combined_array = np.vstack((linspace_array, random_array)).T

    return combined_array
def generate_data():

    array = generate_wall_data()
    df = pd.DataFrame(array, columns=["angle", "dist"])
    df = polar2catersian(df).round(2)
    df.to_csv('data/coords.csv', index=False)
    return df

def polar2catersian(df: pd.DataFrame):

    df["x"] = df['dist'] * np.cos(df['angle'] * np.pi / 180)
    df["y"] = df['dist'] * np.sin(df['angle'] * np.pi / 180)

    return df

def plot_data(df: pd.DataFrame):

    plot = px.scatter(df, 'x', 'y')
    return plot