let component = ReasonReact.statelessComponent("Link");
let make = (~route, _children) => {
  ...component,
  render: _self => {
    let href = Route.toString(route);
    let onClick = e => {
      ReactEvent.Mouse.preventDefault(e);
      ReasonReact.Router.push(href);
    };
    <a href onClick> ..._children </a>;
  },
};
