[@bs.val] [@bs.scope "Date"]
external now : unit => float = "now";

type worldState = {
  mutable accumulatedTime: float,
  mutable clock: float,
  mutable maxSteps: int,
  mutable timestep: float,
  mutable bodies: list(Body.body),
};

let timestep =  1.0 /. 60.0;

let integrateMotion = (bodies, dt, drag) => {
  let newPosition = Vec2d.origin();
  let dtSquared = dt *. dt;
  List.iter(
    (body: Body.body) => {
      /* Scale force to mass. */
      Vec2d.scale(body.acceleration, body.massInverse);
      /* Derive velocity. */
      Vec2d.copy(body.velocity, body.position);
      Vec2d.sub(body.velocity, body.prevPosition);
      /* Apply friction. */
      Vec2d.scale(body.velocity, drag);

      /* Apply acceleration force to new position. */

      Vec2d.copy(newPosition, body.position);

      /* Get integral acceleration, apply to velocity, then apply updated 
      velocity to position */
      Vec2d.scale(body.acceleration, dtSquared);
      Vec2d.add(body.velocity, body.acceleration);
      Vec2d.add(newPosition, body.velocity);

      /* Store old position, update position to new position. */
      Vec2d.copy(body.position, body.prevPosition);
      Vec2d.copy(body.position, newPosition);

      /* Reset acceleration force. */
      Vec2d.clear(body.acceleration)
    },
    bodies
  );
};

let integrateWorld = (bodies, dt, drag) => {
    List.iteri((index, body) => {
      Body.update(body, dt, index);
    }, bodies);

    integrateMotion(bodies, dt, drag);
};

let step = (world: worldState) => {
    /* Initialise the clock on first step. */
    if (world.clock == 0.0) {
      world.clock = now();
    };

    /* Compute delta time since last step. */
    let time = now();
    let delta = time -. world.clock;

    /* sufficient change. */
    if (delta > 0.0) {
      /* Convert time to seconds. */
      let delta = delta *. 0.001;

      let viscosity = 0.005;
      /* Drag is inversely proportional to viscosity. */
      let drag = 1.0 -. viscosity;

      /* Update the clock. */
      world.clock = time;

      /* Increment time accumulatedTime. */
      world.accumulatedTime = world.accumulatedTime +. delta;

      /* Integrate until the accumulatedTime is empty or until the */
      /* maximum amount of iterations per step is reached. */

      let i = ref(0);
      while (world.accumulatedTime >= timestep && i^ < world.maxSteps) {
        /* Integrate motion by fixed timestep. */
        integrateWorld(world.bodies, timestep, drag);

        /* Reduce accumulatedTime by one timestep. */
        world.accumulatedTime = world.accumulatedTime -. timestep;
        i := i^ + 1;
      };
    };
};

  