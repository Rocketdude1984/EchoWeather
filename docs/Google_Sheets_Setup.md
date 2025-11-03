# Google Sheet Setup

This project use google sheets as the data collection software for the weather station, that way there is no need to open a port to the web using NGROK or a similar provider.
This solution to me seems the most veristile as it allows multiple stations to send data to the same spread sheet for collection making it easier to possiblly train an AI model later.


## 1. Setting up Your Google Sheet and getting Credentials
Random Nerd Turtorials has a great and beginner friendly article on how to setup a Google sheet for the project and get the required credentials to input into the creds.h file of the code.

## 2. Creating an app script for your sheet
Once you have a sheet created and input all of the required credentials into the cred.h file, you have to set up an app script on your sheet that send data to the app when it is requested.
Copy the code from the GetData file in the docs folder and input the code into a new app script.
Deploy this script (it will require some verfication from you to do so, but this will only be allowing you with the URL to access the weather data). 
Once the script is deployed makes sure to copy the web app URL. This is the URL you will input into the Internet Module of the app in MIT App Inventor (see the App folder for more information). 

This script fetches the last 144 rows of data (or if less than 144 all the rows) and sends JSON data to the app.

<img src="/docs/images/SheetSetup.png" alt="Diagram" width="900"/>

fig. 1 - the sheet setup with headers used, this is how the microcontroller will send the weather data. You can of course change the order of columns but this will have to be done in the arduino code (EchoWeather_v1.ino).
