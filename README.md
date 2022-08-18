# Evolution simulator using a real-time neuro-genetic algorithm

<img src="/images/simulation-recording.gif" height="350"/>


## ⬇️ Setup:
#### 1. Installation (Linux):

###### (the installation was not tested on Windows or MacOS, follow [this guide](https://bernhardfritz.github.io/piksel/#/gettingstarted/buildproject) to build on your operating system)

1. Clone this repository and navigate into it:
    ```
    git clone --recursive https://github.com/theopfr/neuro-evolution.git
    cd neuro-evolution/
    ```

2. Navigate to the "lib/" folder and clone the "Piksel" libary into it:
    ```
    cd lib
    git clone --recursive https://github.com/bernhardfritz/piksel.git
    ```
3. Install [CMake](https://cmake.org/install/) if you don't have it yet

#### 2. Create Makefile:
Navigate to the "build_native/" folder and execute:
```
cmake ..
```

## ▶️ Running the simulation:
#### Compiling:
Navigate to the "build_native/" folder.
To compile the program after doing code changes execute:
```
make
```

#### Executing:
Navigate to the "build_native/" folder.
To run the program execute:
```
./index
```

You can change the simulation configurations inside [./src/config.json"](./src/config.json) without having the recompile the program.
To see which configurations there are check out the table at the end of the README (TODO).


## 📜 Rules:
All variables mentioned below can be configured and experimented with. Check them out in the table at the end of the README (TODO).
1. An organism looses a specific amount of energy which starts at ``1.0``. The amount of energy loss each iteration starts at ``initialEnergyLoss`` (default: ``0.004``) and gets increased the following way: 
``energyLoss = 1/400000 * iteration + initialEnergyLoss``
If the energy reaches ``0.0`` the organism dies and it converts to meat based food.
2. If an organism runs into plant based or meat based food it gains specific amount of energy somewhere between ``0.0`` and ``maxEnergyGainByFood`` (default: ``0.5``) depending on its ``diet`` gene (``diet = 0.0`` means 100% herbivorous, ``diet = maxEnergyGainByFood`` means 100% carnivorous).
3. If an organism runs into another and both are breedable (which means their breedability is ``1.0``) they produce a child (when a new organism is born its breedability starts at ``0.0`` and increases by ``0.005`` each time-step until it reaches ``1.0`` indicating adulthood).
4. If two organisms produce a child their breedability is st to ``0.0`` but regenerates by ``breedabilityRegeneration`` (default: ``0.05``) each iteration until it is at ``1.0`` again.
5. A new child inherits its brain and genes from the parent with the highest energy.
6. When a brain and genes are inherited by a child there is a change of ``geneMutationProbability``% (default: ``20``%) that a gene gets mutated by adding or subtracting ``geneMutationFactor`` (default: ``0.05``). Same for the brain synapses with the mutation chance of ``brainMutationProbability``% (default: ``7``%) and mutation factor ``brainMutationFactor`` (default: ``0.01``).
7. If an organism runs into a wall it looses ``wallDamage`` (default: ``0.5``) energy.

## 🧬 Genes:
All gene ranges can be configured and experimented with. Check them out in the table at the end of the README (TODO).
1. ``diet``: A value in range ``[0.0; 1.0]``. The closer it is to ``0.0`` the more energy the organism gains from plant based food. The closer to ``1.0`` the more energy it gains from meat. The range ``[0.0; 1.0]`` gets mapped to ``[0.0; maxEnergyGainByFood]`` to calculate how much energy is gained. 
2. ``size``: A value in range ``[universalMinAdultSize; universalMaxAdultSize]`` (default: ``[12.0; 23.0]``) which represents the max adult size of an organism. The size also determines the speed of an organism by getting mappend to the speed range of ``[minSpeed, maxSpeed]`` (default: ``[2.0, 4.0]``). So the smaller the organism the faster it is.
3. ``sightReach``: A value in range ``[universalMinSightReach; universalMaxSightReach]`` (default: ``[50.0; 100.0]``) which determines at which maximum distance the organisms can detect food (range of the two sensors).

## 👁️ Senses:
An organism has only one sense: Sight.
If food enters ``sightReach`` of an organism it can detect it by processing the distance from the end of both sensors. 

Diagram:
<img src="/images/organism.png" height="400"/>

## 🧠 Brain:
###### (the architecture might change if new inputs and/or outputs are added, but I'm too lazy to update the illustration each time)
The neural network of an organism has the following fixed topology:
- input-nodes: ``3`` (distance from left sensor, distance from right sensor, reccurent memory value t-1)
- hidden-nodes: ``hiddenSize`` (default: ``6``), tanh activation
- output-nodes: ``2``, sigmoid activation (turn angle, reccurent value t)

Diagram:
<img src="/images/brain.png" height="400"/>

## 📊 Simulation data visualization:
When you start the simulation, you can simutainuously run a Python script to plot relevant data.
Make sure that you have the following Python packages installed: ``matplotlib``, ``numpy``

To run the simulation, go to the root directory of this project and execute:
```
python visualize.py
```

Example plots:
<img src="/images/visualizations.png" height="400"/>
