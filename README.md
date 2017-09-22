BRG SongSaver
=============

ESP8266 sketch + web API/frontend to save the title and artist of the current song playing on Brony Radio Germany.


## Requirements ##
* Webserver
* ESP8266 (or any development board having an ESP8266)
  * e.g. Wemos D1 Mini
* 1 button with a couple of wires


## Preparing the web API/frontend
1. Rename `/web/config-default.php` to `/web/config.php` and open it in any editor.
    * Put in your desired protection key, which acts as your API key.
    * This key is needed to use `/web/np.php`.
2. Make sure `/web/np.php` is accessible using HTTP. **HTTPS is not supported yet.**


## Preparing the ESP8266 ##
1. Rename `/sketch/config-default.h` to `/sketch/config.h` and open it.
    * Put in your Wifi SSID and password
    * Put in the url to access `/web/np.php`. Note that this url has to contain the `key` parameter.
    
      ```
      http://yourdomain.de/path/web/np.php?key=PUT_YOUR_KEY_HERE
      ```
    * If you want, you can also change the pin used for the button.
2. Upload `/sketch/sketch.ino` to your ESP8266 using for example the Arduino IDE.
    * You need to have additional boards installed in the Arduino IDE, to be able to upload the sketch. Check out this Repo to find out how it works: https://github.com/esp8266/Arduino
    * Additionally you need to install the ESP8266WiFi library. You can simply do this by adding it through the library manager in the Arduino IDE.
3. Connect your button to your desired pin (by default its D7) and ground. There is no need for an pullup/-down resistor, we're just using an internal one.


## How it works ##
Push the button if you want to save the title and artist of the current song playing on Brony Radio Germany.

The ESP8266 then connects to your WiFi and sends a request to your webserver, which then will grab the information using the BRG API and will save the data to a JSON file.

After the Arduino sent the request, the WiFi module will go back to sleep.

To get a nice and tidy list of all your saved titles, open up your web browser and navigate to `/web/index.html`. This page requires JavaScript to work, the data will be loaded via AJAX every 5 seconds.


## Tips ##
#### Adding a HTTPS exception to your .htaccess file ####
As described above, accessing `/web/np.php` thorugh the ESP8266 using HTTPS is not supported yet.
If you're using a `.htaccess` file to enforce HTTPS, you can add a simple exception to it:
```
RewriteCond %{REQUEST_URI} !^/path/to/web/np.php
```
