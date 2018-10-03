module GetAllProjects = [%graphql {|
  query getAllProjects {
      projects {
        id
        name
        todos {
          id 
          name
          finished
        }
        finished
      }
    }
|}];
  
module GetAllProjectsQuery = ReasonApollo.CreateQuery(GetAllProjects);

module GetProject = [%graphql {|
  query getProject($id: ID!) {
    project(id: $id) {
      id
      name
      todos {
        id 
        name
        finished
      }
      finished
    }
  }
|}];
  
module GetProjectQuery = ReasonApollo.CreateQuery(GetProject);
  
module AddProject = [%graphql {|
  mutation createProject($name: String!) {
    addProject(name: $name) {
      ok
      project {
        id
        name
      }
    }
  }
|}];

module AddProjectMutation = ReasonApollo.CreateMutation(AddProject);

module DeleteProject = [%graphql {|
  mutation deleteProject($id: ID!) {
    deleteProject(id: $id) {
      ok
    }
  }
|}];

module DeleteProjectMutation = ReasonApollo.CreateMutation(DeleteProject);

module AddTodo = [%graphql {|
  mutation addTodo($projectId: ID!, $todoName: String!) {
    addTodo(projectId: $projectId, todoName: $todoName) {
      ok
    }
  }
|}];

module AddTodoMutation = ReasonApollo.CreateMutation(AddTodo);

module ToggleTodo = [%graphql {|
  mutation toggleTodo($id: ID!) {
    toggleTodo(id: $id) {
      ok
    }
  }
|}];

module ToggleTodoMutation = ReasonApollo.CreateMutation(ToggleTodo);

module DeleteTodo = [%graphql {|
  mutation deleteTodo($id: ID!) {
    deleteTodo(id: $id) {
      ok
    }
  }
|}];

module DeleteTodoMutation = ReasonApollo.CreateMutation(DeleteTodo);