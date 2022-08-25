# CGOGL Chapter 1: Computer Graphics Hardware

## Video Display Devices
Primary output device in a graphics system is a video monitor

* Cathode-ray tubes (CRT)
* Flat-pannel displays
  * Reduced power consumption and thinner designs than CRTs

### CRTs
* A [**phosphor**](https://en.wikipedia.org/wiki/Phosphor) is a substance that emits light when exposed to some radiant energy
* Shoot a beam of electrons at a phospher coating, exciting the phosphor electrons which emit light.
* Need to regenerate the image after the light fades (electrons go from excited state to ground state); refresh rate

#### Raster-Scan Displays
* Electrons sweep one row at a time, each row called a **scan line**
  * (So that's why RTioW calls it a *scanline*...)
* **Refresh buffer**/**frame buffer**/**color buffer** stores the frame data (image to be displayed on screen)
  * Data on the intensity of the electron beams and color

**Aspect ratio**: number of pixel columns divided by the number of scanlines (or the reciprocal) on the display
* Many other ways to describe aspect ratio (ex. rectangle: width/height)

Black and white images only need 1 bit to store color information (set electron bean "off" or "on")
* More bits are required to display additional colors
* **Depth** of the buffer area = **bit plane** = # bits per pixel
  * High quality systems typically use 24 bits per pixel (3 bytes, RGB?)
* A frame buffer that uses 1 bit per pixel = **bitmap**
  * Multiple bits per pixel = **pixmap**

Refresh rate
* Humans could notice gaps between frames (flickering effect) if refresh rate < 24 frames per second
* Units: hertz (Hz) = units of cycles per second, with 1 cycle = 1 frame
  * 60 frames per second = 60 Hz
* On some raster-scan systems, interlacing scanlines (scan even lines first, then odd lines) to compensate for lower refresh rates while still being able to see the image as a whole

#### Random-Scan Displays
* Display an image on a subset of the display
  * Can draw lines in any specified order (no need to go one row at a time, top to bottom)
* Also called **vector displays** as they can display line drawings

### Color CRT Monitors
* **Beam penetration**
  * Have layers of phosophers, one layer per color
    * Ex. A layer for red, a layer for green
  * The color emitted is how deep the electron beam penetrates the layer
    * Faster electrons go further into the layer
      * Ex. Slow electrons cause red to be emitted (only excites the first layer), fast electrnos cause green to be emitted (penetrates red layer and excites the green layer)
      * Intermediate speeds can show combinations of colors (ex. yellow, orange)
* **Shadow mask** method
  * Have three electron beams (RGB)
  * Shadow mask can control which colors are emitted
  * Can control the intensity of each electron beam to produce different colors

### Flat-Panel Displays
* Reduced weight, volume, and power requirements than CRTs
  * Flat displays, can hang on a wall, use in mobile devices

Two categories of flat-panel displays
* Emissive/emitters: converts electrical energy to light
  * Ex. LEDs (light-emitting diodes), plasma panels, thin-film electroluminescent displays
* Non-emissive/nonemitters: use another light source (ex. sunlight, or built-in backlight) to create graphics patterns
  * Ex. Liquid crystal device (LCD)
    * Ex. Those solar-powered Texas Instruments calculators
    * Ex. [Tamagotchi screens!!!!](http://tama.loociano.com)
      * Note to self: I should write a Tamagotchi HW emulator

### Three-dimensional Viewing Devices
* Vibrating mirror to change focal length...  🤷‍♀️
* Hologram???

### Stereoscopic and Virtual-Reality Systems
* Stereoscopy: gives the effect of 3D
  * Need two views of the scene (left and right eyes' directions)
    * We see depth when these two images merge
  * Used in virtual reality systems
