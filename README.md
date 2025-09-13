# CAT.M  

CAT.M is a **Computer-Aided Thermal Management** solution designed for your room. It helps you conserve energy, dress appropriately for the weather, and view temperature graphs.  


## Features  
- Built with the Open Meteo API — no need for an API key  
- Alerts the user when the indoor temperature is higher than the outdoor temperature, prompting them to turn off the AC and open a window, reducing unnecessary electricity use in the summer  
- Built-in clock with NTP server synchronization for accurate time  
- Recommends clothing based on current and future weather  
- Charts to show how the weather will evolve throughout the day  


## List of Materials  
- [Adafruit QT Py ESP32-S2 WiFi Dev Board](https://www.adafruit.com/product/5325)  
- A cheap, no-name SSD1306 OLED screen  
- A button  
- A random BMP-280 sensor  


## Bringing it to Life  

This section explains how the idea of CAT.M came to be, along with the design and building process.  

// TODO: Add how it came to be  

After finishing the draft code (just querying the API and printing the info to the console for now), I was stuck waiting for my screen and sensor to arrive. So I started making the pixel art. Given the name, the mascot had to be a cat. After some thought, I decided a lucky cat would be perfect, since it would be easy to animate or dress up later on.  

![First cat rendition](https://github.com/user-attachments/assets/bd8e2e75-d42c-4518-9919-f0d471600d87)  
<sup>First-ever rendition of the cat.</sup>  

Next, I worked on choosing what information should be displayed and where. I planned to have multiple pages, so it wasn’t crucial to show everything on a single screen. In the end, I decided to display: the clock, inside temperature, outside temperature, sunrise, sunset, precipitation chances, next-hour temperature, and the timezone.  

![First draft of the home screen](https://github.com/user-attachments/assets/3afefc77-aafb-4699-bcd3-4852f5107726)  
<sup>First draft of the home screen</sup>  

The next screen was trickier due to the size limitations. The idea was to display a graph of daily outdoor temperatures, with the eventual goal of showing other graphs such as precipitation probability. The first hurdle was screen size. For example, the number “20” requires 7×5 pixels, which isn’t a problem until you realize the usable space is only 120 pixels wide. Some quick math showed that only 17 numbers could fit, but for the graph to remain accurate, data points needed even spacing.  

So I faced a decision: buy a bigger screen or accept the inaccuracies of a slightly misaligned graph. In the end, I chose to keep the inaccuracies, since a bigger screen would require a complete redesign of the homepage.  

![Graph prototype 1](https://github.com/user-attachments/assets/93c2e7fe-e373-4d1c-9e4d-c57258b81ade)  

However, even with tight spacing, the numbers still didn’t fit. The only other options were splitting the screen or buying a bigger display. Having already invested quite a bit, I chose the split-screen approach. After some cramming, I barely managed to fit everything.  

![Graph prototype 2](https://github.com/user-attachments/assets/2cde13e5-dc68-414c-90e2-11d94c1039ef)

I considered baking the temperature scale (left side of the graph) into the background, but realized that in winter I’d need a range from about +35°C to -15°C, which would never fit. Instead, I’ll dynamically calculate the scale: take the highest temperature, add a few degrees for clarity, and go down from there. While there’s still design work left (animations, extra pages), I wanted a prototype working, so I moved to a simulator: [wokwi.com](https://wokwi.com).  

After a full day, I got the first pieces of the UI done. What remained was drawing the cat and displaying the text. Since the cat is too complex to draw with pixels and lines, I considered either generating code via Python or using a matrix of 1s and 0s to define the pixels.  

![UI work](https://github.com/user-attachments/assets/dd819010-a898-4a32-90a9-bd051c274a0f)  

I ended up using the matrix approach but realized I only needed an array. So I used [image2cpp](https://javl.github.io/image2cpp/) to convert the image into bits, which I displayed with `.drawBitmap()`.  

![Bitmap test](https://github.com/user-attachments/assets/6d54e658-2b92-4c08-849d-a6d703a836a6)  

Finally, my display arrived, so I could test everything (not quite everything, since I hadn’t written the sensor code yet). After wiring it up and compiling, everything worked as expected! One issue: the display has a weird two-color pattern that wasn’t mentioned in the product page. Luckily, it doesn’t affect functionality, so it stays for now.  

![First hardware test](https://github.com/user-attachments/assets/b0b013f9-f7b2-4f58-a494-3ce9023fb977)  

Next, I linked the actual API data with the text on screen. This took longer than expected due to issues with data passing between functions and tweaking UI element positions.  

Some problems I still need to solve:  
- Clock only updates in 15-minute increments (using API time directly)  
- No forecast beyond the next hour  
- No animations yet  
- Indoor sensor not integrated  
- No AC alert system  

That’s a lot, so I’ll probably focus on designing the enclosure tomorrow. For now, here’s the first “finished” prototype of CAT.M:  

![First finished prototype](https://github.com/user-attachments/assets/6e05bdc3-de8d-4f31-8c62-f6f9b5fd1719)  

I didn’t design the enclosure after all, since I got sucked into making the BME280 work. Now, with the sensor integrated, the homepage is functionally complete. Tomorrow I’ll sketch the enclosure and maybe add a button for switching pages.  

After reworking the code (separating each screen into its own function and creating a “chooser” function), I hooked up the button, and now I can switch screens. At first, I forgot to debounce the button, making it impossible to switch reliably. I added `delay()`, but quickly realized it wasn’t a good solution since it caused inconsistent behavior. I switched to a `millis()`-based debounce system (similar to the API refresh timer).  

Here are the two current screens (not impressive yet, but it’s coming together):  

![Two screens](https://github.com/user-attachments/assets/ab94f750-b49a-4168-9bc9-c0cb25862366)  

I added the cat waving animation, it isn't my best work but it works. It is on a random timer (well pseudo-random but for our purposes random). One thing I would like to maybe improve is not using delay() because it blocks the whole program for about 1 second, however I haven't yet thought of a way to do it. Next up is the second screen, which is going to be how to dress based on the weather. 


After getting back from a small vacation I finally had the energy to make the enclosure. At first I was just going to do a thick triangle, but it just didn't sit right with me. So after a few designs, I landed on this:
<img width="1920" height="1080" alt="image" src="https://github.com/user-attachments/assets/f205d488-7c1d-40d6-9924-637f7a646530" />

In the back, some vents for the heat to rise out from the board, plus a hole for the usb c cable to passthrough to the board. In front, a simple layout, just the screen and the button. One design choice I am really proud of is the mounting of the temperature sensor. I mounted it in the center of the enclosure, outside so the temperatures aren't skewed by the enclosure or the SOC's temps. After 3D printing it, it looks very good in person, and because of a trick I learned (printing a thin sheet of material with all the holes that will make it in the final product so you can test tolerances quickly), everything fit perfectly. Attached is a picture of the 3D printed product (keep in mind this isn't complete since I have yet to print the side panel):

<p align="center">
  <img width="500" height="563" alt="image" src="https://github.com/user-attachments/assets/204221f2-125c-4734-9590-1013b648e0c1" />
</p>

Finally added the hourly temps display, however I hate the way it looks. It's just a wall of text, which you actually need to put effort into understanding, which is not a good thing, however I have no idea how to fit all the temperatures on the display without it looking bad. One idea I have is redoing all the numbers in Photoshop to slim them down, although I don't think it will look that much better. Another idea I am considering is adding an interactive way to select the hour you want to display, but I would love to able to display the full 24 hours.
![IMG_20250826_002231_555](https://github.com/user-attachments/assets/5f93ff70-bad6-42d6-a639-ef46603534d1)

Decided on a middle of the road approach, displaying 12 hours in the future, max at 24. So for example if its 1 am, you'd see the weather all the way up to 12 am, and if it is 1 pm, you would see from 12 am to 11 pm. I would like to again use tomorrows weather forecast but that would require a bit of code reworking and for now I just want to get it to a semi finished state. Also added the cat since there was some empty space, the sprite turned out decent for my artistic skills, but if I have the time I might revisit both of the sprites(home page and hourly temps page). Here is how it looks right now:
![IMG_20250826_205658_148](https://github.com/user-attachments/assets/0af7472a-0577-40c8-b013-5324415ea296)

After painting the housing, I finally worked the courage to put everyhing in an hot glue it in place. The hole for the display was slightly too small, and so I got a file and started expanding the hole, trying to see if the display fit. At one point, by forcing it too much, I broke a bit of glass, which i thought was not that important. I decided to do a test to see if the display still worked after that, just so I dont have to take it out of the super glue. Little did I know, the display was not fine at all. It started smoking instantly and a bright orange light started coming out of one of the traces. So until I get a display from either ordering one or asking a friend, I cannot move forward with this project. ETA for coming back is about 1-2 days depending on which option I choose.

It took significantly longer than 1-2 days because I decided to order from Aliexpress. It was however worth it due to the fact that this time i could get a display that was just one color, not that weird yellow and blue combination. After testing the new display, I started assembling everything. The reason for this is that for the hardware functionality the project is done, and so there is no reason to keep a rat's nests of cables on my desk. Assembly went smooth, only inversing the SDA and SCL cables for my sensor, however I quickly discovered and fixed this error. Below you will find some photos of the final look (I also used the built in led to get a cool cyan backlight).

<img width="1920" height="1080" alt="Collage" src="https://github.com/user-attachments/assets/a909c581-86af-4818-a74d-a6a04a7434ef" />

Next up, finishing up the software, aka adding the AC functionality and the clothing one.
