# App Notes

The app for the EchoWeather system was design in Gimp and built in MIT App Inventor

I could not include the APK because if you want to make this project you will have to edit a URL in App Inventor in order to connect to your app script that fetches data from your Google Sheet (re: fig. 3).

## Displayed Values
* Temperature
* Humidity
* Pressure
* Average wind speed
* Gust wind speed
* Cardinal wind direction
* rain rate in in/hr
* light level
* UV level
* Battery Voltage
* Location of Weather Station
* Last updated time
* Icon of type of weather based on light, rain, and temperature
* 24 Hour high and low values for temperature, wind, rain, and humidity

<img src="/docs/images/AppHomeScreen.png" alt="Diagram" width="300"/>

fig. 1 - EchoWeather App homescreen

The App also has a settings screen for changing units on temperature, wind speed, and daylight saving or standard time, see fig. 2.

<img src="/docs/images/AppSettingScreen.png" alt="Diagram" width="300"/>

fig. 2 - The settings screen for the app to change select units and time

<img src="/docs/images/appURLconfig.png" alt="Diagram" width="800"/>

fig. 3 - The URL component you must change to match your own app script in App Inventor

See Google Sheets setup in the docs folder on how to get this URL for your own project.
