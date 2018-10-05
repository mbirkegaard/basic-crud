type auth0 = {
  domain: string,
  clientID: string,
  audience: string
};

type config = {
  api: string,
  auth0: auth0
};

let activeConfig: config;