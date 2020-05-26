import paho.mqtt.client as mqtt #import the client

import plotly.graph_objs as go
import json
import webbrowser
from plotly.offline import iplot

#def prompter():
#    webbrowser.open("temp-plot.html")
    
#Dash app#
def plot_radio(lat, lon):
    fig = go.Figure(
            go.Scattermapbox( #Layer 2, implemented for placing the concentrator and could be expanded to support of multiple concentrators.
                mode ="markers",
                lat = [lat], 
                lon = [lon],
                marker = {'size': 14, 'symbol': ["fire-station"]},
                text = "DER ER EN DER ER FALDET HER!!!!!!",    
            ))
    fig.update_layout(
        autosize=True,
        hovermode='closest',
        showlegend = False,
        mapbox=dict(
            accesstoken= "pk.eyJ1IjoiamFjb2I3NCIsImEiOiJjazhzcW9peXgwMjF2M21wOGdxenozMWRpIn0.cvEeafk5_3_FS-zkcOE6Jw",
            bearing=0,
            center=dict(
                lat = lat,
                lon = lon,
            ),
        pitch=0,
        zoom=16,
        ),
    )
    iplot(fig)


def on_message(client,userdata,msg):
       global m_decode
       topic=msg.topic
       message = msg.payload.decode("utf-8","ignore")
       object_json = str(message).split("\"objectJSON\":\"")[1].split("\",\"")[0]
       dict_json = json.loads(object_json.replace("\\",""))
       lat = dict_json["Lat"]
       lon = dict_json["Lon"]
       plot_radio(lat, lon)
       print("Lat is currently: {} \nLon is currently: {}".format(lat, lon))

broker_address="127.0.0.1:1883"

client = mqtt.Client("paclido") #create new instance
client.on_message=on_message

print ("connected to broker", broker_address)
client.connect("127.0.0.1", 1883, 60)
client.subscribe([("application/1/#", 0), ("application/1/device/70b3d549992371f8/rx", 0)])  
client.loop_forever()












