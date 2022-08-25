# Digital Image Processing

Types of image processing operations:
* Luminance
* Color
* Linear filtering (ex. blur)
* Non-linear filtering 
* Dithering

## Luminance

**Luminance** is how bright/white a color is (need a more technical term)

Image processing operations:
* Brightness
* Contrast
* Gamma
* Histogram equalization

Brightness
* Can convert to a different color model (HSL) and adjust the brightness component
* Increase values of each component in RGB

Contrast
* Get the average brightness across all pixels in the image, `L*`
* For each pixel, get the difference between the the pixel's brightness and the mean brightness `L*`
  * Then scale the brightness of the pixel with this difference

Gamma

CIE XYZ is a *linear* color space
* If we multiply a color by a certain factor, the brightness is also multiplied by this factor
* But human color perception (brightness adaptation of our eyes) and display devices are *not* linear

* Brightness encoded in an image is different from how the brightness is percieved
* Apply a non-linear function to account for this difference
* More on [gamma correction (Scratchapixel)](https://www.scratchapixel.com/lessons/digital-imaging/digital-images)

[Histogram Equalization](https://towardsdatascience.com/histogram-equalization-5d1013626e64)

* A histogram with x-axis as intensity value and y-axis as the number of pixels with that intensity value
* Histogram equalization improves contrast in an image
* To apply to an RGB image, convert to HSV and adjust the brightness (V) without affecting hue/saturation

## Color

Convert to grayscale, either:
* Convert to HSL, set S=0 (saturation), convert back to RGB
* Set each component of RGB of every pixel to L

Adjusting saturation, either:
* Convert to HSL, scale S, convert back
* `R' = L + scale * (R-L)`
  * Apply to other pixel components (G, B)

[White balance](https://photographylife.com/definition/white-balance)
* *White balance* balances the color temperature in your image to a *neutral color temperature* (sunlight at noon)
  * Candle flame (warm/orange)
  * Sunlight and flash (neutral)
  * Heavy overcast sky (cool/blue)
* Opposite colors are added to neutralize the temperature
  * So whites that appear blue/orange look white after white balance; removes color cast
  * White balance makes the image "look more natural"

Von Kries method for white balance
* Adjust colors in [LMS color space](https://en.wikipedia.org/wiki/LMS_color_space) (long, medium, and short wavelengths sensed by the cones in our eyes)

Adjusting white balance:
1. Convert RGB to XYZ color space
2. Convert XYZ to LMS color space
3. Divide each pixel component by `LMS_w`, the color white in the LMS color space
4. Convert image back to RGB

## Linear Filtering

Blur
* Convolution
  * Weighted sum of values
  * Pattern of weights = filter/kernel
    * Ex. Box filter, triangle filter, Gaussian

2D Convolution
* For each pixel, compute linear combination of pixels within the kernel/filter
  * The kernel/filter determines how to compute this linear combination

Different filters for different operations
* Gaussian Blur, edge detection, emboss

Edge Detection
* Convolve with a *Laplacian filter* which finds the differences between neighboring pixels

Sharpen
* Add edges computed in edge detection to the original image

## Separable Filters
* Can apply filter across row first, then columns (ex. possible with Gaussian filter)

## Non-linear Filtering
* Apply non-linear functions to pixels
* The filter depends on the input

Examples: median filter, bilateral filter

Bilaterial filter - blur but preserve edges (See paper under Readings)

## [Dithering](https://en.wikipedia.org/wiki/Dither#Digital_photography_and_image_processing)

Quantization
* Reduce resolution of intensity by mapping values to integers
  * Apply quantization due to limited frame buffer sizes, limited ranges for pixel values
* Uniform quantization
* [Halftoning](https://en.wikipedia.org/wiki/Halftone): using dots to give the appearance of darker/lighter colors
  * Ex. comic book style

Dithering algorithms: random dithering, [ordered](https://en.wikipedia.org/wiki/Ordered_dithering), error diffusion, Floyd-Steinberg

[Ordered Dithering (Computerphile!)](https://www.youtube.com/watch?v=IviNO7iICTM&ab_channel=Computerphile)

[Error diffusion dithering (Computerphile!)](https://www.youtube.com/watch?v=ico4fJfohMQ&ab_channel=Computerphile)
* Talks about Floyd-Steinberg
  * For each pixel, we see how off the pixel value is (from white? ), then compensate for this error across the neighboring pixels
    * Make pixel brighter, make surrounding pixels darker (compensate for error), which overall gives a gray appearance
  * Floyd-Steinberg chooses hard-coded values of how the error is distributed

## Readings
* [A pixel is not a little square!](https://www.cs.princeton.edu/courses/archive/spring22/cos426/readings/Smith95b.pdf)
  * [Graphics Programing Virtual Meetup discussion](https://www.youtube.com/watch?v=gpGc7N1vsQc&ab_channel=GraphicsProgrammingVirtualMeetup)
* [Bilateral Filtering](https://www.cs.princeton.edu/courses/archive/spring22/cos426/readings/Paris09.pdf)
  * Sections 1-2
