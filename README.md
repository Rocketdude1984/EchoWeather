# EchoWeather: DIY weather station and app
A personal project to design, build, and program a working weather station that connects to a database and smart phone app. Collected data will then be used to train an AI model that will predict future weather variables.

<img src="docs/images/EchoWeatherCompleteBottom.jpg" alt="Diagram" width="600"/>

## Weather Variables

* Wind Speed
* Wind Direction
* Temperature
* Humidity
* Pressure
* Ambient Light
* UV Light
* Rain

## Sensors and Components
1. XIAO ESP32C3: https://www.amazon.com/dp/B0DGX3LSC7?ref=ppx_yo2ov_dt_b_fed_asin_title&th=1
2. BME280 breakout module: https://www.amazon.com/dp/B0DHPCFJD6?ref=ppx_yo2ov_dt_b_fed_asin_title&th=1
3. AS5600 Magnetic Encoder: https://www.amazon.com/dp/B094F8H591?ref=ppx_yo2ov_dt_b_fed_asin_title&th=1
4. Solar Power Manager Board: https://www.amazon.com/dp/B0FQP915R9?ref_=ppx_hzsearch_conn_dt_b_fed_asin_title_2
5. LTR390-UV Ultraviolet Sensor: https://www.amazon.com/dp/B0CHJL143Z?ref=ppx_yo2ov_dt_b_fed_asin_title&th=1
6. VEML7700 Ambient Light Sensor: https://www.amazon.com/dp/B09KGYF83T?ref=ppx_yo2ov_dt_b_fed_asin_title
7. 6V3W Waterproof Solar Panel: https://www.amazon.com/dp/B0CS364JGG?ref=ppx_yo2ov_dt_b_fed_asin_title&th=1

## Modules
The weather station will be built in 4 modules.
1. Power and basic variables (Main module)
2. Wind vane
3. Anemometer
4. Rain gauge

Each module will be connected together to the main module where all collected data will be sent to the cloud.

## Power
The station will be powered by two 18650s in parallel. To keep the battery charged, the station will have a built in 6V3W solar panel that charges the battery through a solar power management board.

## Materials
Most of the station is printed with PETG which should be able to withstand fairly high outdoor temperatures and resist fading or cracking. 1" PVC pipe is used for the main support to hold each module of the station. For extra UV protection a few coats of white spray paint will be used.

## Code
Download full code folder and make sure to replace the credentials in cred.h with your own information for WiFi and google sheet connectivity. Read this article for getting IDs and keys for the credentials: [https://randomnerdtutorials.com/esp32-datalogging-google-sheets/].

## App
The app was designed in MIT App Inventor and I have included the .aia file that you can import into App Inventor and configure to your project. (the only required configuration is the URL to your app script for sheets, re: docs/Google_sheet_setup).

## Calibration 
After allowing the system to run for a few months I compared known weather data from reliable sources with the data gathered from the station. Most values were close to the true ones, but I did calculate some scaling factors for the station. See the Calibration and Changes page under docs.

## One-year Thoughts
The Station has been running near constant for a year now (a few hiccups caused me to miss a few days). I did some things right and lot wrong that I would do different if I ever made another station.

### What I did right
- The Wind direction sensor worked perfectly.
- The Rain sensor did work, but how on that in the next section.
- The batteries only lost power 2 times (once because of an extended cloudy period > 7 days of cloulds, and once because of the batteries overheating).
- The app worked great for the whole test period
- The PETG and paint held up great even in the extreme heat.
- The enclosure was completely waterproof.

### What I would do differently
- lithium-polymer was the wrong choice for the Texas heat, I had 2 sets of lipos overheat and become unusable because of the Texas heat and the lack of active or passive cooling. I would instead switch to LiFePo4 cells with a thermal sensor on the cells that would activate a small fan to vent out the hot air in the enclosure.
- The ESP32C3 Xiao board just didn't have enough GPIO pins, I had to use a boot pin for the rain sensor that could sometimes cause the station to not turn on if it was stuck high. I would use a standard ESP32 dev board.
- I would make sure to have a quick-disconnect function for the staion and the mount. I had to take the station down quite a few times and unscrewing the mount and holding the station was very difficult to do on a ladder. I would make sure to use a connection that only needed a single thumb screw to remove the station.
- I would also make a custom holder for the staion when I am working on it because it I did not like the way the anemometer and wind vane would bend on when the station sat on the desk.
- The BME sensor did not prove to be super accurate either. The temperature was always 10-15 degrees higher than the true temperature in temps over 90 degrees, but yet correct under about 80 degrees. The humidity sensor just has been 100% for about 3 months and not changed, but it never seemed that accurate before.
- The lux sensor also didn't really correlate to any real values, but it did work for relative measurements.
- The solar board I installed half-way through the test was much better than the first board I had, but I would need to find one that works with the lower voltage of the LiFePo4 cells I would use.
- I would also be great for the station to start a new spreadsheet for logging data every year because the app took a while to load a years worth of data from the spreadsheet.
