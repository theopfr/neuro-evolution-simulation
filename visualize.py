import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import json
import time
import os



file_name = "./data/simulation_data.json"

last_change_stamp = 0
last_simulation_iteration = 0

total_plot_indices = []
total_organism_amounts = []

sub_plot_config = [5, 1, 0]

fig = plt.figure()
"""ax1 = fig.add_subplot(5, 1, 1)
ax2 = fig.add_subplot(5, 1, 2)
ax3 = fig.add_subplot(5, 1, 3)
ax4 = fig.add_subplot(5, 1, 4)
ax5 = fig.add_subplot(5, 1, 5)"""
ax1 = fig.add_subplot(3, 2, 1)
ax2 = fig.add_subplot(3, 2, 2)
ax3 = fig.add_subplot(3, 2, 3)
ax4 = fig.add_subplot(3, 2, 4)
ax0 = fig.add_subplot(3, 2, 5)


def plotter(idx: int) -> None:
    global last_change_stamp
    global last_simulation_iteration

    stamp = os.stat(file_name).st_mtime
    if stamp != last_change_stamp:
        last_change_stamp = stamp

        try:
            with open(file_name, "r") as f:
                data = json.load(f)

            simulation_iteration = list(data.keys())[0]
            if simulation_iteration == last_simulation_iteration:
                return
            
            last_simulation_iteration = simulation_iteration

            # plot amount of living organisms over over time
            organism_amount = data[simulation_iteration]["organismAmount"]
            total_plot_indices.append(idx)
            total_organism_amounts.append(organism_amount)
            ax0.plot(total_plot_indices, total_organism_amounts, color="red")

            # plot histogram of the organism sizes
            sizes = data[simulation_iteration]["sizes"]
            ax1.hist(sizes, bins=6, color="red")

            diet = data[simulation_iteration]["diets"]
            ax2.hist(diet, bins=2, color="red")

            sight_angles = data[simulation_iteration]["sightAngles"]
            ax3.hist(sight_angles, bins=6, color="red")

            sight_reaches = data[simulation_iteration]["sightReaches"]
            ax4.hist(sight_reaches, bins=6, color="red")

            print(organism_amount)

        except Exception as e:
            print(e)



ani = FuncAnimation(fig, plotter, interval=0.01)
plt.tight_layout()
plt.show()