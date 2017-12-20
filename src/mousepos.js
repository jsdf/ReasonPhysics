let mousepos = {x: 0, y: 0};

document.addEventListener('mousemove', event => {
  mousepos.x = event.clientX;
  mousepos.y = event.clientY;
});

module.exports = {
  getMouseX() {
    return mousepos.x;
  },
  getMouseY() {
    return mousepos.y;
  },
};
