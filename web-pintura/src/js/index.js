/*
    References to the objects in the HTML page
*/
const canvas = document.getElementById("drawing-board");
const toolbar = document.getElementById("toolbar");
const ctx = canvas.getContext("2d");

// Get the canvas' top-left corner's position
const canvasOffsetX = canvas.offsetLeft;
const canvasOffsetY = canvas.offsetTop;
console.log("Upper-left corner of canvas: (" + canvasOffsetX + ", " + canvasOffsetY + ")")

// Set the canvas dimensions to overrid default sizes
// The default size is applied at the beginning, regardless of
//  how large the canvas appears in the window
canvas.width = window.innerWidth - canvasOffsetX;
canvas.height = window.innerHeight - canvasOffsetY;

console.log("Canvas width: " + canvas.width)
console.log("Canvas height: " + canvas.height)

/*
    Global variables
*/
let isPainting = false;
let lineWidth = document.getElementById("lineWidth").value;
// The position (x,y) in the canvas of where we start drawing from
let startX = 0;
let startY = 0;

console.log("lineWidth: " + lineWidth);

/*
    Event listeners
*/
toolbar.addEventListener(click, e => {
    
});
