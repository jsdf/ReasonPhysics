open Reprocessing;

Random.self_init();

let random = () => Random.float(1.0);

let nBodies = 10;

type state = {
  bodies: array(ReasonPhysics.Body.body),
  demoState: Demo.state,
  physics: ReasonPhysics.Physics.physicsState,
};

let init_state = (): state => {
  let demoState = Demo.init_state();
  /* set up the physics behaviours particular to the demo */
  let attractionRadiusSquared = 10.0 *. 10.0;
  let attraction = (body: ReasonPhysics.Body.body, _dt, index) => {
    let strength = 1.0 +. float_of_int(index + 1) *. 0.1;
    let delta = ReasonPhysics.Vec2d.origin();
    /* Vector pointing from body to target. */
    ReasonPhysics.Vec2d.copy(delta, demoState.mousePos);
    ReasonPhysics.Vec2d.sub(delta, body.position);
    /* Squared distance to target. */
    let distSq = ReasonPhysics.Vec2d.magSq(delta);
    ReasonPhysics.Vec2d.norm(delta);
    ReasonPhysics.Vec2d.scale(delta, distSq /. attractionRadiusSquared);
    /* Apply force. */
    ReasonPhysics.Vec2d.scale(delta, strength);
    ReasonPhysics.Vec2d.add(body.acceleration, delta);
  };
  let maybeAttraction = (body, dt, index) =>
    if (demoState.attraction) {
      attraction(body, dt, index);
    };
  let gravity = (body: ReasonPhysics.Body.body, _dt, _index) =>
    ReasonPhysics.Vec2d.add(
      body.acceleration,
      ReasonPhysics.Vec2d.create(0.0, 98.0 *. body.mass),
    );
  let maybeGravity = (body: ReasonPhysics.Body.body, dt, index) =>
    if (!demoState.attraction) {
      gravity(body, dt, index);
    };
  let edgeBounce =
    ReasonPhysics.Behaviour.edgeBounce(
      ~min=demoState.windowOrigin,
      ~max=demoState.windowBounds,
    );
  let bodies: array(ReasonPhysics.Body.body) =
    Array.of_list(
      List.map(
        i =>
          ReasonPhysics.Body.create(
            ~id=i,
            ~mass=10.0,
            ~radius=20.0,
            ~position=
              ReasonPhysics.Vec2d.create(
                random() *. 600.0,
                random() *. 600.0,
              ),
            ~behaviours=[maybeAttraction, maybeGravity, edgeBounce],
          ),
        Demo.range(0, nBodies - 1),
      ),
    );
  let collision = ReasonPhysics.Behaviour.collision(bodies);
  Array.iter(
    (body: ReasonPhysics.Body.body) =>
      body.behaviours = [collision, ...body.behaviours],
    bodies,
  );

  let physics = ReasonPhysics.Physics.create(~viscosity=0.02);

  {demoState, physics, bodies};
};

let setup = env => {
  Env.size(~width=1000, ~height=600, env);
  init_state();
};

let mouseMove = (state, env) => {
  let {demoState} = state;
  let (mouseX, mouseY) = Env.mouse(env);

  demoState.mousePos.x = float_of_int(mouseX);
  demoState.mousePos.y = float_of_int(mouseY);
  demoState.windowBounds.x = float_of_int(Env.width(env));
  demoState.windowBounds.y = float_of_int(Env.height(env));

  state;
};

let draw = (state, env) => {
  let {demoState, physics, bodies} = state;

  print_endline("draw");

  ReasonPhysics.Physics.step(physics, bodies);

  // render
  Draw.background(Utils.color(~r=255, ~g=217, ~b=229, ~a=255), env);
  
  // drawBodies
  Draw.fill(Utils.color(~r=41, ~g=166, ~b=244, ~a=255), env);
  Array.iteri(
    (i, body: ReasonPhysics.Body.body) =>
      Draw.ellipse(
        ~center=(
          int_of_float(body.position.x),
          int_of_float(body.position.y),
        ),
        ~radx=int_of_float(body.radius),
        ~rady=int_of_float(body.radius),
        env,
      ),
    bodies,
  );
  state;
};

let start = () => run(~setup, ~draw, ~mouseMove, ());
