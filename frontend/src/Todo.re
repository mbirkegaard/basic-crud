let module ToggleButton = {
  let component = ReasonReact.statelessComponent("ToggleButton");
  let make = (~id, _children) => {
    ...component,
    render: _self => {
      <Queries.ToggleTodoMutation>
      ...((mutate, _) => {
          let mutation = Queries.ToggleTodo.make(~id=id, ());
          <MutationButton mutate=(mutate(~variables=mutation##variables, ~refetchQueries=[|"getAllProjects"|])) >
            (ReasonReact.string("Toggle"))
          </MutationButton>
      })
      </Queries.ToggleTodoMutation>
    }
  }
};

let module DeleteButton = {
  let component = ReasonReact.statelessComponent("DeleteButton");
  let make = (~id, _children) => {
    ...component,
    render: _self => {
      <Queries.DeleteTodoMutation>
      ...((mutate, _) => {
          let mutation = Queries.DeleteTodo.make(~id=id, ());
          <MutationButton mutate=(mutate(~variables=mutation##variables, ~refetchQueries=[|"getAllProjects"|])) >
            (ReasonReact.string("Delete"))
          </MutationButton>
      })
      </Queries.DeleteTodoMutation>
    }
  }
};

let component = ReasonReact.statelessComponent(__MODULE__);
let make = (~todo, _children) => {
  ...component,
  render: _self => {
    open ReasonReact;
    <div>
      <div>(todo##name ++ " - " ++ ((todo##finished) ? "done" : "not done") |> string)</div>
      <ToggleButton id=todo##id/>
      <DeleteButton id=todo##id/>
    </div>
  }
};