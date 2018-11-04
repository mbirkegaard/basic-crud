type auth0Client = {. "authorize": [@bs.meth] (unit => unit)};

type session = {
  idToken: string,
  accessToken: string,
  expiresIn: int,
};

[@bs.deriving abstract]
type clientOptions = {
  domain: string,
  clientID: string,
  audience: string,
  responseType: string,
  scope: string,
  redirectUri: string,
};

[@bs.module "auth0-js"] [@bs.new]
external createClient: clientOptions => auth0Client = "WebAuth";

type err = string;
type authResult = {
  .
  "accessToken": string,
  "idToken": string,
  "expiresIn": int,
};
type parseHashOption = {. "hash": string};
type userInfoResult = {
  .
  "sub": string,
  "nickname": string,
  "name": string,
  "picture": string,
  "updated_at": string,
  "email": string,
  "email_verified": string,
};

type jsCallback('a) = (Js.null(err), Js.null('a)) => unit;

[@bs.send]
external _parseHash:
  (auth0Client, parseHashOption, jsCallback(authResult)) => unit =
  "parseHash";
[@bs.scope "client"] [@bs.send]
external _userInfo: (auth0Client, string, jsCallback(userInfoResult)) => unit =
  "userInfo";

let redirectUri = () => {
  open Location;
  let portSuffix = port != "" ? {j|:$port|j} : "";
  {j|$protocol//$hostname$portSuffix/callback|j};
};

open Config;
let {domain, clientID, audience} = Config.activeConfig.auth0;

let clientOptions =
  clientOptions(
    ~domain,
    ~clientID,
    ~audience,
    ~responseType="token id_token",
    ~scope="openid email profile",
    ~redirectUri=redirectUri(),
  );

let client = createClient(clientOptions);

let getSession: unit => option(session) =
  () =>
    switch (
      Dom.Storage.(
        getItem("idToken", localStorage),
        getItem("accessToke", localStorage),
        getItem("expiresIn", localStorage),
      )
    ) {
    | (Some(idToken), Some(accessToken), Some(expiresIn)) =>
      Some({idToken, accessToken, expiresIn: int_of_string(expiresIn)})
    | (_, _, _) => None
    };

let setSession = session =>
  Dom.Storage.(
    setItem("idToken", session.idToken, localStorage),
    setItem("accessToke", session.accessToken, localStorage),
    setItem("expiresIn", string_of_int(session.expiresIn), localStorage),
  )
  |> ignore;

let clearSession = () =>
  Dom.Storage.(
    removeItem("idToken", localStorage),
    removeItem("accessToke", localStorage),
    removeItem("expiresIn", localStorage),
  )
  |> ignore;

open Belt;

let authResultToSession = authResult => {
  accessToken: authResult##accessToken,
  idToken: authResult##idToken,
  expiresIn: authResult##expiresIn,
};

let jscbToResultcb:
  (Result.t('a, string) => unit, Js.null(string), Js.null('a)) => unit =
  (callback, err, res) =>
    switch (Js.nullToOption(err), Js.nullToOption(res)) {
    | (Some(err), _) => callback(Error(err))
    | (_, Some(res)) => callback(Ok(res))
    | (None, None) =>
      callback(Error("Both err and res were null in callback"))
    };

let userInfo =
    (accessToken, callback: Result.t(userInfoResult, string) => unit) =>
  _userInfo(client, accessToken, jscbToResultcb(callback));

let parseHash =
    (
      url: ReasonReact.Router.url,
      callback: Result.t(session, string) => unit,
    ) =>
  _parseHash(
    client,
    {"hash": url.hash},
    jscbToResultcb(res => Result.map(res, authResultToSession) |> callback),
  );
