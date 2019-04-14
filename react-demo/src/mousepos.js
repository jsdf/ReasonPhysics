let mousepos = { x: 0, y: 0 };

document.addEventListener("mousemove", function(event) {
  mousepos.x = event.clientX;
  mousepos.y = event.clientY;
});

function handleTouch(event) {
  var firstTouch = event.touches[0];
  if (firstTouch) {
    mousepos.x = firstTouch.clientX;
    mousepos.y = firstTouch.clientY;
  }
}

document.addEventListener("touchmove", handleTouch);

document.addEventListener("touchstart", handleTouch);

module.exports = {
  hasTouch() {
    return "ontouchstart" in window;
  },
  getMouseX() {
    return mousepos.x;
  },
  getMouseY() {
    return mousepos.y;
  }
};
