type action =
  | Authorize
  | LoadSessionFromStorage
  | HandleLoginCallback(ReasonReact.Router.url)
  | HandleSessionResult(Belt.Result.t(Auth0.session, string))
  | GetUserInfo(Auth0.session)
  | SetUser(State.user)
  | Logout
  | ChangeRoute(Route.t);

let string_of_action = action =>
  switch (action) {
  | Authorize => "Authorize"
  | LoadSessionFromStorage => "LoadSessionFromStorage"
  | HandleLoginCallback(_) => "HandleLoginCallback"
  | HandleSessionResult(_) => "HandleSessionResult"
  | GetUserInfo(_) => "GetUserInfo"
  | SetUser(_) => "SetUser"
  | Logout => "Logout"
  | ChangeRoute(route) => "ChangeRoute " ++ Route.toString(route)
  };
