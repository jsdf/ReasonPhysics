[@bs.val] [@bs.scope "window"] external innerWidth : float = "innerWidth";

[@bs.val] [@bs.scope "window"] external innerHeight : float = "innerHeight";

[@bs.module "./mousepos"] external getMouseX : unit => float = "getMouseX";

[@bs.module "./mousepos"] external getMouseY : unit => float = "getMouseY";

[@bs.val] [@bs.scope "Math"] external random : unit => float = "random";

type state = {
  mutable attraction: bool,
  mutable debug: bool,
  mousePos: Vec2d.vec2d,
  windowOrigin: Vec2d.vec2d,
  windowBounds: Vec2d.vec2d
};

let updateInputs = (demoState) => {
  demoState.mousePos.x = getMouseX();
  demoState.mousePos.y = getMouseY();
  demoState.windowBounds.x = innerWidth;
  demoState.windowBounds.y = innerHeight
};

let startLoop = ((demoState, render, bodies, physics)) => {
  Renderer.renderLoop(() => {
    updateInputs(demoState);
    Physics.step(physics, bodies);
    render(bodies, demoState);
  });
}

let rec range = (i, j) =>
  if (i > j) {
    []
  } else {
    [i, ...range(i + 1, j)]
  };

let init_state = () : state => {
  attraction: true,
  debug: false,
  mousePos: Vec2d.create(100.0, 300.0),
  windowOrigin: Vec2d.create(0.0, 0.0),
  windowBounds: Vec2d.create(0.0, 0.0)
};

