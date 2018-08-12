[@bs.val] [@bs.scope "window"] external innerWidth : float = "innerWidth";

[@bs.val] [@bs.scope "window"] external innerHeight : float = "innerHeight";

[@bs.module "./mousepos"] external getMouseX : unit => float = "getMouseX";

[@bs.module "./mousepos"] external getMouseY : unit => float = "getMouseY";

[@bs.val] [@bs.scope "Math"] external random : unit => float = "random";

type state = {
  mutable attraction: bool,
  mutable click: bool,
  mousePos: Vec2d.vec2d,
  windowOrigin: Vec2d.vec2d,
  windowBounds: Vec2d.vec2d
};

let rec range = (i, j) =>
  if (i > j) {
    []
  } else {
    [i, ...range(i + 1, j)]
  };

let init_state = () : state => {
  attraction: true,
  click: false,
  mousePos: Vec2d.create(100.0, 300.0),
  windowOrigin: Vec2d.create(0.0, 0.0),
  windowBounds: Vec2d.create(0.0, 0.0)
};

let demo1 = () => {
  let demo = init_state();
  let attractionRadiusSquared = 10.0 *. 10.0;
  let attraction = (body: Body.body, _dt, index) => {
    let strength = 100.0 +. float_of_int(index + 1) *. 2.0;
    let delta = Vec2d.origin();
    /* Vector pointing from body to target. */
    Vec2d.copy(delta, demo.mousePos);
    Vec2d.sub(delta, body.position);
    /* Squared distance to target. */
    let distSq = Vec2d.magSq(delta);
    Vec2d.norm(delta);
    Vec2d.scale(delta, distSq /. attractionRadiusSquared);
    /* Apply force. */
    Vec2d.scale(delta, strength);
    Vec2d.add(body.acceleration, delta)
  };
  let maybeAttraction = (body, dt, index) =>
    if (demo.attraction) {
      attraction
        (body, dt, index)
        /* Js.log3("applied attraction ", body.acceleration.x, body.acceleration.y) */
    };
  /* let gravity = Behaviour.constantForce(~force=Vec2d.create(0.0, -980)); */
  let gravity = (body: Body.body, _dt, _index) =>
    Vec2d.add(body.acceleration, Vec2d.create(0.0, 980.0 *. body.mass));
  let maybeGravity = (body: Body.body, dt, index) =>
    if (! demo.attraction) {
      /* Js.log2("applying gravity to ", body); */
      gravity
        (body, dt, index)
        /* Js.log3("applied gravity ", body.acceleration.x, body.acceleration.y) */
    };
  /* let repulsion = (body: Body.body, dt, index) =>
     if ( demo.click) {
       demo.click = false;

       gravity(body, dt, index);
     }; */
  let edgeBounce = Behaviour.edgeBounce(~min=demo.windowOrigin, ~max=demo.windowBounds);
  let bodies: array(Body.body) =
    Array.of_list(
      List.map(
        (i) =>
          Body.create(
            ~id=i,
            ~mass=10.0,
            ~radius=20.0,
            ~position=Vec2d.create(random() *. 600.0, random() *. 600.0),
            ~behaviours=[maybeAttraction, maybeGravity, edgeBounce]
          ),
        range(0, 10)
      )
    );
  let collision = Behaviour.collision(bodies);
  Array.iter((body: Body.body) => body.behaviours = [collision, ...body.behaviours], bodies);
  (demo, bodies)
};

let step = (demo) => {
  demo.mousePos.x = getMouseX();
  demo.mousePos.y = getMouseY();
  demo.windowBounds.x = innerWidth;
  demo.windowBounds.y = innerHeight
};