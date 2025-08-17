CAT.M is a Computer Aided Thermal Managment solution made for your room that helps you conserve energy, dress for the weather and look at temperature graphs.

**FEATURES**
- Built with the Open Meteo API, meaning no need for an API key
- Alerts the user when the temperature inside is hotter than the temperature outside, prompting the user to turn off the AC and open a window, reducing the unneccesarry use of electricity in the summer
- Built in clock with a connection to a NTP server for accurate time
- Recommend to the user what to wear based on the current and future weather
- Charts to show how will the weather evolve over the next 24 hours


**LIST OF MATERIALS**
- The Adafruit QT Py ESP32-S2 WiFi Dev Board (https://www.adafruit.com/product/5325)
- A cheap no name SSD1306 OLED screen
- A button
- A random BMP-280 sensor


**BRINGING IT TO LIFE**

This is the section where I talk about how the idea of CAT.M came to be, how I did the design and the building process.

After finishing the draft code (just querying the api and printing the info out to the console for now), I am stuck waiting for my screen and sensor to arrive. So I start making the pixel art. Obviously, given the name, the mascot HAS to be a cat. After a bit of thinking I decide a lucky cat would be perfect for this, because it will easy to animate/dress later down the road.
<img width="1920" height="1080" alt="image" src="https://github.com/user-attachments/assets/bd8e2e75-d42c-4518-9919-f0d471600d87" />
<sup>First ever rendition of the cat.</sup>

Next up, choosing what information to be displayed and where. I do plan on having multiple pages, and so it isn't crucial to have everything on here. In the end, I decided to have: the clock, inside temperature, outside temperature, sunset time, sunrise time, chances of precipitation, the temperature next hour and the timezone.
<img width="1920" height="1080" alt="image" src="https://github.com/user-attachments/assets/3afefc77-aafb-4699-bcd3-4852f5107726" />
<sup>First draft of the home screen</sup>


