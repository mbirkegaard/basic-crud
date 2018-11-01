open State;
open Action;

let component = ReasonReact.reducerComponent("App");

let make = _children => {
  ...component,
  initialState: () => {route: Home, user: None},
  reducer: (action, state) =>
    switch (action) {
    | Authorize =>
      ReasonReact.SideEffects(
        (
          _ => {
            /* Store to send the user back to where they came from after authorizing */
            Dom.Storage.(
              setItem("redirectPath", Location.path, sessionStorage)
            );
            Auth0.client##authorize();
          }
        ),
      )
    | HandleLoginCallback(url) =>
      ReasonReact.SideEffects(
        (
          self =>
            Auth0.parseHash(url, parseResult =>
              HandleSessionResult(parseResult) |> self.send
            )
        ),
      )
    | LoadSessionFromStorage =>
      ReasonReact.SideEffects(
        (
          self =>
            switch (Auth0.getSession()) {
            | Some(session) => HandleSessionResult(Ok(session)) |> self.send
            | None => ()
            }
        ),
      )
    | HandleSessionResult(sessionResult) =>
      switch (sessionResult) {
      | Ok(session) =>
        ReasonReact.SideEffects(
          (
            self => {
              Auth0.setSession(session);
              GetUserInfo(session) |> self.send;
            }
          ),
        )
      | Error(err) =>
        ReasonReact.SideEffects(
          (_ => Js.log2("HandleSessionResult error: ", err)),
        )
      }
    | GetUserInfo(session) =>
      ReasonReact.SideEffects(
        (
          self =>
            Auth0.userInfo(session.accessToken, result =>
              switch (result) {
              | Ok(userInfoResult) =>
                SetUser({session, username: userInfoResult##name})
                |> self.send
              | Error(err) => Js.log2("GetUserInfo error:", err)
              }
            )
        ),
      )
    | SetUser(user) =>
      let redirectPath =
        switch (Dom.Storage.(getItem("redirectPath", sessionStorage))) {
        | Some(redirectPath) =>
          Dom.Storage.(removeItem("redirectPath", sessionStorage));
          redirectPath;
        | None => ""
        };
      ReasonReact.UpdateWithSideEffects(
        {...state, user: Some(user)},
        (_ => ReasonReact.Router.push(redirectPath)),
      );
    | Logout =>
      ReasonReact.UpdateWithSideEffects(
        {...state, user: None},
        (_ => Auth0.clearSession()),
      )
    | ChangeRoute(route) => ReasonReact.Update({...state, route})
    },
  didMount: self => {
    let watcherId =
      ReasonReact.Router.watchUrl(url => {
        let action =
          switch (url.path) {
          | ["callback"] => HandleLoginCallback(url)
          | _ => ChangeRoute(Route.fromUrl(url))
          };
        action |> self.send;
      });
    self.onUnmount(() => ReasonReact.Router.unwatchUrl(watcherId));

    let url = ReasonReact.Router.dangerouslyGetInitialUrl();
    if (url.path == ["callback"]) {
      HandleLoginCallback(url) |> self.send;
    } else {
      LoadSessionFromStorage |> self.send;
    };
  },
  render: self => {
    let {user, route} = self.state;
    <div>
      <NavBar user dispatch={self.send} />
      <hr />
      {
        switch (user) {
        | Some(_) =>
          switch (route) {
          | Home => <ProjectList />
          | Projects(id) => <ProjectView id />
          | NotFound => <div> {ReasonReact.string("404")} </div>
          }
        | _ => ReasonReact.null
        }
      }
    </div>;
  },
};

ReasonReact.Router.push("");
