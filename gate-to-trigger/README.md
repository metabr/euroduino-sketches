# Gate to trigger

Sends trigger from Digital Out 1 on gate rise edge on Digital In 1. While gate
1 is high, send trigger on rise edge on Digital In 2.

Used for looped EG with MakeNoise Function:

  * Gate -> Digital In 1
  * MakeNoise EOC -> Digital In 2
  * Digital Out 1 -> MakeNoise Trig In
