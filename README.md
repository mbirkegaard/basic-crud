This is a wonderfully brutalist todo app that I put together as research. On the backend it uses Python, Flask, SQLAlchemy, and GraphQL via Graphene and on the frontend it's ReasonML, React, and GraphQL via ApolloClient.

First you need a Auth0 account. Create an API, call it whatever you like, and come up with an API audience for it, e.g `https://todoapi.thiscanprettymuchbeaything.com`. Create an single page application and add `http://localhost:300/callback` to the allowed callback URLs. 

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

To run the frontend, do `npm install` or `yarn` in `/frontend`. 

`./node_modules/.bin/send-introspection-query http://localhost:5000/api`
