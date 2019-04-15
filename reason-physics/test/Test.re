let nBodies = 10;
let attractionRadiusSquared = 10.0 *. 10.0;

let rec range = (i, j) =>
  if (i > j) {
    [];
  } else {
    [i, ...range(i + 1, j)];
  };

/* set up the physics behaviours particular to the demo */
let attraction = (body: Body.body, _dt, index) => {
  let strength = 1.0 +. float_of_int(index + 1) *. 0.1;
  let delta = Vec2d.origin();
  /* Vector pointing from body to target. */
  Vec2d.sub(delta, body.position);
  /* Squared distance to target. */
  let distSq = Vec2d.magSq(delta);
  Vec2d.norm(delta);
  Vec2d.scale(delta, distSq /. attractionRadiusSquared);
  /* Apply force. */
  Vec2d.scale(delta, strength);
  Vec2d.add(body.acceleration, delta);
};
let gravity = (body: Body.body, _dt, _index) =>
  Vec2d.add(body.acceleration, Vec2d.create(0.0, 98.0 *. body.mass));
let edgeBounce =
  Behaviour.edgeBounce(~min=Vec2d.origin(), ~max=Vec2d.create(10.0, 10.0));
let bodies: array(Body.body) =
  Array.of_list(
    List.map(
      i =>
        Body.create(
          ~id=i,
          ~mass=10.0,
          ~radius=20.0,
          ~position=Vec2d.create(600.0, 600.0),
          ~behaviours=[attraction, gravity, edgeBounce],
        ),
      range(0, nBodies - 1),
    ),
  );
let collision = Behaviour.collision(bodies);
Array.iter(
  (body: Body.body) => body.behaviours = [collision, ...body.behaviours],
  bodies,
);

let physics = Physics.create(~viscosity=0.02);