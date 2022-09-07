# Evolution simulator using a real-time neuro-genetic algorithm

<img src="/images/simulation-recording.gif" height="350"/>

---

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

---

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

---

## 📜 Rules:
All variables mentioned below can be configured and experimented with. Check them out in the table at the end of the README (TODO).
1. An organism looses a specific amount of energy which starts at ``1.0``. The amount of energy loss each iteration gets calculated in the following way:
```
energyLoss = initialEnergyLoss + map(size, 0.0, maxEnergyLossBySize) + map(speed, 0.0, maxEnergyLossBySpeed) + energyLossIncrementionRate * currentLifeTime)
```
where ``initialEnergyLoss``, ``maxEnergyLossBySize`` and ``maxEnergyLossBySpeed`` are two simulation parameters.
If the energy reaches ``0.0`` the organism dies and it converts to meat based food.
2. If an organism runs into plant based or meat based food it gains specific amount of energy somewhere between ``0.0`` and ``maxEnergyGainByFood`` depending on its ``diet`` gene (``diet = 0.0`` means 100% herbivorous, ``diet = maxEnergyGainByFood`` means 100% carnivorous).
3. Food has a chance to rot and will eventually become poisonous. If an organism eats such rotten food it looses energy.
4. If an organism runs into another and both are breedable (which means their breedability is ``1.0``) they produce a child (when a new organism is born its breedability starts at ``0.0`` and increases by ``0.005`` each time-step until it reaches ``1.0``, indicating adulthood).
5. If two organisms produce a child their breedability is set to ``0.0`` but regenerates by ``breedabilityRegeneration`` each iteration until it is at ``1.0`` again.
6. A new child inherits its brain from the parent with the highest energy. It also inherits randomly genes from each parent.
7. When a brain and genes are inherited by a child there is a change of ``geneMutationProbability`` percent that a gene gets mutated by adding or subtracting ``geneMutationFactor``. Same for the brain synapses with the mutation chance of ``brainMutationProbability`` and mutation factor ``brainMutationFactor``.
8. If an organism runs into a wall it looses ``wallDamage`` amount of energy.
9. An organism can store ``maxFoodStorage`` amount of food in its stomach. If it eats when it's full it looses ``overFedEnergyLoss`` amount of energy. It takes ``digestionIterations`` iterations to digest the food and be able to eat again.

---

## 🧬 Genes:
All gene ranges can be configured and experimented with. Check them out in the table at the end of the README (TODO).
1. ``diet``: A value in range ``[0.0; 1.0]``. The closer it is to ``0.0`` the more energy the organism gains from plant based food. The closer to ``1.0`` the more energy it gains from meat. The range ``[0.0; 1.0]`` gets mapped to ``[0.0; maxEnergyGainByFood]`` to calculate how much energy is gained. 
2. ``size``: A value in range ``[universalMinAdultSize; universalMaxAdultSize]`` which represents the adult size of an organism.
3. ``speed``: A value in range ``[universalMinSpeed; universalMaxSpeed]`` which represents the speed of an organism.
4. ``sightReach``: A value in range ``[universalMinSightReach; universalMaxSightReach]`` which determines at which maximum distance the organisms can detect food (range of the two sensors).

---

## 🧠 Brain:
###### (the architecture might change if new inputs and/or outputs are added, but I'm too lazy to update the illustration each time)
The neural network of an organism has the following fixed topology:
- input-nodes: ``3`` (distance from left sensor, distance from right sensor, reccurent memory value t-1)
- hidden-nodes: ``hiddenSize``, tanh activation
- output-nodes: ``2``, sigmoid activation (turn angle, open-mouth)

The neural network processes the next food in its sight-reach and how full its stomach is. Therefore the inputs are the following:
The distance of food to the left sensor, the distance of food to the right sensor, the food type (plant: ``0.0``, meat: ``0.5``, rotten: ``1.0``) and the percentage of how full the organism is.

The output of the neural network is the turn angle and wether it opens its mouth or not. If the mouth is closed the organism doesn't eat food if it passes it. It opens it's mouth if the open-mouth neuron is bigger than the``openMouthThreshold`` threshold.

Senses diagram:
<img src="/images/organism.png" height="400"/>

Brain Diagram:
<br/>
<img src="/images/brain.png" height="400"/>

---

## 📊 Simulation data visualization:
When you start the simulation, you can simutainuously run a Python script to plot relevant data.
Make sure that you have the following Python packages installed: ``matplotlib``, ``numpy``

To run the simulation, go to the root directory of this project and execute:
```
python visualize.py
```

Example plots:
<br/>
<img src="/images/visualizations.png" height="400"/>
