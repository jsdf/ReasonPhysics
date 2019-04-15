let now = Platform.now;
// [@bs.val] [@bs.scope "Date"] external now: unit => float = "now";

type physicsState = {
  mutable accumulatedTime: float,
  mutable clock: float,
  mutable viscosity: float,
  mutable simulationRate: float,
  mutable timeScale: float,
  mutable dynamicTimestep: bool,
};

let maxSteps = 4;

let timestep = 1.0 /. 60.0;

let create = (~viscosity: float) => {
  accumulatedTime: 0.0,
  clock: 0.0,
  viscosity,
  simulationRate: 1.0,
  timeScale: 1.0,
  dynamicTimestep: true,
};

let integrateMotion = (bodies, dt, drag) => {
  let newPosition = Vec2d.origin();
  Array.iteri(
    (_i, body: Body.body) => {
      /* Scale force to mass. */
      Vec2d.scale(body.acceleration, body.massInverse);
      /* Derive velocity. */
      Vec2d.copy(body.velocity, body.position);
      Vec2d.sub(body.velocity, body.prevPosition);
      /* Apply friction. */
      Vec2d.scale(body.velocity, drag);
      /* Apply acceleration force to new position. */
      /* Get integral acceleration, apply to velocity, then apply updated
         velocity to position */
      Vec2d.copy(newPosition, body.position);
      Vec2d.scale(body.acceleration, dt);
      Vec2d.add(body.velocity, body.acceleration);
      Vec2d.add(newPosition, body.velocity);
      /* Store old position, update position to new position. */
      Vec2d.copy(body.prevPosition, body.position);
      Vec2d.copy(body.position, newPosition);
      /* Reset acceleration force. */
      Vec2d.copy(body.prevAcceleration, body.acceleration);
      Vec2d.clear(body.acceleration);
      /* store velocity for use in acc calculations by user code */
      Vec2d.copy(body.nonIntegralVelocity, body.velocity);
      Vec2d.scale(body.nonIntegralVelocity, 1.0 /. dt);
    },
    bodies,
  );
};

let integrateBodies = (bodies, dt, drag) => {
  Array.iteri((index, body) => Body.update(body, dt, index), bodies);
  integrateMotion(bodies, dt, drag);
};

let step = (physics: physicsState, bodies: array(Body.body)) => {
  /* Initialise the clock on first step. */
  if (physics.clock == 0.0) {
    physics.clock = now();
  };
  /* Compute delta time since last step. */
  let time = now();
  /* fixed delta for debugging */
  let time =
    physics.dynamicTimestep
      ? time
      : physics.clock +. 16.667 *. physics.timeScale *. physics.simulationRate;
  let delta = time -. physics.clock;
  /* sufficient change. */
  if (delta > 0.0) {
    /* Convert time to seconds. */
    let delta = delta *. 0.001;
    /* Drag is inversely proportional to viscosity. */
    let drag = 1.0 -. physics.viscosity;
    /* Update the clock. */
    physics.clock = time;
    /* Increment time accumulatedTime.
       Don't accumulate any additional time if we're already more than 1 second behind.
       This happens when the tab is backgrounded, and if this grows large enough we
       won't be able to ever catch up.
       */
    if (physics.accumulatedTime < 1.0) {
      physics.accumulatedTime = physics.accumulatedTime +. delta;
    } else {
      Js.log("accumulated too much time, not accumulating any more");
    };
    /* Integrate until the accumulatedTime is empty or until the */
    /* maximum amount of iterations per step is reached. */
    let i = ref(0);
    let timestep = timestep *. physics.timeScale;
    while (physics.accumulatedTime >= timestep && i^ < maxSteps) {
      /* Integrate bodies by fixed timestep. */
      integrateBodies(bodies, timestep, drag);
      /* Reduce accumulatedTime by one timestep. */
      physics.accumulatedTime = physics.accumulatedTime -. timestep;
      i := i^ + 1;
    };
  };
};