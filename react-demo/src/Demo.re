[@bs.val] [@bs.scope "window"] external innerWidth: float = "innerWidth";

[@bs.val] [@bs.scope "window"] external innerHeight: float = "innerHeight";

[@bs.module "./mousepos"] external getMouseX: unit => float = "getMouseX";

[@bs.module "./mousepos"] external getMouseY: unit => float = "getMouseY";

[@bs.val] [@bs.scope "Math"] external random: unit => float = "random";

type state = {
  mutable attraction: bool,
  mutable debug: bool,
  mousePos: ReasonPhysics.Vec2d.vec2d,
  windowOrigin: ReasonPhysics.Vec2d.vec2d,
  windowBounds: ReasonPhysics.Vec2d.vec2d,
};

let updateInputs = demoState => {
  demoState.mousePos.x = getMouseX();
  demoState.mousePos.y = getMouseY();
  demoState.windowBounds.x = innerWidth;
  demoState.windowBounds.y = innerHeight;
};

let startLoop = (demoState, render, bodies, physics) => {
  Renderer.renderLoop(() => {
    updateInputs(demoState);
    ReasonPhysics.Physics.step(physics, bodies);
    render(bodies, demoState, physics);
  });
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