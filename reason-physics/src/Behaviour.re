let attraction = (~target: Vec2d.vec2d, ~radius: float, ~strength: float) => {
  let radiusSquared = radius *. radius;
  (body: Body.body, _dt, _index) => {
    let delta = Vec2d.origin();
    /* Vector pointing from body to target. */
    Vec2d.copy(delta, target);
    Vec2d.sub(delta, body.position);
    /* Squared distance to target. */
    let distSq = Vec2d.magSq(delta);
    Vec2d.norm(delta);
    Vec2d.scale(delta, distSq /. radiusSquared);
    /* Apply force. */
    Vec2d.scale(delta, strength);
    Vec2d.add(body.acceleration, delta);
  };
};

let edgeBounce =
    (~min: Vec2d.vec2d, ~max: Vec2d.vec2d, body: Body.body, _dt, _index) => {
  /* failwith("die"); */
  let opposite = (-1.0) *. body.mass;
  if (body.position.x -. body.radius < min.x) {
    body.position.x = min.x +. body.radius;
    Vec2d.add(
      body.acceleration,
      Vec2d.create(body.nonIntegralVelocity.x *. opposite, 0.0),
    );
  } else if (body.position.x +. body.radius > max.x) {
    body.position.x = max.x -. body.radius;
    Vec2d.add(
      body.acceleration,
      Vec2d.create(body.nonIntegralVelocity.x *. opposite, 0.0),
    );
  };
  if (body.position.y -. body.radius < min.y) {
    body.position.y = min.y +. body.radius;
    Vec2d.add(
      body.acceleration,
      Vec2d.create(0.0, body.nonIntegralVelocity.y *. opposite),
    );
  } else if (body.position.y +. body.radius > max.y) {
    body.position.y = max.y -. body.radius;
    Vec2d.add(
      body.acceleration,
      Vec2d.create(0.0, body.nonIntegralVelocity.y *. opposite),
    );
  };
};

let constantForce =
    (~force: Vec2d.vec2d, body: Body.body, _dt: float, _index: int) =>
  Vec2d.add(body.acceleration, force);

let collisionSeparationOffset =
    (pos: Vec2d.vec2d, overlap: float, separationForce: float) => {
  Vec2d.norm(pos);
  Vec2d.scale(pos, overlap *. separationForce);
  pos;
};

let collisionRestitutionForce =
    (pos: Vec2d.vec2d, restitution: float, separationForce: float) => {
  Vec2d.norm(pos);
  Vec2d.scale(pos, restitution *. separationForce);
  pos;
};

let collision = (pool: array(Body.body)) => {
  let delta = Vec2d.origin();
  let direction = Vec2d.origin();
  (body: Body.body, _dt, _index) => {
    let length = Array.length(pool);
    let j = ref(0);
    while (j^ < length) {
      let otherBody = pool[j^];
      j := j^ + 1;
      if (body != otherBody) {
        Vec2d.copy(delta, otherBody.position);
        Vec2d.sub(delta, body.position);
        let distanceSquared = Vec2d.magSq(delta);
        Vec2d.copy(direction, delta);
        Vec2d.norm(direction);
        let radii = body.radius +. otherBody.radius;
        if (distanceSquared <= radii *. radii) {
          let distance = sqrt(distanceSquared);
          let overlap = radii -. distance -. 0.5;
          /* Total mass. */
          let mt = body.mass +. otherBody.mass;
          /* Distribute collision responses. */
          let bodySeparationForce = body.mass /. mt;
          let otherBodySeparationForce = otherBody.mass /. mt;
          /* Move particles so they no longer overlap.*/
          Vec2d.add(
            body.position,
            collisionSeparationOffset(
              Vec2d.clone(delta),
              overlap,
              -. bodySeparationForce,
            ),
          );
          Vec2d.add(
            otherBody.position,
            collisionSeparationOffset(
              Vec2d.clone(delta),
              overlap,
              otherBodySeparationForce,
            ),
          );
        };
      };
    };
    /* apply normal forces */
    /* Vec2d.add(body.acceleration, ) */
    /*
     let restitutionToUse = if (body.restitution > otherBody.restitution) {
      body.restitution;
     } else {
      otherBody.restitution;
     };


     let velMag = Vec2d.mag(body.velocity);


     Vec2d.scale(body.velocity, restitutionToUse * velMag);
     */
    /*
     Vec2d.add(
       body.velocity,
       collisionRestitutionForce(Vec2d.clone(delta), restitutionToUse, -. bodySeparationForce)
     );
      Vec2d.add(
        otherBody.velocity,
        collisionRestitutionForce(Vec2d.clone(delta), restitutionToUse, otherBodySeparationForce)
      );
      */
  };
};