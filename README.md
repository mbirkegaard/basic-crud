This is an incredibly brutalist Todo app that I put together as research into a possible tech stack for a consulting project. The project didn't pan out, but what I did might be informative anyway so here it is!

On the backend it uses Python, Flask, SQLAlchemy, and GraphQL via Graphene and on the frontend it's ReasonML, React, and GraphQL via ApolloClient.

First you need an Auth0 account. Create an API, call it whatever you like, and come up with an API audience for it, e.g `https://todoapi.thiscanprettymuchbeaything.com`. Create an single page application and add `http://localhost:300/callback` to the allowed callback URLs.

In `/backend` add `dev_config.py` and give it the following entries
```
DB_URI = 'sqlite:///database.sqlite3'

AUTH0_DOMAIN = 'found in settings of the App you just created in Auth0',  
AUTH0_ALGORITHMS = ['RS256']
AUTH0_API_AUDIENCE = 'found in the settings of the API you just created in Auth0'
```

In `/frontend/src` add `Config.re`, and give it these entries
```
type auth0 = {
  domain: string,
  clientID: string,
  audience: string
};

type config = {
  api: string,
  auth0: auth0
};

let devConfig : config = {
  api: "http://localhost:5000/api",
  auth0: {
    domain: "same as AUTH0_DOMAIN above",
    clientID: "found in settings of the App you just created in Auth0",
    audience: "same as AUTH0_API_AUDIENCE above",
  } 
};

let activeConfig = devConfig;
```

To run the backend, you need [pipenv](https://pypi.org/project/pipenv/). Install it, navigate to `/backend` and run `pipenv --three` followed by `pipenv install`. To launch the Flask development server, run `./startDevelopmentServer.sh`. It serves a GraphQL-endpoint at `/api`. If you want to use GraphiQL without having to have a session, set `ALLOW_NO_AUTH_SESSION = True` in the config-file. 

To run the frontend, install dependencies by with `npm install` or `yarn` in `/frontend`. Before you can compile the Reason files to Javascript, you need to run the introspection query with `./node_modules/.bin/send-introspection-query http://localhost:5000/api` or (`yarn send-introspection-query http://localhost:5000/api`). This performs a query against the GraphQL-endpoint to get names and types of all fields, queries and mutations, saving them in `graphql_schema.json`. With this file present, this setup doesn't just give you typechecking of the Reason-code you write, but also of all your API-calls! 

To compile the frontend do `npm run reason:dev` (or `yarn reason:dev`) and run `npm run webpack:dev` (or `yarn webpack:dev`) to serve it at `http://localhost:3000`. When you click the login-button you will be redirected to the Auth0-login page, where, if you've used standard settings, you can log in with any gmail-address.
