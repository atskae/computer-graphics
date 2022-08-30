"use strict";

var Filters = Filters || {};

////////////////////////////////////////////////////////////////////////////////
// General utility functions
////////////////////////////////////////////////////////////////////////////////

// Constrain val to the range [min, max]
function clamp(val, min, max) {
  /* Shorthand for:
   * if (val < min) {
   *   return min;
   * } else if (val > max) {
   *   return max;
   * } else {
   *   return val;
   * }
   */
  return ((val < min) ? min : ((val > max) ? max : val));
}

// extract vertex coordinates from a URL string
function stringToCoords( vertsString ) {
  var centers = [];
  var coordStrings = vertsString.split("x");
  var coordsSoFar = 0;
  for (var i = 0; i < coordStrings.length; i++) {
    var coords = coordStrings[i].split("y");
    var x = parseInt(coords[0]);
    var y = parseInt(coords[1]);
    if (!isNaN(x) && !isNaN(y)) {
      centers.push({x: x, y: y})
    }
  }

  return centers;
}

////////////////////////////////////////////////////////////////////////////////
// Filters
////////////////////////////////////////////////////////////////////////////////

// Fill the entire image with color
Filters.fillFilter = function( image, color ) {
  for (var x = 0; x < image.width; x++) {
    for (var y = 0; y < image.height; y++) {
      // uncomment this line to enable this function
      image.setPixel(x, y, color);
    }
  }
  return image;
};

// At each center, draw a solid circle with the specified radius and color
Filters.brushFilter = function( image, radius, color, vertsString ) {
  // centers is an array of (x, y) coordinates that each defines a circle center
  var centers = stringToCoords(vertsString);

  // draw a filled circle centered at every location in centers[].
  // radius and color are specified in function arguments.
  // ----------- STUDENT CODE BEGIN ------------
  // ----------- Our reference solution uses 10 lines of code.
  console.log("Filling in " + centers.length + " circles with brush filter");
  for (const center of centers) {
    // Iterate through the pixels within the square that bounds the circle
    // If the point is within the circle, color in the pixel
    for (let x=center.x-radius; x<center.x+radius; x++) {
      // Skip this point if it is out of bounds of the bounding square
      // The circle/brush could be out of bounds at the image corners
      if (x<0) { continue; }
      
      for (let y=center.y-radius; y<center.y+radius; y++) {
          if (y<0) { continue; }
          // Compute the distance from the circle center and the point
          let distance = Math.sqrt(Math.pow(x-center.x, 2) + Math.pow(y-center.y, 2));
          
          // Outside the circle bounds; do not fill
          if (distance >= radius) { continue; }
          else {
            // The point is inside the circle; fill it in!
            //console.log("Filling at (" + x + "," + y + ")");
            image.setPixel(x, y, color);
          }
      }
    }
  }
  // ----------- STUDENT CODE END ------------
  //Gui.alertOnce ('brushFilter is not implemented yet');

  return image;
};

/*
 * At each center, draw a soft circle with the specified radius and color.
 * Pixel opacity should linearly decrease with the radius from alpha_at_center to 0.
 * 
 * https://en.wikipedia.org/wiki/Alpha_compositing#Description
 */
Filters.softBrushFilter = function( image, radius, color, alpha_at_center, vertsString ) {
  // centers is an array of (x, y) coordinates that each defines a circle center
  var centers = stringToCoords(vertsString);

  // draw a filled circle with opacity equals to alpha_at_center at the center of each circle
  // the opacity decreases linearly along the radius and becomes zero at the edge of the circle
  // radius and color are specified in function arguments.
  // ----------- STUDENT CODE BEGIN ------------
  // ----------- Our reference solution uses 21 lines of code.
  console.log("Filling in " + centers.length + " circles with soft brush filter");
  // Alpha/transparency of the image is constant (not see-through)
  let alpha_b = 1.0;
  for (const center of centers) {
    // Iterate through the pixels within the square that bounds the circle
    // If the point is within the circle, color in the pixel
    for (let x=center.x-radius; x<center.x+radius; x++) {
      // Skip this point if it is out of bounds of the bounding square
      // The circle/brush could be out of bounds at the image corners
      if (x<0) { continue; }
      
      for (let y=center.y-radius; y<center.y+radius; y++) {
          if (y<0) { continue; }
          // Apply image composition `A over B`, where A is the foreground and B is the background
          // A is the soft brush, and B is the image
         
          // Compute the distance from the circle center and the point
          let distance = Math.sqrt(Math.pow(x-center.x, 2) + Math.pow(y-center.y, 2));
          
          // Outside the circle bounds; do not fill
          if (distance >= radius) { continue; }
          else {
            // The alpha value decreases linearly from the circle center to the edge starting from alpha_at_center
            // alpha = (-1 * alpha_at_center/radius) * distance + alpha_at_center
            //    distance=0 -> alpha_at_center
            //    distance=radius -> 0
            // Compute the alpha at this distance from the center
            // Slope = -1 * alpha_at_center / radius
            let alpha_a = (-1 * alpha_at_center/radius) * distance + alpha_at_center;
            //console.log("alpha_a: " + alpha_a + ", at distance: " + distance);
           
            let alpha_over = alpha_a + alpha_b * (1 - alpha_a);
            let color_image = image.getPixel(x, y);

            // Compute the final color of the brush over the image, then set the new color in the image
            // C_o = final color; C_a = brush color; C_b = image color
            // C_o = (C_a*alpha_a + C_b*alpha_b*(1-alpha_a)) / alpha_over
            let left_arg = color.multipliedBy(alpha_a); 
            let right_arg = color_image.multipliedBy(alpha_b).multipliedBy(1 - alpha_a);
            let numerator = left_arg.plus(right_arg);
            let final_color = numerator.dividedBy(alpha_over);
            //console.log("Final color after soft brush: " + final_color);
            image.setPixel(x, y, final_color);
          }
      }
    }
  }

  // ----------- STUDENT CODE END ------------
  //Gui.alertOnce ('softBrushFilter is not implemented yet');

  return image;
};

Filters.customFilter = function( image, value ) {
  // You can use this filter to do whatever you want, for example
  // trying out some new idea or implementing something for the
  // art contest.
  // Currently the 'value' argument will be 1 or whatever else you set
  // it to in the URL. You could use this value to switch between
  // a bunch of different versions of your code if you want to
  // code up a bunch of different things for the art contest.
  // ----------- STUDENT CODE BEGIN ------------
  // ----------- Our reference solution uses 0 lines of code.
  // ----------- STUDENT CODE END ------------
  Gui.alertOnce ('customFilter is not implemented yet');
  return image;
};
