[@bs.val] [@bs.scope "Date"]
external now : unit => float = "now";

type physicsState = {
  mutable accumulatedTime: float,
  mutable clock: float,
};

let maxSteps = 4;
let timestep =  1.0 /. 60.0;

let create = () => {
  accumulatedTime: 0.0,
  clock: 0.0,
};

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

let integrateBodies = (bodies, dt, drag) => {
    List.iteri((index, body) => {
      Body.update(body, dt, index);
    }, bodies);

    integrateMotion(bodies, dt, drag);
};

let step = (physics: physicsState, bodies: list(Body.body)) => {
    /* Initialise the clock on first step. */
    if (physics.clock == 0.0) {
      physics.clock = now();
    };

    /* Compute delta time since last step. */
    let time = now();
    let delta = time -. physics.clock;

    /* sufficient change. */
    if (delta > 0.0) {
      /* Convert time to seconds. */
      let delta = delta *. 0.001;

      let viscosity = 0.005;
      /* Drag is inversely proportional to viscosity. */
      let drag = 1.0 -. viscosity;

      /* Update the clock. */
      physics.clock = time;

      /* Increment time accumulatedTime. */
      physics.accumulatedTime = physics.accumulatedTime +. delta;

      /* Integrate until the accumulatedTime is empty or until the */
      /* maximum amount of iterations per step is reached. */

      let i = ref(0);
      while (physics.accumulatedTime >= timestep && i^ < maxSteps) {
        /* Integrate bodies by fixed timestep. */
        integrateBodies(bodies, timestep, drag);

        /* Reduce accumulatedTime by one timestep. */
        physics.accumulatedTime = physics.accumulatedTime -. timestep;
        i := i^ + 1;
      };
    };
};

  