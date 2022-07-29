# Evolution simulator using a real-time neuro-genetic algorithm

## Rules:
(values mentioned below can be configured and experimented with)
1. An organism looses ``0.004`` energy each time-step starting at ``1.0``, if the energy reaches ``0.0`` the organism dies and it converts to meat based food.
2. If an organism runs into plant based or meat based food it gains specific amount of energy somewhere between ``[0.0; 0.5]`` depending on its ``diet`` gene.
3. If an organism runs into another and both are "horny" (which means its "horniness is ``1.0``) they produce a child ("horniness" starts at ``0.0`` and increases by ``0.005`` each time-step until it reaches ``1.0``).
4. If two organisms produce a child they both lose ``0.1`` "horniness" but gain ``0.005`` each iteration until it is at ``1.0`` again.
5. A new child inherits its brain and genes from the one parent which lives the longest.
6. There is a ``20%`` chance that a gene mutates by getting a value in range ``[-0.05; 0.05]`` (mapped to the actual gene value range) added to it.
7. There is a ``7%`` chance that weight value / synapse of the brain mutates by getting a value in range ``[-0.01; 0.01]`` added to it.
6. If an organism runs into a wall it looses ``0.75`` energy.

## Genes:
(all gene ranges can be configured and experimented with)
1. ``diet``: A value in range ``[0.0; 1.0]``. The closer it is to ``0.0`` the more energy the organism gains from plant based food. The closer to ``1.0`` the more energy it gains from meat.
2. ``size``: A value in range ``[12.0; 23.0]`` which represent the max adult size of an organism. The size also determines the speed of an organism by getting mappend to the speed range of ``[2.0, 4.0]``.
3. ``sightAngle``: A value in range ``[75.0; 160.0]`` which determines in which angle the organisms can detect food in.
3. ``sightReach``: A value in range ``[50.0; 100.0]`` which determines at which maximum distance the organisms can detect food.


## Senses:
An organism has only one sense: Sight.
If food enters the ``sightAngle`` and ``sightReach`` an organism can detect it by processing the distance from the end of both angle ends. 

TODO insert image here

## Brain:
The brain of an organism has the following fixed topology:
- input-nodes: ``3`` (distance from lower angle end, distance from higher angle end, reccurent value t-1)
- hidden-nodes: ``3``, tanh activation
- output-nodes: ``2``, sigmoid activation (turn angle, reccurent value t)

TODO insert image here
