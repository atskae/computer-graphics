# Notes

Making sure I obtain the *mathematical intuition* behind each image processing filter instead of code-monkey copy-pasta-ing off the internetz.

## Luminance Operations

### Contrast

There are [various formulas for adjusting contrast](https://en.wikipedia.org/wiki/Contrast_(vision)#Formula) and various mathematical definitions of what *contast* is.
* [GIMP formula for contrast](https://en.wikipedia.org/wiki/Image_editing#Contrast_change_and_brightening)

**Luminance contrast** is defined as:
```
luminance_difference / average_luminance
```

Recall, **luminance** is how dark/bright an image is, specifically the value of each color channel: Red, Green, Blue. 

Recall that [**grayscale**](https://en.wikipedia.org/wiki/Grayscale) images only displays the light intensity of each pixel.
* So we can get the average luminance from a grayscale image (according to [this MatLab post](https://www.mathworks.com/matlabcentral/answers/109662-how-to-write-the-code-for-the-average-luminance-of-input-image#answer_118293))

[Another way to think of contrast (GIMP)](https://pippin.gimp.org/image-processing/chap_point.html): changes the range of possible luminance values.
* If you visualize a histogram of luminance values in an image, changing the range of luminance values mean compressing or expanding the histogram around the midpoint value.
* Here the midpoint is `0.5` which corresponds to 50% gray
  * When we add/subtract the midpoint value `0.5`, we are centering the compression/expansion of the histogram around the midpoint
```
new_value = (old_value - 0.5) × contrast + 0.5
```

* When `contrast > 1`, the brighter pixels become brighter and the darker pixels become darker, which expands the range of the luminance histogram
  * The steepness of the function that computes the `new_value` influences how drastic the brightness and darkness becomes
* When `contrast < 1`, the range of possible luminance shrinks

We can visually see the contrast vs. range (possible y values) changes (graphed on [Desmos!](https://www.desmos.com/calculator)):

![Graph key](images/graph-color-key.png)
![Contrast vs. range](images/contrast-histogram-ranges.png)

Back to this funky formula:

![Contrast with tangent](images/contrast-tan.png)

In essense, it's the same as `new_value = (old_value - 0.5) × contrast + 0.5`, but we just use an extra fancy function (`tan()`) to mess with the `contrast` value before we compute the new value. 

The luminance histogram ranges still increases linearly:

![Tangent graph key](images/tan-graph-color-key.png)
![Tangent graph histogram values](images/contrast-tan-histogram-ranges.png)

Now it makes sense that:
* `contrast=0` (black graph) returns the original image
* `contrast=-1` (solid red graph) gives a completely gray image (all possible pixel values map to `0.5` which is middle gray)
* Larger the contrast value, the steeper the function gets, which gives us a larger range of possible values

![Contrast full range](images/contast-tan-histogram-full-range.png)

* The function is undefined when `contrast` reaches `1`
  * This is because `tan()` is undefined at `pi/2`
* We only care about `x: [0, 1]` since that's the range of each pixel value
  * The different `contrast` values yield a different *function*

Makes sense!!! ✨🌈💃

Why `tan()`?
* It maps more nicely to the [-1, 1] range
  * Ex) `new_value = (old_value - 0.5) × contrast + 0.5` can take really large positive or negative numbers forever
* There's probably a better explanation...


### Gamma Correction

I saw gamma correction briefly in the [ray tracer!](https://github.com/atskae/computer-graphics/blob/master/ray-tracing-in-one-weekend/README.md#83-gamma-correction)

[A nice post on gamma correction](https://www.cambridgeincolour.com/tutorials/gamma-correction.htm)
* This is in the context of *cameras*, not a commputer screen...

![Human vision vs camera](images/human-vision-vs-camera.png)

**Why do we need gamma correction?** Our eyes do not experience brightness the same way cameras do.
* Cameras sense how many photons hit its sensors
  * So twice the # of photons -> twice the resulting brightness
    * To a camera, brightness is linear
    * [Gamma correction (video)](https://www.youtube.com/watch?v=wFx0d9c8WMs&ab_channel=VideoTechExplained)
* Humans are more sensitive to luminance changes in darker tones than in lighter tones
* Humans can percieve darker regions much brighter than they truly are ([Scratchapixel](https://www.scratchapixel.com/lessons/digital-imaging/digital-images)) 🤯
* *Lightness* is how humans percieve the actual luminence
```
lightness ~= luminance ^ (1/3)
```

[Displaying images to the screen (Scratchapixel)](https://www.scratchapixel.com/lessons/digital-imaging/digital-images/display-image-to-screen)
* Now our screen's light intensity is linear (unlike CRTs which has a non-linear function of input voltage vs luminance)
* But we still apply gamma correction because it allows us to have larger varations of luminance for darker shades than lighter shades: **gamma encoding**
  * Apply a gamma `1/γ` to the intensity value, convert it to a byte `[0. 255]` and store this value in the image file
    * Actual luminance goes from `0.1` -> `0.2`, the percieved lightness difference is much larger than actual luminance increasing from `0.8` -> `0.9`

![Keine Ahnung](images/nonsense-not-really-maf.png)

*Gamma encoding* is different from actually displaying the image
* Once we applied *gamma encoding* to an image, we want to display it on a screen *in linear space*
* Apply the inverse gamma to the gamma-encoded image so that we can *perceive* the image brightness as increasing linearly
* Gamma encoding is applied *independent* of the display technology!!
  * It is merely a tool to encode the brightness more efficiently for human perception

![Gamma encoding](images/gamma-encoding.png)

TILs
* [**psychophysics**](https://en.wikipedia.org/wiki/Psychophysics) tries to quantify the relationship between physical stimuli and how we perceived them / sensations
* The brightness humans perceived is part of [**Stevens Power Law**](https://en.wikipedia.org/wiki/Stevens%27s_power_law)


I think nothing above had anything to do with the filter OTL

We apply `e^x` to the value from the UI `x`. If `x` is negative, we get a fractional exponent, and otherwise we get a postive value > 1. This gives us to exponent to raise each pixel value (`gamma` value):

![e^x](images/e-to-da-x.png)

With a fractional exponent, we get brighter images, and exponents > 1, we get darker images:

![Gamma results](images/gamma-results.png)


### Vignette
We have the diagonal of the image `D`. Everything outside of `outerR` is black. Everything inside of `innerR` is clear/untouched. The color values between `innerR` and `outerR` slowly become darker.

![Vignette image labels](images/vignette-image-diagonal.png)

For **linear falloff**, we see how far the point `(x,y)` is along the image's half-diagonal `D/2`, then we get the proportional of where the point is in the ring that `innerR` and `outerR` creates.

We subtract 1 from the result so that the pixels are brighter toward the center.

![Vignette linear falloff](images/vignette-linear.png)

TODO: Fix logic because something ain't right...

### Histogram Equalization

There are clusters of intensity values that are close together, that's why there is little contrast.
Equalization stretches this range out to the full spectrum of the lightness levels (ex. the lightnest pixel in the image will map to the lightest possible level). This makes the histogram "flatter" in the sense that selected lightness values are more spread out across the spectrum, *not* that the counts of each lightness level are redistributed equally (I mistakenly thought that this was what "flatter" meant).

## Color Operations

### Grayscale

[**Grayscale**](https://en.wikipedia.org/wiki/Grayscale) images display the light intensity/luminance of each pixel.

To [convert from color to grayscale](https://en.wikipedia.org/wiki/Grayscale#Converting_color_to_grayscale), we compute the linear luminance (`Y`, from CIE XYZ):

```
(Red, Green, Blue) = pixel
Y = luminance = 0.2126*Red + 0.7152*Green + 0.0722*Blue
```

The coefficients that are multiplied to each color channel is related to how sensitive the average human eye is to the intensity/luminance that color.
* For example, human vision is most sensitive to green (so in the luminance equation, it has the highest coefficient: `0.7152`), and the least sensitve to blue.
* If we add the coefficients, we get `1`:

```
0.2126 + 0.7152 + 0.0722 = 1
```

### Saturation

Reading:
* [Image processing by interpolation and extrapolation](http://graficaobscura.com/interp/index.html)
* [Image interpolation/extrapolation in an OpenGL context](http://www.yaldex.com/open-gl/ch19lev1sec5.html)

To blend two images together, we **linearly interpolate** them:
```
out = (1 - alpha)*in0 + alpha*in1
```

* If `alpha` is in range `[0, 1]`, we interpolate between the two images
* If `alpha` > 1.0 will decrease `in0` and scale `in1`, extrapolating between the two images
* If `alpha` < 1.0, it will scale `in0` and decrease `in1`

We could think of **extrapolation** "away from" a degenerate image as:
* Going from a blurry image (the degenerate image) to a sharp image
* Black-and-white image to a saturated image

We could obtain the `alpha` value in different ways to achieve different visual effects
* Ex. use a function to compute `alpha` using `X` and `Y`
* Brush controls alpha depending on where the cursor is, where the pixel is from the center of the brush

Visual effects using interpolation and extrapolation
* Brightness: use pure black image as the degenerate image
  * Interpolation darkens the image
  * Extrapolation brightens the image
* Contrast: use constant grey image with the average image luminance
  * Interpolation reduces contrast
  * Extrapolation increases contrast
  * Negative alpha inverts the image
  * Average luminance is always preserved
* **Saturation**: want to either move away from or toward the pixel's luminance value.
  * Use a ~~black and white~~ grayscale image as the degenerate image


`in0` is the grayscale (luminance) pixel (when alpha=0, then the in1 value is disregarded) and `in1` is the source image.
If `alpha=0`, then `ratio=alpha-1=0-1=-1`, which results in a grayscale image.

```
out = (1 - alpha)*in0 + alpha*in1
```

Example with light purple (red=0.77, green=0.54, blue=0.87):
![Purple original](images/purple-original.png)

The degenerate image is the grayscale image, which is the luminance value of the pixel (recall, [color to grayscale](https://en.wikipedia.org/wiki/Grayscale#Converting_color_to_grayscale)):
```
luminance = 0.2126*red + 0.7152*green + 0.072*blue
```

For our light purple, calculating the luminance value:
```python
light_purple = Pixel(red=0.77, green=0.54, blue=0.87)
luminance = 0.2126*light_purple.red + 0.7152*light_purple.green + 0.072*light_purple.blue
> ~0.61
```
We roughly get luminance=0.61.

To get a more saturated purple, we extrapolate using an `alpha` value greater than 1.
Extrapolating with `alpha=2`:
```python
# in0 is our degenerate image, which is grayscale in this case
# in0 is the luminance value of the pixel
# in1 is the orignal pixel value
# out = (1-alpha)*in0 + alpha*in1
# luminance for light_purple = 0.61
out_red = (1-2)*0.61 + 2*light_purple.red
> ~0.93
out_green = (1-2)*0.61 + 2*light_purple.green
> ~0.47
out_blue = (1-2)*0.61 + 2*light_purple.blue
# Here we have to clamp the value since it is greater than 1.0
> ~1.87
> 1.0 
```

Original purple: Pixel(red=0.77, green=0.54, blue=0.87)
Saturated purple: Pixel(red=0.93, green=0.47, blue=1.0)

It is indeed more saturated!
![Saturated purple](images/purple-saturated.png)

Doing this napkin math by hand was very helpful.

## Etsy

```javascript
let lightness in Object.keys(cdf)
```
is not the same as

```javascript
let lightness of Object.keys(cdf)
```

`of` will get you the keys you added. `in`...??? Was zum Teufel OTL

## Resources

* [Short explanations on GIMP image processing implementations](https://pippin.gimp.org/image-processing/) and other notes on GIMP
