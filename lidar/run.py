from client import Client
from GUI import UserInterface

import time
import sys
import ast

# Initiate GUI-class
GUI = UserInterface()

# Initiate Client-class
client = Client()

# input_string: "deg1, dist1, type1, 
#                deg2, dist2, type2,
#                hastighet, portdistance, avstånd till mitten, 
#                vinkel till port, varv, previous cone"
def run_client():
    if GUI.QUIT_PRESSED:
        client.send(client.DISCONNECT_MESSAGE)
        print(client.DISCONNECT_MESSAGE)
        time.sleep(0.1)
        sys.exit()

    if GUI.mode_changed:
        client.send(GUI.mode)
        GUI.mode_changed = False
    
    else:
        client.send(GUI.KEY_PRESSED)
        
    time.sleep(0.1)
    client.message = client.recieve()
    # This will be a list of values instead of a list as a string
    #dash_board_values = ast.literal_eval(client.recieve())
    #dash_board_values = client.getList(client.recieve())
    #GUI.update_dash_board(dash_board_values)

#print(f"[NEW CONNECTION] {client.ADDR} connected.")
while True:
    #GUI.GUI_loop("90, 100, SMALL, 90, 2100, BIG, 100, 2, 30, 2, 3")
    GUI.GUI_loop(client.message)
    run_client()
