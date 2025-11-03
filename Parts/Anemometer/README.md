# Notes

REQUIRED FILES FOR ANEMOMETER
Download the following parts from the anemometer project by [wt29](https://github.com/wt29):
- [AnemometerBaseCap.stl](https://github.com/wt29/Anemometer/blob/main/stl/AnemometerBaseCap.stl)
- [AnemometerRotor.stl](https://github.com/wt29/Anemometer/blob/main/stl/AnemometerRotor.stl)
- [AnemometerRotorCap.stl](https://github.com/wt29/Anemometer/blob/main/stl/AnemometerRotorCap.stl)

These parts did need some minor modification by filing, drilling, and sanding, but they did work.

The EchoWeather_AnemometerEnclosure was used just as an adapter to make wt29's parts with this project's support rod.
New wind cups were also designed (EchoWeather_WindCupv3). These redesigned cups were more sensitive to lighter winds.

## Construction
1. Print 3 total wind cups
2. Some drilling is required to allow the 3144 hall-effect sensor to fit flush with surface of print (AnemometerBaseCap)
3. 3. Glue two 5x2mm magnets into bottom divots of rotor. MAKE SURE THE SOUTH POLE OF MAGNET IS FACING OUTWARD, the 3144 can only detect south magnetic poles, I used a test set up with an arduino and the 3144 sensor to know which pole was south.
4. Use a 4mm Threading die to cut threads into 4mm rod, about a 1/4".
5. Cut the rod to length to be used as axle.
6. Glue wind cups into rotor, ensuring each cup is pushed in all the way and level.
7. Glue 3144 sensor into rotor base cap and solder wiring to leads, BEFORE gluing the base cap to the anemometer enclosure.
8. Insert single ball bearing into rotor (bottom). printed part has slots for 2 bearings, but in testing 1 bearing provided less friction and more sensitivity.
9. Insert 4mm rod through rotor assembly and then into the base assembly. (I put a small washer in-between the two assesmblies to reduce friction)
10. Use lubricating oil to reduce friction between rotor and base at the washer.

## Components
- (2) 5x2mm magnets
- 8x4x3 ball bearing
- 4mm stainless steel rod, cut to length
- 3144 hall-effect sensor
- Support rod assembly (see supoort rod notes)
