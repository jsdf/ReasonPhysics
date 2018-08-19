[%%debugger.chrome];

/* for now all bodies are circles. later we can make this a variant type which
   supports circle, rect, etc bodies */
type body = {
  id: int,
  mass: float,
  massInverse: float,
  radius: float,
  radiusSquared: float,
  restitution: float,
  prevPosition: Vec2d.vec2d,
  position: Vec2d.vec2d,
  velocity: Vec2d.vec2d,
  nonIntegralVelocity: Vec2d.vec2d,
  acceleration: Vec2d.vec2d,
  prevAcceleration: Vec2d.vec2d,
  mutable behaviours: list(behaviour)
}
and behaviour = (body, float, int) => unit;

let create = (~mass, ~radius, ~position, ~behaviours, ~id) => {
  id,
  mass,
  massInverse: 1.0 /. mass,
  radius,
  radiusSquared: radius *. radius,
  restitution: 1.0,
  position: Vec2d.clone(position),
  prevPosition: Vec2d.clone(position),
  velocity: Vec2d.origin(),
  nonIntegralVelocity: Vec2d.origin(),
  acceleration: Vec2d.origin(),
  prevAcceleration: Vec2d.origin(),
  behaviours
};

/* run all the behaviours for this body for some delta time */
let update = (body, dt, index) =>
  List.iter(
    (behaviour) =>
      /* Js.log2("acc before",body.acceleration.y); */
      behaviour(body, dt, index),
    /* Js.log2("acc after",body.acceleration.y); */
    body.behaviours
  );