let url = ReasonReact.Router.dangerouslyGetInitialUrl();

switch url.hash {
| "1" => DemoGravity.start()
| _ => DemoGravity.start()
};