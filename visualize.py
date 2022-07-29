import matplotlib.pyplot as plt
import json
import time
import os


file_name = "./data/simulation_data.json"

last_change_stamp = 0
while True:
    stamp = os.stat(file_name).st_mtime

    if stamp != last_change_stamp:
        last_change_stamp = stamp
        try:
            with open(file_name, "r") as f:
                data = json.load(f)
                print(data.keys())
        except:
            pass


