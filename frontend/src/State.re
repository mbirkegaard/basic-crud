type user = {
  session: Auth0.session,
  username: string
};

type state = {
  user: option(user),
  route: Route.t,
};