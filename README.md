# A simulation of evolution using Real-Time NEAT

# Rules:
1. An organism looses 0.002 energry starting at 1.0, if the energy reaches 0 the organism dies and it converts to meat-food.
2. If an organism runs into plant-food it gains 0.25 energy, if it runs into meat-food it gains 0.1 energy (engery has to upper limit).
3. If an organism runs into another and both are "horny" (which means its "horniness is 1.0) they produce a child ("horniness" starts at 0.0 and increases by 0.002 each time-step until it reaches 1.0).
4. If two organisms produce a child they both lose 0.1 "horniness".
5. If the "horniness" of an organism is below 1.0, it gains 0.005 each iteration.
6. If an organism runs into a wall it looses 0.25 energy.

# Senses:
An organism has only one sense: Sight.
It can see 40 pixels far in a 160 degree angle.

# Brain:
An organisms brain has two input neurons: One to tell if it sees food ({0;1}) and one to tell in what angle that food lies to the current direction ([0;1]).