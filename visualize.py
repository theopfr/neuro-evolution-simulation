import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import json
import numpy as np
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
ax0 = fig.add_subplot(3, 2, 1)
ax1 = fig.add_subplot(3, 2, 2)
ax2 = fig.add_subplot(3, 2, 3)
ax3 = fig.add_subplot(3, 2, 4)


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

            ax0.plot(total_plot_indices, total_organism_amounts, color="mediumaquamarine")
            ax0.set_title("organism amount")

            # plot histogram of organism sizes
            sizes = data[simulation_iteration]["sizes"]
            ax1.clear()

            ax1.hist(sizes, bins=6, color="blue", alpha=0.45)
            ax1.axvline(np.mean(sizes), color="mediumaquamarine", linestyle="dashed", linewidth=1.25)

            ax1.set_xlim([config["universalMinAdultSize"], config["universalMaxAdultSize"]])
            ax1.set_title(f"organism sizes, average: {round(np.mean(sizes), 3)}")

            # plot histogram of organism diets
            diet = data[simulation_iteration]["diets"]
            ax2.clear()

            ax2.hist(diet, bins=5, color="blue", alpha=0.45)
            ax2.axvline(np.mean(diet), color="mediumaquamarine", linestyle="dashed", linewidth=1.25)

            ax2.set_xlim([0, 1])
            ax2.set_title(f"organism diets, average: {round(np.mean(diet), 3)}")

            # plot histogram of organism sight reaches
            sight_reaches = data[simulation_iteration]["sightReaches"]
            ax3.clear()

            ax3.hist(sight_reaches, bins=6, color="blue", alpha=0.45)
            ax3.axvline(np.mean(sight_reaches), color="mediumaquamarine", linestyle="dashed", linewidth=1.25)

            ax3.set_xlim([config["universalMinSightReach"], config["universalMaxSightReach"]])
            ax3.set_title(f"organism sight reaches, average: {round(np.mean(sight_reaches), 3)}")

        except Exception as e:
            print(e)



ani = FuncAnimation(fig, plotter, interval=0.01)
plt.tight_layout()
plt.show()