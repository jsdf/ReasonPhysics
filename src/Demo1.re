[@bs.val] [@bs.scope "Math"] external random : unit => float = "random";

let start = () => {
  let demoState = Demo.init_state();
  let attractionRadiusSquared = 10.0 *. 10.0;
  let attraction = (body: Body.body, _dt, index) => {
    let strength = 100.0 +. float_of_int(index + 1) *. 2.0;
    let delta = Vec2d.origin();
    /* Vector pointing from body to target. */
    Vec2d.copy(delta, demoState.mousePos);
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
    if (demoState.attraction) {
      attraction(body, dt, index)
    };
  let gravity = (body: Body.body, _dt, _index) =>
    Vec2d.add(body.acceleration, Vec2d.create(0.0, 980.0 *. body.mass));
  let maybeGravity = (body: Body.body, dt, index) =>
    if (! demoState.attraction) {
      gravity(body, dt, index)
    };
  let edgeBounce = Behaviour.edgeBounce(~min=demoState.windowOrigin, ~max=demoState.windowBounds);
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
        Demo.range(0, 10)
      )
    );
  let collision = Behaviour.collision(bodies);
  Array.iter((body: Body.body) => body.behaviours = [collision, ...body.behaviours], bodies);
  let render = (bodies: array(Body.body), demoState: Demo.state) =>
    ReactDOMRe.renderToElementWithId(
      <div onClick=((_event) => demoState.attraction = ! demoState.attraction) className="demo">
        (Renderer.drawBodies(bodies))
        <div style=(ReactDOMRe.Style.make(~position="absolute", ()))>
          <button
            onClick=(
              (event) => {
                demoState.debug = ! demoState.debug;
                ReactEvent.Mouse.stopPropagation(event)
              }
            )>
            (ReasonReact.string("debug:" ++ (demoState.debug ? "on" : "off")))
          </button>
          <button
            onClick=(
              (event) => {
                demoState.attraction = ! demoState.attraction;
                ReactEvent.Mouse.stopPropagation(event)
              }
            )>
            (
              ReasonReact.string(
                "toggle mode: ["
                ++ (demoState.attraction ? "attract to mouse" : "vertical gravity")
                ++ "]"
              )
            )
          </button>
          (demoState.debug ? Renderer.renderBodiesDebugText(bodies) : ReasonReact.null)
        </div>
      </div>,
      "root"
    );
  Demo.startLoop((demoState, render, bodies))
};