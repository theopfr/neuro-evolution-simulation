import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import json
import time
import os

plt.style.use("ggplot")


simulation_config_file = "./src/config.json"
with open(simulation_config_file, "r") as f:
    config = json.load(f)

simulation_data_file = "./data/simulation_data.json"

last_change_stamp = 0
last_simulation_iteration = 0

total_plot_indices = []
total_organism_amounts = []

fig = plt.figure()
ax1 = fig.add_subplot(3, 2, 1)
ax2 = fig.add_subplot(3, 2, 2)
ax3 = fig.add_subplot(3, 2, 3)
ax4 = fig.add_subplot(3, 2, 4)
ax0 = fig.add_subplot(3, 2, 5)


def plotter(idx: int) -> None:
    global last_change_stamp
    global last_simulation_iteration

    stamp = os.stat(simulation_data_file).st_mtime
    if stamp != last_change_stamp:
        last_change_stamp = stamp

        try:
            with open(simulation_data_file, "r") as f:
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
            ax0.set_title("organism amount")

            # plot histogram of the organism sizes
            sizes = data[simulation_iteration]["sizes"]
            ax1.clear()
            ax1.hist(sizes, bins=6, color="blue", alpha=0.45)
            ax1.set_xlim([config["universalMinAdultSize"], config["universalMaxAdultSize"]])
            ax1.set_title("organism sizes")

            diet = data[simulation_iteration]["diets"]
            ax2.clear()
            ax2.hist(diet, bins=5, color="blue", alpha=0.45)
            ax2.set_xlim([0, 1])
            ax2.set_title("organism diets")

            sight_angles = data[simulation_iteration]["sightAngles"]
            ax3.clear()
            ax3.hist(sight_angles, bins=6, color="blue", alpha=0.45)
            ax3.set_xlim([config["universalMinSightAngle"], config["universalMaxSightAngle"]])
            ax3.set_title("organism sight angles")

            sight_reaches = data[simulation_iteration]["sightReaches"]
            ax4.clear()
            ax4.hist(sight_reaches, bins=6, color="blue", alpha=0.45)
            ax4.set_xlim([config["universalMinSightReach"], config["universalMaxSightReach"]])
            ax4.set_title("organism sight reaches")

        except Exception as e:
            print(e)



ani = FuncAnimation(fig, plotter, interval=0.01)
plt.tight_layout()
plt.show()