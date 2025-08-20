CAT.M is a Computer Aided Thermal Managment solution made for your room that helps you conserve energy, dress for the weather and look at temperature graphs.

## **Features**
- Built with the Open Meteo API, meaning no need for an API key
- Alerts the user when the temperature inside is hotter than the temperature outside, prompting the user to turn off the AC and open a window, reducing the unneccesarry use of electricity in the summer
- Built in clock with a connection to a NTP server for accurate time
- Recommend to the user what to wear based on the current and future weather
- Charts to show how will the weather evolve over the day


## **List of materials**
- The Adafruit QT Py ESP32-S2 WiFi Dev Board (https://www.adafruit.com/product/5325)
- A cheap no name SSD1306 OLED screen
- A button
- A random BMP-280 sensor


## **Bringing it to life**

This is the section where I talk about how the idea of CAT.M came to be, how I did the design and the building process.

//add how it came to be

After finishing the draft code (just querying the api and printing the info out to the console for now), I am stuck waiting for my screen and sensor to arrive. So I start making the pixel art. Obviously, given the name, the mascot HAS to be a cat. After a bit of thinking I decide a lucky cat would be perfect for this, because it will easy to animate/dress later down the road.
<img width="1920" height="1080" alt="image" src="https://github.com/user-attachments/assets/bd8e2e75-d42c-4518-9919-f0d471600d87" />
<sup>First ever rendition of the cat.</sup>

Next up, choosing what information to be displayed and where. I do plan on having multiple pages, and so it isn't crucial to have everything on here. In the end, I decided to have: the clock, inside temperature, outside temperature, sunset time, sunrise time, chances of precipitation, the temperature next hour and the timezone.
<img width="1920" height="1080" alt="image" src="https://github.com/user-attachments/assets/3afefc77-aafb-4699-bcd3-4852f5107726" />
<sup>First draft of the home screen</sup>

Moving onto the next screen, which is the harder one to make due to the size limitations. The idea for this screen is to have a graph of the daily temperatures from outside, with the end goal to setup some kind of system to display other graphs, such as precipitation probabilty over the day. The first hurdle I encountered is the size of the screen. The minimum size of, for example, the number 20, is 7 pixels wide and 5 pixels high, which would not be a problem if not for the fact that the space I can work with is 120 pixel in width. Some quick math reveals that only 17 numbers with those dimension can be fitted on the screen. The issue is that in order for the graph to be accurate you need the numbers to be spaced out in the same way, so as not to move a data point closer to another and suggest an unrealistically steeper graph. The decision I am now facing is: do I buy a bigger screen or accept the inaccuracies of the missaligned graph. In the end I choose to keep the inaccuracies, because a new screen would entail a complete redesign of the home page, which would be tedious. 
<img width="1920" height="1080" alt="image" src="https://github.com/user-attachments/assets/93c2e7fe-e373-4d1c-9e4d-c57258b81ade" />
However, even with the abysmal spacing, the numbers still did not fit. The only other solutions that I can think of is either split the screen in half or eat my pride and buy a bigger screen and redo all my design work. Having already spent quite a bit on this project, I believe the best choice is the split screen route.
After a bit of cramming I was able however to get everything to fit, although with BARELY enough space.
<img width="1920" height="1080" alt="image" src="https://github.com/user-attachments/assets/2cde13e5-dc68-414c-90e2-11d94c1039ef" />

I contemplated whether or not to bake the left side of the graph (the temps) into the background, but I realised that if I want to use this in winter I would have to have a range of about 35 to -15 degrees, which would not fit at all. And so what I will probably do is, based on the input array, get the highest point, add a few degrees for clarity's sake, and go down from there. While there are some design stuff left to do (mainly animations and some other miscellaneous pages), I want to get a prototype working, and for that I'm going to use a simulator, more specifically [wokwi.com](wokwi.com).

After a full day, I have the first bits of the ui done. What remains is to draw the cat and display the text. Because the cat is a complicated figure to draw with pixels and lines, I'm probably going to either write a python code that outputs the exact code to recreate it, or use a matrix to show 1s and 0s based on if the pixels are light up, and using that to display the cat.
<img width="1916" height="999" alt="image" src="https://github.com/user-attachments/assets/dd819010-a898-4a32-90a9-bd051c274a0f" />

I decided to go with the matrix approach, however quickly realised that i only needed an array. And so I used the tool [image2cpp](https://javl.github.io/image2cpp/) to convert my image into bits, which I then displayed using the .drawBitmap function. 
<img width="1917" height="1000" alt="image" src="https://github.com/user-attachments/assets/6d54e658-2b92-4c08-849d-a6d703a836a6" />

My display is here and that means I can finally test everything(not quite everything since I have not yet read the code for the sensor integration). After connecting everything on the breadboard and compiling the sketch, everything was functioning as expected! One unfortunate thing is that the display I order has this weird 2 color pattern which was not specified on the product page, but it doesnt affect the functionality that much, so for now its here to stay.
![IMG-20250819-WA0001-cropped 2](https://github.com/user-attachments/assets/b0b013f9-f7b2-4f58-a494-3ce9023fb977)

The next step was to link the actual information from the api with the text displayed on the screen. This took slightly longer than expected due to multiple issues, mostly the data not being passed correctly through funtions, but also some minor adjustments to the position of the UI elements. Some current things that I want to fix are: the clock being only in increments of 15 due to me just using the api provided time, lack of viewing the forcasted weather in the future, the lack of animation, the sensor for the indoor temps not being setup and finally the system to alert the user to turn off their AC. This is quite a lot however, so I'm probably going to focus on designing the enclosure tomorrow. Here is a picture of the first "finished" prototype of CAT.M:
![IMG_20250820_010721_364](https://github.com/user-attachments/assets/6e05bdc3-de8d-4f31-8c62-f6f9b5fd1719)

I did not in fact design the enclosure as I got sucked into making the BME280 work, and so I just added the final piece of the puzzle as far as the homepage is concerned. Hopefully tomorrow I start doing some sketches for the enclosure, along with maybe putting a button to change the current page.



