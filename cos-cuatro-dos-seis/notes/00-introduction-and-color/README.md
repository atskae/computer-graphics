# Introduction and Color

## What is Computer Graphics?
* [Imaging](https://en.wikipedia.org/wiki/Imaging): representing objects in 2D (images)
* [Modeling](https://en.wikipedia.org/wiki/3D_modeling): representing objects in 3D (models)
* [Rendering](https://en.wikipedia.org/wiki/3D_rendering): converting 3D models to 2D images
* [Animation](https://en.wikipedia.org/wiki/Computer_animation): using a series of 2D images to create the illusion of movement

## Imaging

A [**(digital) image**](https://en.wikipedia.org/wiki/Digital_image) is an image made of *pixels* (picture elements).

A [**pixel**](https://en.wikipedia.org/wiki/Pixel) is the smallest controllable element of an image.
* Each pixel is a sample of the original image/object
  * More samples = more accurate representation
  * Each pixel has varying [**intensity**](https://en.wikipedia.org/wiki/Intensity_(physics)) values
* Colors are represented by component intensities
  * Ex. red, green, blue (RGB)
  * Ex. cyan, magenta, yellow, black (CMYK)

We can apply a *function* to the 2D digital image to obtain a sample of the image
* Ex. We can apply `f(x,y)` to obtain the intensity value at position `(x, y)` in the image.

Various functions can be applied to images.

### Photographic Images

#### [Plenoptic Function](https://homepages.inf.ed.ac.uk/rbf/CVonline/LOCAL_COPIES/ASHBROOK1/node4.html)

In a photographic image, each pixel holds the amount of *radiance* (the amount/intensity of light) that arrives at the camera sensor from a certain position/direction/etc.

7D Plenoptic function:
```
radiance = L(x, y, z, θ, ϕ, t, λ) 
```

gives us the radiance that arrives to the sensor:
* From (camera) position `(x, y, z)`
* From direction/angle `(θ, ϕ)`
* At time `t`
* At wavelength `λ` (or frequency)
    * The [wavelength determines the color you see](https://en.wikipedia.org/wiki/Light)

A photographic image is a 2D array of samples of the 7D Plenoptic function.

Research paper: [*The Plenoptic Function and the Elements of Early Vision (Andelson, 1991)*](http://persci.mit.edu/pub_pdfs/elements91.pdf)

* [Charged Coupled Device (CCD)](https://en.wikipedia.org/wiki/Charge-coupled_device)
  * CCD image sensors convert incoming photons to electron charges, then the CCD reads outs these charges


### Electromagnetic Spectrum, Frequency/Wavelengths

[**Electromagnetic spectrum**](https://en.wikipedia.org/wiki/Electromagnetic_spectrum): a range of electromagnetic radiation (waves of the electromagnetic field) by their wavelengths/frequency

The frequency of light `v` is the speed of light `c` over the wave's wavelength `λ`:
```
v = c/λ
```

**Light** is a subset of waves in the electromagnetic spectrum that is visible to the human eye.
* Humans can only see waves with wavelengths `λ` between 700nm (red) to 400nm (violet)

### Color

The amount of energy at each visible frequency, the **spectrum**/**spectral power distribution(SPD)**, determines the *color* of the light
* Ex. white light = all frequencies of light (red to violet) emitted
* Ex. red light = only waves of frequency 4.3e14 Hz (700nm) is emitted

On a computer, we have to represent color with a finite amount of data, even though color is characterized by an infinite number of frequencies.

### Framebuffer

A [**framebuffer**](https://en.wikipedia.org/wiki/Framebuffer) is a portion of RAM on a computer that holds the pixels of the current screen of the computer display

Different frame buffer display technologies to display an image from the framebuffer:
* Soft-copy devices (creates an image in non-tangible form)
  * Video display devices: LCD (liquid crystal display), LED (light-emitting diodes), [ELD (electroluminescent display)](https://en.wikipedia.org/wiki/Electroluminescent_display)
* Hard-copy devices (creates an image in tangible form)
  * Printers, film recorders, pen plotters

More on [Hard-copy vs soft-copy devices](http://computergraphics-notes.blogspot.com/2013/07/hardcopy-devices.html)

### Human Color Perception

Why RGB to represent colors on a computer?

[Tristimulus Theory - Tricking the eye to see color (video)](https://www.youtube.com/watch?v=uIhTswEsymY&ab_channel=0612TVw%2FNERDfirst)

In our eyes we have rod cells and cone cells.
* Rod cells sense brightness of light
* Cone cells sense color
  * We have three types of cone cells to capture different ranges of the visible light spectrum:
    * Short wavelength cones -> ~blue = "blue cone"
    * Medium wavelength cones -> ~green = "green cone"
    * Long wavelength cones -> ~red = "red cone"

Each cone is stimulated by varying amounts for a certain range of wavelengths of light.
* Ex. The blue cone is stimulated if the light's wavelength is within the range (~400nm to ~550nm), and then detects the amount of blue color

Because we have only three cones (RGB), there are spectral power distributions (SPDs) that our eyes cannot tell apart: [**metamerism**](https://en.wikipedia.org/wiki/Metamerism_(color))
* Ex. different SPDs will look like white light to us
  * White light from a lamp vs white light from the sun

Screens and displays are built around the limitations of human eyesight; it only displays the SPDs that we can see
* There is no need to display colors across the infinite frequencies of visible light.
* Multiple SPDs would appear the same to the eye
* Displays use RGB since those are the most sensitive colors to our eyes (we have cone cells that captures these colors)

**Tristimulus Theory** ("tri-stimulus"): the "illusion" of being able to produce any color with red, green, and blue.

### Color Models

Various color models, each useful for different purposes
* RGB, CMY, HSV, XYZ, `La*b*`, etc.
* [Awesome Scratchapixel article on colors (additive and subtractive models)](https://www.scratchapixel.com/lessons/digital-imaging/colors) 


#### RGB Color Model, RGB Color Cube
* Used for displays that emit light directly to our eyes
  * Computer screens, smartphones 

Colors are additive
* We combine the primary colors (red, green, blue) to create the secondary colors (cyan, magenta, yellow)

#### CMY Color Model
* Used for surfaces that absorb (subtracts) colors from the incoming light
  * The resulting light that is reflected off the surface is the color that we see

* Colors are subtractive, good for printing, painting.

#### HSV Color Model
Hue, saturation, and value

Uses the color spectrum / spectral power distribution to obtain the HSV values:
* Hue (H) - the dominant frequency of light (highest peak)
  * The color/tint
  * Expressed as a number between 0 and 360 degrees
    * Red: 0-60 degrees
    * Yellow: 51-120 degrees
    * etc.
* Saturation (S) - Excitation purity (ratio of the highest frequency of light to the rest of the frequencies)
  * Amount of gray
  * Expressed as a percentage
    * 0% = more gray
    * 100% = primary color
* Value (V) - luminence (area under the curve)
  * Brightness/intensity of the color
  * Expressed as a percent
  * 0% = black
  * 100% = reveals the most color

HSV and HSB ("brightness" instead of "value") are the same color model

Useful for user interfaces (ex. tools that designers use, the color picker in Clip Studio or Photoshop) because these are more intuitive to the user (better represents how people relate colors)

TIL after all these years:

![HSV color picker in Clip Studio Paint](images/clip-studio-paint_hsv-color-picker.png)

* [What is HSV in design](https://www.lifewire.com/what-is-hsv-in-design-1078068)

### XYZ Color Model (CIE)

CIE (*Commission internationale de l’éclairage*) is an organization that creates international standards related to light and color
* **Color matching** tries to numerically specify a certain color and be able to reproduce that color
* [A Beginner's Guide to (CIE) Colorimetry](https://medium.com/hipster-color-science/a-beginners-guide-to-colorimetry-401f1830b65a)

* CIE XYZ expresses the entire gamut of human color perception

* [CIE 1931 color space](https://en.wikipedia.org/wiki/CIE_1931_color_space)

The [**gamut**](https://en.wikipedia.org/wiki/Gamut) is a certain *complete subset of colors*

* ["RGB to XYZ: The Science and History of Color" by John Austin (video)](https://www.youtube.com/watch?v=AS1OHMW873s&ab_channel=StrangeLoopConference)

* John Guild (UK), additive primaries model
* Irwin Priest (US), HSV model
* Came up with CIE-RGB, negative colors
  * Not ergonomic mathematically, not linear
* Used imaginary numbers as their primary colors to shift the CIE-RGB color space to only positive numbers
* The three axes in the new color space became XYZ
  * Y = brightness
* Possible to apply linear transformations to CIE XYZ

2019, sRGB becomes the standard devices
* sRGB is a subset of CIE XYZ
  * Can only display a subset of colors

XYZ and RGB are different transformations of the same data
* Can convert between each other
* Anything you can do with one you can do with the other

Companies can create their own color models (each with different primaries)
* Adobe RGB
* Microsoft sRGB

that are a subset triangle of XYZ and can then map to XYZ

* [More on CIE XYZ (Scratchapixel)](https://www.scratchapixel.com/lessons/digital-imaging/colors/color-space)

TIL
* [Munsell Color Science Laboratory (RIT)](https://www.rit.edu/science/munsell-color-lab)
  * You can get a PhD in color science 😲

### [CIELAB (L*a*b) Color Space (1976)](https://en.wikipedia.org/wiki/CIELAB_color_space)
`L*` for perceptual lightness (0=black, 100=white)
`a*` and `b*` for the four unique colors of human vision: red, green, blue, and yellow.

Based on the [*opponent color model*](https://en.wikipedia.org/wiki/Opponent_process).
* `a*`: green-red opponent colors (negative values toward green, positive values toward red)
* `b*`: blue-yellow opponent colors (negative value toward blue, positive values toward yellow)

Like CIE XYZ, CIELAB is
  * Device-independent
  * CIE standard observer
    * Averagine the results of color matching experiments
  * 3 dimensional, covers the entire space of human color perception

TIL There is a standard illuminant for daylight: [D65](https://en.wikipedia.org/wiki/Illuminant_D65)

## Readings
OpenGL Chapters 1, 2, and 19
