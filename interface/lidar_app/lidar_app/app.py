from dash import Dash, html, dcc, callback, Output, Input
from datetime import datetime
import pandas as pd
import plotly.express as px
import os

from data import generate_data, plot_data

app = Dash(__name__)

df = generate_data()

app.layout = html.Div([
    
    html.Header([
        html.Img(src="/assets/franquito_williams.jpeg", className="icon"),
        html.H1("Lidar content", className="header-title")
    ]),

    html.Div(children=[
                html.Div([
            html.H3("Configuracion", className="sidebar-title"),
            html.Div(id="output", className="output_content"),

            html.Label("Cambios de parametros"),
            html.Div(children=[
                html.Label("Velocidad de giro:"),
                dcc.Input(id='input_rot_speed', type='number', value=2, className="input_field"),
                html.Label("[Rev por sergundo]"),
            ], className="param_container"),


            html.Button("actualizar", id='data_button', className="btn"),
        ], className="sidebar"),

        html.Div(children=[
            dcc.Graph(id='graph-content', figure=plot_data(df)),
            
        ], className='dashboard')
    ], className='content')
], className='container')

@callback(
    Output('graph-content', 'figure'),
    Input('data_button', 'n_clicks')
)
def update_graph(n):
    df = generate_data()
    return plot_data(df)

if __name__ == '__main__':
    app.run(debug=True)