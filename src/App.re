type repository = {
  name: string,
  description: string,
  href: string,
};

module Api = {
  open Json.Decode;

  let decodeResults =
    field(
      "items",
      list(
        optional(json =>
          {
            name: field("name", string, json),
            description: field("description", string, json),
            href: field("html_url", string, json),
          }
        ),
      ),
    );

  let getResults = query =>
    /* This is a local "open", it makes the Js.Promise module available inside of the parentheses */
    Js.Promise.(
      Fetch.fetch("https://api.github.com/search/repositories?q=" ++ query)
      |> then_(Fetch.Response.json)
      |> then_(json => decodeResults(json) |> resolve)
      |> then_(results =>
           results
           |> List.filter(optionalItem =>
                switch (optionalItem) {
                | Some(_) => true
                | None => false
                }
              )
           /* Turn our items out of option types into a regular record */
           |> List.map(item =>
                switch (item) {
                | Some(item) => item
                }
              )
           |> resolve
         )
    );
};

type state = {
  input: string,
  isLoading: bool,
  results: list(repository),
};

type action =
  | UpdateInput(string)
  | UpdateResults(list(repository))
  | Search;

let component = ReasonReact.reducerComponent("App");

let make = _children => {
  ...component,
  initialState: () => {input: "", isLoading: false, results: []},
  reducer: (action, state) =>
    switch (action) {
    | UpdateInput(newInput) =>
      ReasonReact.Update({...state, input: newInput})
    | UpdateResults(results) =>
      ReasonReact.Update({...state, isLoading: false, results})
    | Search =>
      ReasonReact.UpdateWithSideEffects(
        {...state, isLoading: true},
        self => {
          let value = self.state.input;
          let _ =
            Api.getResults(value)
            |> Js.Promise.then_(results => {
                 self.send(UpdateResults(results));
                 Js.Promise.resolve();
               });
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
          ~width="32rem",
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
      <div>
        {!self.state.isLoading
           ? self.state.results
             /* Convert to list to an array for ReasonReact's type bindings */
             |> Array.of_list
             /* Map each array item to a <Card /> component */
             |> Array.map(({name, href, description}) =>
                  <Card key=href name href description />
                )
             /* Transform the array into a valid React node, similar to ReasonReact.string */
             |> ReasonReact.array
           : ReasonReact.null}
      </div>
    </div>,
};