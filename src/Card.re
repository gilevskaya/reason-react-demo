let component = ReasonReact.statelessComponent("Card");

let make = (~name, ~description, ~href, _children) => {
  ...component,
  render: _self =>
    <div
      style={ReactDOMRe.Style.make(
        ~border="1px solid #898989",
        ~borderRadius="4px",
        ~padding="1rem",
        ~margin="0.5rem",
        ~width="30rem",
        (),
      )}>
      <h3>
        <a href target="_blank" rel="noopener noreferrer">
          {ReasonReact.string(name)}
        </a>
      </h3>
      <p> {ReasonReact.string(description)} </p>
    </div>,
};