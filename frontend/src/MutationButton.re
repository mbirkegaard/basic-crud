let component = ReasonReact.statelessComponent("MutationButton");

let make = (~mutate, children) => {
  ...component,
  render: _self =>
    <button onClick={_e => mutate() |> ignore}> ...children </button>,
};
