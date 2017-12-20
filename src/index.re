[%bs.raw {|require('./index.css')|}];

[@bs.val] [@bs.scope "window"]
external requestAnimationFrame : ((unit) => unit) => unit = "requestAnimationFrame";

[@bs.val] [@bs.scope "Math"]
external random : unit => float = "random";

[@bs.module "./mousepos"] external getMouseX : unit => float = "getMouseX";
[@bs.module "./mousepos"] external getMouseY : unit => float = "getMouseY";


let render = (bodies) =>
  ReactDOMRe.renderToElementWithId(
    <div
      style={
        ReactDOMRe.Style.make(~position="absolute", ())
      }
    >
      {ReasonReact.arrayToElement(
        Array.of_list(
          List.mapi((i, body: Body.body) =>
            <div
              key={string_of_int(i)}
              style={
                ReactDOMRe.Style.make(
                  ~position="absolute",
                  ~left=string_of_float(body.position.x -. body.radius /. 2.0) ++ "px",
                  ~top=string_of_float(body.position.y -. body.radius /. 2.0) ++ "px",
                  ~width=string_of_float(body.radius) ++ "px",
                  ~height=string_of_float(body.radius) ++ "px",
                  ~backgroundColor="red",
                  ()
                )
              }
            >
              {ReasonReact.stringToElement(string_of_int(i))}
            </div>,
            bodies
          )
        )
      )}
    </div>,
    "root"
  );

let rec range = (i, j) =>
  if (i > j) {
    []
  } else {
    [i, ...range(i + 1, j)]
  };

let attractionTarget = Vec2d.create(100.0, 300.0);

let attractionRadiusSquared = 10.0 *. 10.0;

let attraction = (body: Body.body, _dt, index) => {
  let strength = 100.0 *. float_of_int(index + 1);
  let delta = Vec2d.origin();

  /* Vector pointing from body to target. */
  Vec2d.copy(delta, attractionTarget);
  Vec2d.sub(delta, body.position);

  /* Squared distance to target. */
  let distSq = Vec2d.magSq(delta);

  Vec2d.norm(delta);
  Vec2d.scale(delta, distSq /. attractionRadiusSquared);

  /* Apply force. */
  Vec2d.scale(delta, strength);
  Vec2d.add(body.acceleration, delta);

};

let bodies =
  List.map(
    (_i) =>
      Body.create(
        10.0,
        50.0,
        Vec2d.create(
          random() *. 600.0,
          random() *. 600.0
        ),
        [attraction]
      ),
    range(0, 10)
  );

let physics = Physics.create();

let rec asyncLoop = () => {
  attractionTarget.x = getMouseX();
  attractionTarget.y = getMouseY();
  Physics.step(physics, bodies);
  render(bodies);
  requestAnimationFrame(asyncLoop);
};

asyncLoop();
