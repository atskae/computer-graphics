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
A **raster system** stores an image as pixels
* Devices use a frame buffer to store pixel data

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

## Raster-Scan Systems
* Uses a special-purpose processor called the **video controller**/**display controller** that controls the operations on the display device
  * Frame buffer can be anyone on the system and is accessed by the video controller
* Raster-Scan systems also use other processors in addition to the video controller
  * CPU, accelerators

### Video Controller
* The frame buffer is typically a subset of the system memory
  * Frame buffer contents accessed with Cartesian coordinates which correspond to screen positions

### Raster-Scan Display Processor
* Have a designated **display processor**/**graphics controller**/**display co-processor**
  * Frees the CPU from graphics chores 🧹
  * Also could have own memory
  * (GPU?)
* **Scan conversion**: image to frame buffer format
  * Ex. Convert lines and shapes to pixel values to store in the frame buffer
* Generate line styles (dotted, dashed), display color areas, apply transformations to objects
* Interact with other interfaces (ex. mouse, drawing tablet!)
* [**Run-lenght encoding**](https://en.wikipedia.org/wiki/Run-length_encoding), store repeated data as its value and count.
  * Ex. pixel color and count
  * Save storage space
* **Cell encoding**: store rectangular areas...

## Graphics Workstations and Viewing Systems
Basically a bunch of examples of where screens are used!

## Input Devices
Keyboards, mice, data gloves, joysticks...

The best one out of them all:

### Digitizers!!
**Digitizer** inputs 2D/3D coordinates
* Ex. engineering/architectural applications, the digitizer scans the drawing/object and converts them to discrete coordinates
  * The points are joined with straight-line segments to approximate a curve or surface shape
* One type of digitizer: a **graphics tablet** !!!! 🥳💃🌟 Wahoooooo
  * Constructed with rectangular grid of wires embedded in the tablet surface
  * Electromagnetic waves interact with the electric signal in the pen/stylus to record a tablet position

[How does a drawing tablet work](https://essentialpicks.com/how-does-drawing-tablet-work/) 😍🤯
* No batteries in the stylus
  * The pen gets its energy wirelessly from the tablet via **electromagnetic resonance (EMR)**
* The stylus contains coils that converts electromagnetic waves to electrical energy
  * The electrical energy is used to measure pen pressure and buttton clicks
  * The measured data is sent back to the tablet with electromagnetic waves
* Oscillator, pulse signal, ... this sounds awfully familiar...
* When the coil in the stylus experiences a pulse of electromagnetic wave, an electric current is induced into the stylus
* Locating and tracking stylus position
  * A voltage increases occurs when the pen tip touches the tablet surface
  * The tip of the stylus has the highest voltage increase
* Pressure data
  * Is stored in the form of *phase difference*
    * Higher pen pressure results in a higher phase difference between the transmitted EM wave (from the tablet surface) to the received EM wave (from the stylus)
* Pen tablet drivers
  * Gives artist advanced settings (pen pressure sensitivity, button shortcuts, etc)
  * Communicates with the OS
    * The OS is not designed to work with pen pressure and pen tilt, so the driver helps the OS understand this data
* Display tablet
  * The resonant frequency of the tablet restricts which styluses could interact with the tablet
  * This sounds awfully familiar too...

Aug 26, 2022... What is this feeling?

* Acoustic/sonic tablets
  * Use soundwaves to determine positions (waaas (Deutsch))

### Hard-copy devices
* *Impact printers* directly contact the paper
  * Ex. dot-matrix printers
* *Non-impact printers* do not contact the paper
  * Instead use laser techniques, electrostatic and eletrothermal methods
  * Ex. Ink-jet: electrically-charged ink stream is deflected by the electric field
  * Ex. *Electrostatic devices* places a negative charge on paper
    * The toner is positively charged; toner becomes attracted to the negatively-charged areas on the paper
  * Ex. *electrothermal* heat is applied to generate images on heat-sensitive paper

## Graphics Network
* Graphics applications in multi-user environments across a computer network
* **Graphics server** a monitor on the network

## Graphics on the Internet
* Communicate with *transmission control protocol*/*internet protocol* (TCP/IP)
* *Uniform/universal resource locator* (URL)
  * Two parts, the protocol (http/https/ftp) for transferring the document, and the sever `www.siggraph.org`
    * `ftp://`
