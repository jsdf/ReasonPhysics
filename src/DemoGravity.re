[@bs.val] [@bs.scope "Math"] external random: unit => float = "random";

[@bs.module "./mousepos"] external hasTouch: unit => bool = "hasTouch";

let nBodies = 10;

let formatFloatForInput = f => Printf.sprintf("%.2f", f);

let updateDynamicTimestepFlag = (physics: Physics.physicsState) =>
  physics.dynamicTimestep =
    physics.timeScale == 1.0 && physics.simulationRate == 1.0;

/* ui for messing with demo parameters */
let renderControls =
    (
      bodies: array(Body.body),
      demoState: Demo.state,
      physics: Physics.physicsState,
    ) =>
  <div className="demo-controls">
    <div>
      <button onClick={_event => demoState.debug = !demoState.debug}>
        {
          ReasonReact.string(
            "debug bodies: " ++ (demoState.debug ? "on" : "off"),
          )
        }
      </button>
    </div>
    <div>
      <button onClick={_event => demoState.attraction = !demoState.attraction}>
        {
          ReasonReact.string(
            "toggle mode: ["
            ++ (
              demoState.attraction ? "attract to mouse" : "vertical gravity"
            )
            ++ "]",
          )
        }
      </button>
    </div>
    <div>
      <label className="demo-controllabel">
        {ReasonReact.string("viscosity: ")}
        {
          ReactDOMRe.createElement(
            "input",
            ~props=
              ReactDOMRe.objToDOMProps({
                "type": "range",
                "min": "0",
                "max": "0.3",
                "step": "0.01",
                "value": formatFloatForInput(physics.viscosity),
                "onChange": event =>
                  physics.viscosity =
                    float_of_string(event->ReactEvent.Form.target##value),
              }),
            [||],
          )
        }
        {
          ReactDOMRe.createElement(
            "input",
            ~props=
              ReactDOMRe.objToDOMProps({
                "type": "text",
                "readOnly": true,
                "value": formatFloatForInput(physics.viscosity),
              }),
            [||],
          )
        }
      </label>
    </div>
    <div>
      <label className="demo-controllabel">
        {ReasonReact.string("timescale: ")}
        {
          ReactDOMRe.createElement(
            "input",
            ~props=
              ReactDOMRe.objToDOMProps({
                "type": "range",
                "min": "0",
                "max": "2.0",
                "step": "0.1",
                "value": formatFloatForInput(physics.timeScale),
                "onChange": event => {
                  physics.timeScale =
                    float_of_string(event->ReactEvent.Form.target##value);
                  updateDynamicTimestepFlag(physics);
                  physics.accumulatedTime = 0.0;
                },
              }),
            [||],
          )
        }
        {
          ReactDOMRe.createElement(
            "input",
            ~props=
              ReactDOMRe.objToDOMProps({
                "type": "text",
                "readOnly": true,
                "value": formatFloatForInput(physics.timeScale),
              }),
            [||],
          )
        }
      </label>
    </div>
    <div>
      <label className="demo-controllabel">
        {ReasonReact.string("simulation rate: ")}
        {
          ReactDOMRe.createElement(
            "input",
            ~props=
              ReactDOMRe.objToDOMProps({
                "type": "range",
                "min": "0",
                "max": "2.0",
                "step": "0.1",
                "value": formatFloatForInput(physics.simulationRate),
                "onChange": event => {
                  physics.simulationRate =
                    float_of_string(event->ReactEvent.Form.target##value);
                  updateDynamicTimestepFlag(physics);
                  physics.accumulatedTime = 0.0;
                },
              }),
            [||],
          )
        }
        {
          ReactDOMRe.createElement(
            "input",
            ~props=
              ReactDOMRe.objToDOMProps({
                "type": "text",
                "readOnly": true,
                "value": formatFloatForInput(physics.simulationRate),
              }),
            [||],
          )
        }
      </label>
      <div>
        {
          ReasonReact.string(
            "using dynamic timestep: "
            ++ (physics.dynamicTimestep ? "yes" : "no"),
          )
        }
      </div>
    </div>
    {
      demoState.debug ?
        Renderer.renderBodiesDebugText(bodies) : ReasonReact.null
    }
  </div>;

let start = () => {
  let demoState = Demo.init_state();
  /* set up the physics behaviours particular to the demo */
  let attractionRadiusSquared = 10.0 *. 10.0;
  let attraction = (body: Body.body, _dt, index) => {
    let strength = 1.0 +. float_of_int(index + 1) *. 0.1;
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
    Vec2d.add(body.acceleration, delta);
  };
  let maybeAttraction = (body, dt, index) =>
    if (demoState.attraction) {
      attraction(body, dt, index);
    };
  let gravity = (body: Body.body, _dt, _index) =>
    Vec2d.add(body.acceleration, Vec2d.create(0.0, 98.0 *. body.mass));
  let maybeGravity = (body: Body.body, dt, index) =>
    if (!demoState.attraction) {
      gravity(body, dt, index);
    };
  let edgeBounce =
    Behaviour.edgeBounce(
      ~min=demoState.windowOrigin,
      ~max=demoState.windowBounds,
    );
  let bodies: array(Body.body) =
    Array.of_list(
      List.map(
        i =>
          Body.create(
            ~id=i,
            ~mass=10.0,
            ~radius=20.0,
            ~position=Vec2d.create(random() *. 600.0, random() *. 600.0),
            ~behaviours=[maybeAttraction, maybeGravity, edgeBounce],
          ),
        Demo.range(0, nBodies - 1),
      ),
    );
  let collision = Behaviour.collision(bodies);
  Array.iter(
    (body: Body.body) => body.behaviours = [collision, ...body.behaviours],
    bodies,
  );
  let render =
      (
        bodies: array(Body.body),
        demoState: Demo.state,
        physics: Physics.physicsState,
      ) =>
    ReactDOMRe.renderToElementWithId(
      <div>
        <div
          onMouseUp={
            _event =>
              if (!hasTouch()) {
                demoState.attraction = !demoState.attraction;
              }
          }
          className="demo">
          {Renderer.drawBodies(bodies)}
        </div>
        {renderControls(bodies, demoState, physics)}
        <div className="demo-info">
          <a href="https://github.com/jsdf/ReasonPhysics">
            {ReasonReact.string("source on github")}
          </a>
        </div>
      </div>,
      /* (Renderer.drawPositionDebug(bodies)) */
      "root",
    );
  let physics = Physics.create(~viscosity=0.02);
  Demo.startLoop(demoState, render, bodies, physics);
};