# Calibration and Changes

After allowing the station to sit outside and face harsh sunny days, long periods of overcast, and fierce rain and wind, I was able to calibrate the sensors to more accurately measure weather data. I Also had to repair and make changes to some of the design.

## Calibrations

All scaling and offset values are already set in the EchoWeather_v1.ino file.

- UV scaling = 0.0285
- Pressure scaling = 1.003
- Light Level factor 1 = 1.8
- Light Level factor 2 = 7

The wind, humidity, and temperature sensors were all close enough to the true values to not need calibration. 
The Light sensor had a strange measuring problem were under a threshold the values were close to the true values, but after that threshold the values were far to small.
To fix this I included a small change in the getLuxLevel function to apply different scaling factors depenending on a threshold set by measurement (15000):

```
int GetLuxLevel(){
  int lux = veml.readLux();
  if (lux > 15000){
    lux = lux * lightScale2;
  } else {
    lux = lux * lightScale1;
  }
  delay(500);
  return lux;
}
```

## Changes

After some strong winds and storms the station did suffer small damage and actually had a software problem which caused the battery charging board to malfunction and killed the charger. The biggest issue through was not a weather problem but a connectivity issue.

### Weather damage
- I had placed small pieces of milk carton to act as diffusion screen for the UV and light Level sensors, these both fell off during heavy wind due to weak glue bonds. I used gorilla glue to fasten this time and so far they have held on.
- The fastener (bolts and nuts) had already started rusting so I replaced the rusty ones and spray painted new hardware with a rust resistant paint.
- A series of cloudy days (about 6 days of overcast) and a loss of internet connection caused the station to get stuck in a loop trying to connect to the Google Sheets API and this not only drained the batteries, but caused the TP4056 charging board to stop recharging.

### Improvements
Because the station would no longer charge, I had to remove it and diagnose the issue. I found that the TP4056 board was no longer functioning. 
I did not simply replace it with another of the same boards because this issue would probably happen again.
Instead this time I went with a more robost charging system. An actual solar charging board.

Solar Power Manager 5V, JEITA Li-ion Battery Charger Module with Dual USB & 5V/3.3V Output, for Solar Panel Charging
[Solar Charging Board - AmazonUS](https://www.amazon.com/dp/B0FQP915R9?ref_=ppx_hzsearch_conn_dt_b_fed_asin_title_2)

<img src="images/SolarCharging Board.jpg" alt="Diagram" width="500"/>

This board was far superior to the simple TP4056. It had much more protection (Thermal, overdischarge, under voltage, etc) it had a better built-in converter and the setup was much simpler. I wish I would have used it from the start.

So far this new charging board has worked perfectly for over 2 months and has never let the batteries dip below 3 volts even on several cloudy days (about 5 days straight of overcast at one time).

### Code Changes
After this testing I determined that because of using the 2, 18650s cells the station had enough capacity to run with 10 minute measuring intervals for wind and rain, instead of 5min measuring and 5min sleeping. This change improved the acccuracy of the wind speed, wind gust, and rain rates for the station.

This change also makes the controller reset for every measuring period to hopefully elimanate any chance of it becoming stuck in a connection loop like last time.
