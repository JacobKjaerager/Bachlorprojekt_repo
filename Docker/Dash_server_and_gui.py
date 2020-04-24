# -*- coding: utf-8 -*-
"""
Created on Thu Apr  9 14:31:06 2020

@author: Jacob Kjærager
"""
import os
import pandas as pd
from datetime import datetime 

import dash
import plotly.graph_objs as go
import dash_core_components as dcc
import dash_html_components as html
from dash.dependencies import Input, Output

##Supporting functions##
def get_options():
    df = get_df()
    a = []
    for col in df.columns:
        a.append({"label": col, "value": col})

    return pd.Series(a)

def get_df():
    currently_dropped_columns = ["Unnamed: 0", "index", "level_0", "type", "payload__applicationID", 'payload__rxInfo__rfChain',
                                  'payload__applicationName', 'payload__deviceName', 'payload__devEUI',
                                  'payload__devAddr', 'payload__rxInfo__gatewayID', 'payload__rxInfo__timeSinceGPSEpoch',
                                  'payload__rxInfo__board', 'payload__rxInfo__antenna', 'payload__rxInfo__location__altitude',
                                  'payload__rxInfo__location__source', 'payload__rxInfo__location__accuracy',
                                  'payload__rxInfo__fineTimestampType',  'payload__rxInfo__context', "Battery",
                                  'payload__rxInfo__uplinkID', 'payload__rxInfo__crcStatus', 'payload__txInfo__modulation',
                                  'payload__txInfo__loRaModulationInfo__bandwidth', 'payload__txInfo__loRaModulationInfo__spreadingFactor',
                                  'payload__txInfo__loRaModulationInfo__codeRate', 'payload__txInfo__loRaModulationInfo__polarizationInversion',
                                  'payload__adr', 'payload__dr',  'payload__fCnt', 'payload__fPort', 'payload__data', 'payload__objectJSON',
                                  'payload__rxInfo__location__latitude', 'payload__rxInfo__location__longitude', 'payload__rxInfo__time'
                                  ]
    dfs = []
    premade_file_directoy = 'premade_test_files'
    for filename in os.listdir(premade_file_directoy):
        dfs.append(pd.read_csv(os.getcwd()+ "/" + premade_file_directoy + "/"+filename))

    df = pd.concat(dfs, axis=0).reset_index().drop(currently_dropped_columns, axis=1)
    
    return df

def get_unit(dd_value):
    if (dd_value == 'payload__rxInfo__time'):
        unit = ""
    elif(dd_value == 'payload__rxInfo__rssi'):
        unit = " dBm"
    elif(dd_value == 'payload__rxInfo__loRaSNR'):
        unit = ""
    elif(dd_value == 'payload__rxInfo__channel'):
        unit = ""
    elif(dd_value == 'payload__rxInfo__rfChain'):
        unit=""
    elif(dd_value == 'payload__txInfo__frequency'):
        unit=" Hz"
    elif(dd_value == 'Latitude'):
        unit=" N"
    elif(dd_value == 'Longitude'):
        unit=" W"
    elif(dd_value == 'Battery'):
        unit=" mV"
    elif(dd_value == 'distance'):
        unit=" m"
    else:
        unit=" unconfigured unit"
    return unit

def plot_radio(dd_value, slider_value):
    df = get_df()
    
    df = df[df[dd_value] > float(slider_value[0])]
    df = df[df[dd_value] < float(slider_value[1])] #Find solution to this problem!! 

    mapbox_access_token = "pk.eyJ1IjoiamFjb2I3NCIsImEiOiJjazhzcW9peXgwMjF2M21wOGdxenozMWRpIn0.cvEeafk5_3_FS-zkcOE6Jw"
    data = [
        go.Scattermapbox(
            lat=df["Latitude"],
            lon=df["Longitude"],
            mode='markers',
            marker=go.scattermapbox.Marker(
                size=11,
                color=df[dd_value],
                colorbar=dict(
                        title=dd_value
                ),
                colorscale="Jet"
            ),
           hovertext = df[dd_value].astype(str) + get_unit(dd_value)
           #"<br> dist2GW: " + round(df["distance"],1).astype(str) + "m"+
           #"<br> SNR: " + df["payload__rxInfo__loRaSNR"].astype(str)
        ),
        go.Scattermapbox( #Layer 2, implemented for placing the concentrator and could be expanded to support of multiple concentrators.
            mode ="markers",
            lat = [56.155737], 
            lon = [10.189122],
            marker = {'size': 14, 'symbol': ["fire-station"]},
            text = pd.Series(["Concentrator_placement"]),    
        )
    ]
    layout = go.Layout(
        height= 864, #80% of screen is used for map !!!OBS(Only works on Windows.)
        width = 1536, #80% of screen is used for map
        autosize=True,
        hovermode='closest',
        showlegend = False,
        mapbox=dict(
            accesstoken=mapbox_access_token,
            bearing=0,
            center=dict(
                lat = 56.155737,  #Physical placement of Gateway.
                lon = 10.189122,  #Physical placement of Gateway.
            ),
        pitch=0,
        zoom=16,
        ),
    )
    fig = go.Figure(layout=layout, data=data)
    
    return fig

#Dash app#
app = dash.Dash()
app.layout = html.Div([
    html.Table([   
        html.Tr([   
            html.Td([
                dcc.RangeSlider(
                    id='slider',
                    value=[-110, -80] # Default to before callback is executed, will never be seen but avoids an initial NoneType object as value in the callback
                ), 
            ], style={
                   'width': '20vw', 
                   'float':'left',
                   'margin-right': '3vw',
                   'margin-top': '2vw',
                   'margin-left': '2vw',
               }
            ),
            html.Td([
                html.Div(id = "value_of_slider"),
            ], style={
                   'width': '40vw', 
                   'float':'left',
                   'margin-top': '2vw',
                }
            ),
        ], style={
               'width': '100vw',
               
            }
        ),
        html.Tr([
            html.Td([
                dcc.Dropdown(
                    id='dropdown',
                    options=get_options(),
                    value='payload__rxInfo__rssi' 
                ), 
            ], style={
                   'float': 'left', 
                   'width': '20vw',
                   'margin-top': '2vw',
                   'margin-left': '2vw'
                }
            ),
            html.Td([
                dcc.Graph(
                    id="map",
                )
            ], style={
                   'float': 'right',
                }
            ),
        ], style={
               'width': '100vw',
            
            }
        ),
    ]),    
], style={
        'height': '100vw', 
        'width': '100vw',
    }
)

##Callbacks##
@app.callback(
    Output('map', 'figure'),
    [Input('dropdown', 'value'),
     Input('slider', 'value')])
def update_map(dd_value, slider_value):
    
    return plot_radio(dd_value, slider_value)

@app.callback([Output('slider', 'min'),
               Output('slider', 'max'),
               Output('slider', 'step'),
               Output('slider', 'value')],
              [Input('dropdown', 'value')])
def update_slider(dd_value):
    df = get_df()
    min = df[dd_value].min()
    max = df[dd_value].max()
    step = (max - min)/100 #100 steps on slider
    value = [round(min,3), round(max,3)]
    
    return [min, max, step, value]

@app.callback(
    Output('value_of_slider', 'children'),
    [Input('dropdown', 'value'),
     Input('slider', 'value')])
def update_label_for_slider(dd_value, slider_val):
    return "Displaying values of: \"" + str(dd_value) + "\" values between: " + str(slider_val) + get_unit(dd_value)

if __name__ == '__main__':    
    
    
    ##Turn on Dash-Flask server##    
    #app.run_server(debug=True) #For developing the application further
    app.run_server(host="0.0.0.0", debug=True, port=8080) #For docker configuration
        
