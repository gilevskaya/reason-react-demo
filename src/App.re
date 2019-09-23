type state = {
  input: string,
  isLoading: bool,
};

type action =
  | UpdateInput(string)
  | Search;

let component = ReasonReact.reducerComponent("App");

let make = _children => {
  ...component,
  initialState: () => {input: "", isLoading: false},
  reducer: (action, state) =>
    switch (action) {
    | UpdateInput(newInput) =>
      ReasonReact.Update({...state, input: newInput})
    | Search =>
      ReasonReact.UpdateWithSideEffects(
        {...state, isLoading: true},
        self => {
          let value = self.state.input;
          Js.log("value: " ++ value);
          /* This function needs to return a "unit" type, wo we'll insert it here */
          ();
        },
      )
    },
  render: self =>
    <div
      style={ReactDOMRe.Style.make(
        ~display="flex",
        ~flexDirection="column",
        ~alignItems="center",
        ~fontFamily="sans-serif",
        (),
      )}>
      <div
        style={ReactDOMRe.Style.make(
          ~marginTop="1rem",
          ~marginBottom="1rem",
          ~minWidth="32rem",
          ~display="flex",
          ~justifyContent="space-between",
          (),
        )}>
        <form
          onSubmit={ev => {
            ReactEvent.Form.preventDefault(ev);
            self.send(Search);
          }}>
          <label htmlFor="search"> {ReasonReact.string("Search")} </label>
          <input
            id="search"
            name="search"
            value={self.state.input}
            onChange={ev => {
              let value = ReactEvent.Form.target(ev)##value;
              self.send(UpdateInput(value));
            }}
          />
          <button type_="submit">
            {ReasonReact.string("Submit Search")}
          </button>
        </form>
        <div>
          {self.state.isLoading
             ? ReasonReact.string("Loading...") : ReasonReact.null}
        </div>
      </div>
      <Card name="reason" href="" description="This is cool" />
      <Card name="reason" href="" description="And this" />
    </div>,
};