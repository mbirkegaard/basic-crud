type t =
  | Home
  | Projects(string)
  | NotFound;

let fromUrl = (url: ReasonReact.Router.url) =>
  switch (url.path) {
  | [] => Home
  | ["projects", id] => Projects(id)
  | _ => NotFound
  };

let toString = route =>
  switch (route) {
  | Home => "/"
  | Projects(id) => "projects/" ++ id
  | _ => ""
  };
