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
    | Search => ReasonReact.Update({...state, isLoading: true})
    },
  render: self =>
    <div
      style={ReactDOMRe.Style.make(
        ~display="flex",
        ~flexDirection="column",
        ~alignItems="center",
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
        <form>
          <label htmlFor="search"> {ReasonReact.string("Search")} </label>
          <input id="search" name="search " value={self.state.input} />
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