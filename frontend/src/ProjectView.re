module AddTodoForm = {
  let component = ReasonReact.statelessComponent("AddTodoForm");

  let make = (~projectId, _children) => {
    ...component,
    render: _self =>
      <Queries.AddTodoMutation>
        ...{
             (mutate, {result}) => {
               let resultFromResponse = response =>
                 Queries.AddTodoMutation.convertJsInputToReason(response).
                   result;
               let fireMutation = text => {
                 let mutation =
                   Queries.AddTodo.make(~projectId, ~todoName=text, ());
                 mutate(
                   ~variables=mutation##variables,
                   ~refetchQueries=[|"getAllProjects"|],
                   (),
                 );
               };
               <MutationForm
                 fireMutation
                 result
                 resultFromResponse
                 placeholderText="Todo name"
                 buttonText="Add Todo"
               />;
             }
           }
      </Queries.AddTodoMutation>,
  };
};
let str = ReasonReact.string;
let component = ReasonReact.statelessComponent(__MODULE__);
let make = (~id, _chidren) => {
  ...component,
  render: _self => {
    let getProjectQuery = Queries.GetProject.make(~id, ());
    <div>
      <Queries.GetProjectQuery variables=getProjectQuery##variables>
        ...{
             ({result}) =>
               switch (result) {
               | Loading => <div> {ReasonReact.string("Loading...")} </div>
               | Error(error) =>
                 <div> {ReasonReact.string(error##message)} </div>
               | Data(response) =>
                 switch (response##project) {
                 | None =>
                   <div> {{j|Project with id=$id wasn't found|j} |> str} </div>
                 | Some(project) =>
                   let name = project##name;
                   let status = project##finished ? "done" : "not done";
                   <div>
                     <h3> {{j|$name - $status|j} |> str} </h3>
                     <ol>
                       ...{
                            Array.map(
                              todo => <li> <Todo todo /> </li>,
                              project##todos,
                            )
                          }
                     </ol>
                     <AddTodoForm projectId=project##id />
                   </div>;
                 }
               }
           }
      </Queries.GetProjectQuery>
      <Link route=Route.Home> {ReasonReact.string("Back")} </Link>
    </div>;
  },
};
