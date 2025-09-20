# Notes

REQUIRED FILES FOR ANEMOMETER
Download the following parts from the original project by [wt29](https://github.com/wt29):
- [AnemometerBaseCap.stl](https://github.com/wt29/Anemometer/blob/main/stl/AnemometerBaseCap.stl)
- [AnemometerRotor.stl](https://github.com/wt29/Anemometer/blob/main/stl/AnemometerRotor.stl)
- [AnemometerRotorCap.stl](https://github.com/wt29/Anemometer/blob/main/stl/AnemometerRotorCap.stl)
- [windCup.stl](https://github.com/wt29/Anemometer/blob/main/stl/windCup.stl)

These parts did need some minor modification by filing, drilling, and sanding, but they did work

The EchoWeather_AnemometerEnclosure was used just as an adapter to make WT29's parts with this projects support rod.

## Construction
1. Print 3 total wind cups
2. Some drilling is required to allow the 3144 hall-effect sensor to fit flush with surface of print
3. Glue wind cups into rotor, ensuring each cup is pushed in all the way and level.
4. Cut 4mm rod to length to be used as axle.
5. Glue two 5x2mm magnets into bottom divots of rotor. MAKE SURE THE SOUTH POLE OF MAGNET IS FACING OUTWARD, the 3144 can only detect south magnetic poles, I used a test set up with an arduino and the 3144 sensor to know which pole was south.
6. Glue 3144 sensor into rotor base cap and solder wiring to leads, BEFORE gluing the base cap to the anemometer enclosure.
7. Insert ball bearings into rotor (top and bottom).
8. Insert 4mm rod through rotor assembly and then into the base assembly. (I put a small washer in-between the two assesmblies to reduce friction)

## Components
- (2) 5x2mm magnets
- (2) 8x4x3 ball bearings
- 4mm stainless steel rod, cut to length
- 3144 hall-effect sensor
- Support rod assembly (see supoort rod notes)
