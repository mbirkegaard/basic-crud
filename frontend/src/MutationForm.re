type state = {text: string};

type action =
  | UpdateText(string);

let component = ReasonReact.reducerComponent("MutationForm");

let make =
    (
      ~fireMutation: string => Js.Promise.t('a),
      ~result: ReasonApolloTypes.mutationResponse('b),
      ~placeholderText,
      ~buttonText,
      _children,
    ) => {
  ...component,
  initialState: () => {text: ""},
  reducer: (action, _state) =>
    switch (action) {
    | UpdateText(text) => ReasonReact.Update({text: text})
    },
  render: self => {
    let handleMutation = () =>
      fireMutation(self.state.text)
      |> Js.Promise.then_(
           (response: ReasonApolloTypes.executionResponse('b)) => {
           switch (response) {
           | Data(_) => UpdateText("") |> self.send
           | Errors(err) => Js.log2("Error", err)
           | _ => ()
           };
           Js.Promise.resolve(response);
         })
      |> ignore;

    <div>
      <input
        type_="text"
        placeholder=placeholderText
        value={self.state.text}
        onChange={
          evt => ReactEvent.Form.target(evt)##value->UpdateText |> self.send
        }
        onKeyDown={
          evt =>
            ReactEvent.Keyboard.key(evt) == "Enter" ? handleMutation() : ()
        }
      />
      <button
        disabled={self.state.text == ""} onClick={_e => handleMutation()}>
        {ReasonReact.string(buttonText)}
      </button>
      {
        switch (result) {
        | Error(e) => e##message |> ReasonReact.string
        | _ => ReasonReact.null
        }
      }
    </div>;
  },
};
