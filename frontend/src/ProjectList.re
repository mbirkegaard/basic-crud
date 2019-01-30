module DeleteProjectButton = {
  let component = ReasonReact.statelessComponent("DeleteProjectButton");
  let make = (~id, _children) => {
    ...component,
    render: _self => {
      <Queries.DeleteProjectMutation>
      ...((mutate, _) => {
          let mutation = Queries.DeleteProject.make(~id=id, ());
          <MutationButton mutate=(mutate(~variables=mutation##variables, ~refetchQueries=[|"getAllProjects"|])) >
            (ReasonReact.string("Delete"))
          </MutationButton>
      })
      </Queries.DeleteProjectMutation>
    }
  };
};

module ProjectItem = {
  let component = ReasonReact.statelessComponent("ProjectItem");
  
  let make = (~project, _children) => {
    ...component,
    render: _self =>
    <div>
      <Link route=Route.Projects(project##id) >
        <span>(ReasonReact.string(project##name))</span>
      </Link>
      <div>
        <DeleteProjectButton id=project##id />
      </div>
    </div>
  }
};

module ProjectsList = {
  let str = ReasonReact.string;
  
  let component = ReasonReact.statelessComponent("ProjectsList");
  
  let make = (_children) => {
    ...component,
    render: _self => {
      <div>
        <Queries.GetAllProjectsQuery>
        ...(({result}) => {
          switch result {
            | Loading => <div> (ReasonReact.string("Loading")) </div>
            | Error(error) => <div> (ReasonReact.string(error##message)) </div>
            | Data(response) => <div> ...(Array.map(project => <div> <ProjectItem project/> <hr/> </div>, response##projects)) </div>
          }
        })
        </Queries.GetAllProjectsQuery>
      </div>
    }
  };
};

module NewProjectForm = {
  let component = ReasonReact.statelessComponent("NewProject");
  
  let make = (_children) => {
    ...component,
    render: _self => {
      <Queries.AddProjectMutation>
      ...(
      (mutate, { result }) => {
        let fireMutation = (text) => {
          let mutation = Queries.AddProject.make(~name=text, ());
          mutate(~variables=mutation##variables, ~refetchQueries=[|"getAllProjects"|], ());
        };

        <MutationForm fireMutation result placeholderText="Project name" buttonText="Add Project"/>
      })
      </Queries.AddProjectMutation>
    }
  }
};

let component = ReasonReact.statelessComponent("Projects");

let make = (_children) => {
  ...component,
  render: _self => {
    <div>
      <ProjectsList />
      <NewProjectForm />
    </div>
  },
};
