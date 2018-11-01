let component = ReasonReact.statelessComponent("NavBar");

let make = (~user, ~dispatch, _children) => {
  ...component,
  render: _self =>
    <div
      style={
        ReactDOMRe.Style.make(~background="lightblue", ~padding="10px", ())
      }>
      {
        switch (user) {
        | Some(user) =>
          open State;
          let username = user.username;
          <div>
            {ReasonReact.string({j|Welcome $username|j})}
            <button onClick=(_evt => Action.Logout |> dispatch)>
              {ReasonReact.string("Logout")}
            </button>
          </div>;
        | _ =>
          <button onClick=(_evt => Action.Authorize |> dispatch)>
            {ReasonReact.string("Click to Login")}
          </button>
        }
      }
    </div>,
};
