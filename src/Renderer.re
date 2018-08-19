[@bs.val] [@bs.scope "window"] external requestAnimationFrame : (unit => unit) => unit =
  "requestAnimationFrame";

let pad = (s, length: int) => " " ++ s ++ String.make(length - String.length(s) + 1, ' ');

let formatVec = (vector: Vec2d.vec2d, padding: int) =>
  "x="
  ++ pad((vector.x >= 0.0 ? " " : "") ++ string_of_float(vector.x), padding)
  ++ " "
  ++ "y="
  ++ pad((vector.y >= 0.0 ? " " : "") ++ string_of_float(vector.y), padding);

let renderBodiesDebugText = (bodies: array(Body.body)) =>
  <pre>
    (
      ReasonReact.array(
        Array.mapi(
          (i, body: Body.body) =>
            ReasonReact.string(
              Printf.sprintf(
                "%spos={%s} vel={%s} acc={%s} \n",
                pad(string_of_int(i) ++ ":", 4),
                formatVec(body.position, 20),
                formatVec(body.velocity, 20),
                formatVec(body.prevAcceleration, 20)
              )
            ),
          bodies
        )
      )
    )
  </pre>;

let drawBodies = (bodies: array(Body.body)) =>
  <div style=(ReactDOMRe.Style.make(~position="absolute", ()))>
    (
      ReasonReact.array(
        Array.mapi(
          (i, body: Body.body) =>
            <div
              key=(string_of_int(i))
              style=(
                ReactDOMRe.Style.make(
                  ~position="absolute",
                  ~transform=
                    "translate("
                    ++ string_of_float(body.position.x -. body.radius)
                    ++ "px"
                    ++ ", "
                    ++ string_of_float(body.position.y -. body.radius)
                    ++ "px"
                    ++ ")",
                  ()
                )
              )>
              <div
                className="ball"
                style=(
                  ReactDOMRe.Style.make(
                    ~width=string_of_float(2.0 *. body.radius) ++ "px",
                    ~height=string_of_float(2.0 *. body.radius) ++ "px",
                    ()
                  )
                )>
                (ReasonReact.string(string_of_int(i)))
              </div>
            </div>,
          bodies
        )
      )
    )
  </div>;

let renderLoop = (render) => {
  let rec asyncLoop = () => {
    render();
    requestAnimationFrame(asyncLoop)
  };
  asyncLoop()
};