/*
    References to the objects in the HTML page
*/
const canvas = document.getElementById("drawing-board");
const canvasDiv = canvas.parentElement;
const toolbar = document.getElementById("toolbar");
const ctx = canvas.getContext("2d");

// Get the canvas' top-left corner's position in browser
const canvasOffsetX = canvas.offsetLeft;
const canvasOffsetY = canvas.offsetTop;

// Set the canvas dimensions to overrid default sizes
// The default size is applied at the beginning, regardless of
//  how large the canvas appears in the window
//canvas.width = window.innerWidth - canvasOffsetX;
//canvas.height = window.innerHeight - canvasOffsetY;

console.log("Canvas width: " + canvas.width)
console.log("Canvas height: " + canvas.height)

const canvasDivStyle = window.getComputedStyle(canvasDiv);
const canvasDivMarginLeft = parseInt(canvasDivStyle.marginLeft.replace("px", ""));
const canvasDivMarginTop = parseInt(canvasDivStyle.marginTop.replace("px", ""));
console.log("Canvas left margin: " + canvasDivMarginLeft);
console.log("Canvas top margin: " + canvasDivMarginTop);

// Keep track of the top-left and botton-right pixels that have been drawn
// The point closest to the topLeftCorner
let topLeftDrawn = [0, 0];
let closestDistanceTopLeft = Infinity;
// The point closest to the bottomRightCorner
let bottomRightDrawn = [canvas.width, canvas.height]; 
let closestDistanceBottomRight = Infinity;

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
    Utility functions
*/

// Calculate the distance between the 2D points: p1 and p2
function getDistance(p1, p2) {
    //console.log("getDistance() called with: " + p1 + " and " + p2);
    let dx = p1[0]-p2[0];
    let dy = p1[1]-p2[1];
    return Math.sqrt(Math.pow(dx, 2) + Math.pow(dy, 2));
}

// Update the closest points seen
// (x. y) is the new point being drawn
function updateClosestToCorners(p) {
    //console.log("updateClosetCorners() called with: " + p);
    let dist = getDistance([0, 0], p);
    //console.log("Distance between topLeftCorner and " + p + " is " + dist);
    //console.log("Shorter distance found: " + dist < closestDistanceTopLeft);
    if (dist < closestDistanceTopLeft) {
        closestDistanceTopLeft = dist;
        topLeftDrawn = p;
        console.log("Updated closestDistanceTopLeft: " + p + " (" + dist + ")");
    }

    dist = getDistance([canvas.width, canvas.height], p);
    if (dist < closestDistanceBottomRight) {
        closestDistanceBottomRight = dist;
        bottomRightDrawn = p;
        console.log("Updated closestDistanceBottomRight: " + p + " (" + dist + ")");
    }
}

// Clear the canvas and closest drawn points (to corners)
function resetCanvas() {
    ctx.clearRect(0, 0, canvas.width, canvas.height);
    
    // Reset the closest points we've seen
    topLeftDrawn = [0, 0];
    closestDistanceTopLeft = Infinity;
    bottomRightDrawn = [canvas.width, canvas.height]; 
    closestDistanceBottomRight = Infinity;
}

/*
    Event listeners
*/

// Javascript "arrow function" ~= lambda
toolbar.addEventListener("click", e => {
    if (e.target.id == "clear") {
        //console.log("Clear button was clicked!");
        // Set all pixels in the canvs to "transparent black", which appears white
        
        // clearRect(x, y, width, height)
        //  where (x, y) is the top-left corner of the canvas
        let clearCanvas = confirm("Are you sure you want to clear the canvas?");
        if (clearCanvas) {
            resetCanvas();
        }
    }
});

toolbar.addEventListener("change", e => {
    if (e.target.id == "color") {
        ctx.strokeStyle = e.target.value;
        console.log("Color was changed to: " + ctx.strokeStyle);
    }
    else if (e.target.id == "lineWidth") {
        // Equivalent to: lineWidth = document.getElementById("lineWidth").value;
        lineWidth = e.target.value;
        // console.log("Line width changed to: " + lineWidth);
    }
});

canvas.addEventListener("mousedown", e => {
    startX = e.offsetX;
    startY = e.offsetY;
    updateClosestToCorners([startX, startY]);
    isPainting = true;
});

canvas.addEventListener("mousemove", e => {
    //console.log("Mouse position: (" + e.offsetX + ", " + e.offsetY + ")");
    //console.log("isPainting: " + isPainting);
    if (!isPainting) {
        return;
    }

    ctx.lineWidth = lineWidth;
    ctx.lineCap = "round";
    // lineTo(x, y)
    //  We subtract the toolbar's width
    ctx.lineTo(e.clientX - canvasOffsetX, e.clientY - canvasDivMarginTop);
    ctx.stroke();
});


canvas.addEventListener("mouseup", e => {
    startX = e.offsetX;
    startY = e.offsetY;
    isPainting = false;
    // Draw the line
    ctx.stroke();
    ctx.beginPath();
});
