/* for now all bodies are circles. later we can make this a variant type which 
supports circle, rect, etc bodies */
type body = {
  massInverse: float,
  radius: float,
  radiusSquared: float,
  prevPosition: Vec2d.vec2d,
  position: Vec2d.vec2d,
  velocity: Vec2d.vec2d,
  acceleration: Vec2d.vec2d,
  behaviours: list(behaviour),
} and behaviour = (body, float, int) => unit;

let create = (mass, radius, position, behaviours) => {
  {
    massInverse: 1.0 /. mass,
    radius,
    radiusSquared: radius *. radius,
    position,
    prevPosition: position,
    velocity: Vec2d.origin(),
    acceleration: Vec2d.origin(),
    behaviours,
  }
};

/* run all the behaviours for this body for some delta time */
let update = (body, dt, index) => {
  List.iter((behaviour) => behaviour(body, dt, index), body.behaviours);
};
