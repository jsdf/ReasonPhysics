let url = ReasonReact.Router.dangerouslyGetInitialUrl();

switch url.hash {
| "1" => Demo1.start()
| _ => Demo1.start()
};