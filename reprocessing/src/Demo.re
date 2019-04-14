
type state = {
  mutable attraction: bool,
  mutable debug: bool,
  mousePos: ReasonPhysics.Vec2d.vec2d,
  windowOrigin: ReasonPhysics.Vec2d.vec2d,
  windowBounds: ReasonPhysics.Vec2d.vec2d,
};
 

let rec range = (i, j) =>
  if (i > j) {
    [];
  } else {
    [i, ...range(i + 1, j)];
  };

let init_state = (): state => {
  attraction: true,
  debug: false,
  mousePos: ReasonPhysics.Vec2d.create(100.0, 300.0),
  windowOrigin: ReasonPhysics.Vec2d.create(0.0, 0.0),
  windowBounds: ReasonPhysics.Vec2d.create(0.0, 0.0),
};