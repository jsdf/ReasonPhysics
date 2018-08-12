[%%debugger.chrome]

[@bs.val] [@bs.scope "window"] external requestAnimationFrame : (unit => unit) => unit =
  "requestAnimationFrame";

let start = ((demo, bodies)) => {
  let physics = Physics.create();

  let rec asyncLoop = () => {
    Demo.step(demo);
    Physics.step(physics, bodies);
    Renderer.render(bodies, demo);
    requestAnimationFrame(asyncLoop)
  };

  asyncLoop();
};

switch (ReasonReact.Router.dangerouslyGetInitialUrl().hash) {
| "1" => start(Demo.demo1())
| _ => start(Demo.demo1())
};

