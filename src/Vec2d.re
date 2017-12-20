[@bs.val] [@bs.scope "Math"]
external sqrt : (float) => float = "sqrt";


type vec2d = {
  mutable x: float,
  mutable y: float
};

let origin = () : vec2d => {x: 0.0, y: 0.0};

let create = (x, y) : vec2d => {x, y};

let copy = (target, source) => {
  target.x = source.x;
  target.y = source.y
};

let scale = (vector, factor) => {
  vector.x = vector.x *. factor;
  vector.y = vector.y *. factor
};

let add = (v1, v2) => {
  v1.x = v1.x +. v2.x;
  v1.y = v1.y +. v2.y;
};

let sub = (v1, v2) => {
  v1.x = v1.x -. v2.x;
  v1.y = v1.y -. v2.y;
};

let clear = (vector) => {
  vector.x = 0.0;
  vector.y = 0.0
};

let magSq = (vector) =>
  vector.x *. vector.x +. vector.y *. vector.y;

let norm = (vector) => {
  let m = sqrt(vector.x *. vector.x +. vector.y *. vector.y);
  vector.x = vector.x /. m;
  vector.y = vector.y /. m;
};