/* [@bs.val] [@bs.scope "Math"]
   external pi : float = "PI";

   open Webapi.Dom;

   let unwrap = (maybe) => {
     switch (maybe) {
     | Some(x) => x
     | None => failwith("failed to unwrap")
     }
   };

   type renderer = {
     width: int,
     height: int,
     renderParticles: bool,
     renderSprings: bool,
     renderMouse: bool,
     initialized: bool,
     renderTime: float,
     canvas: ElementRe.t,
     ctx: CanvasRe.Canvas2d.t,
   };


   let create = (id) => {

     let canvas = Document.createElement("canvas", document);
     let root = unwrap(Document.getElementById(id, document));
     Element.appendChild(canvas, root);
     let ctx = CanvasRe.CanvasElement.getContext2d(canvas);

     let renderer = {
       width: 800,
       height: 600,
       renderParticles: true,
       renderSprings: true,
       renderMouse: true,
       initialized: false,
       renderTime: 0.0,
       canvas,
       ctx,
     };
     ElementRe.setAttribute("width", string_of_int(renderer.width), canvas);
     ElementRe.setAttribute("height", string_of_int(renderer.height), canvas);

     renderer;
   };

   let twoPI = 2.0 *. pi;

   let step = (renderer: renderer, bodies: array(Body.body)) => {

     ElementRe.setAttribute("width", string_of_int(renderer.width), renderer.canvas);
     Array.iteri(
       (_i, body: Body.body) => {
         Canvas2dRe.beginPath(renderer.ctx);
         Canvas2dRe.arc(
           ~x=body.position.x,
           ~y=body.position.y,
           ~r=body.radius,
           ~startAngle=0.0,
           ~endAngle=twoPI,
           ~anticw=Js.Boolean.to_js_boolean(false),
           renderer.ctx
         );

         Canvas2dRe.setFillStyle(
           renderer.ctx,
           String,
           "#cecece"
         );

         Canvas2dRe.fill(renderer.ctx);

         /* @ctx.arc(p.pos.x, p.pos.y, p.radius, 0, twoPI, no) */
   /*
         @ctx.fillStyle = '#' + (p.colour or 'FFFFFF')
         @ctx.fill() */
       },
       bodies
     );
   }; */
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
                formatVec(body.acceleration, 20)
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

let render = (bodies: array(Body.body), demo: Demo.state) =>
  ReactDOMRe.renderToElementWithId(
    <div onClick=((_event) => demo.attraction = ! demo.attraction) className="demo">
      (drawBodies(bodies))
      <div style=(ReactDOMRe.Style.make(~position="absolute", ()))>
        <button
          onClick=(
            (event) => {
              demo.attraction = ! demo.attraction;
              ReactEvent.Mouse.stopPropagation(event)
            }
          )>
          (
            ReasonReact.string(
              "toggle mode: ["
              ++ (demo.attraction ? "attract to mouse" : "vertical gravity")
              ++ "]"
            )
          )
        </button>
      </div>
    </div>,
    /* (renderBodiesDebugText(bodies)) */
    "root"
  );