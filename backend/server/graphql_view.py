from flask import current_app
from flask_graphql import GraphQLView
from server.auth import AuthError, verify_token


class View(GraphQLView):
    def get_context(self):
        try:
            auth = verify_token()
            return {
                'auth': auth
            }
        except AuthError as exception:
            if current_app.config['ALLOW_NO_AUTH_SESSION']:
                return {
                    'auth': None
                }
            else:
                raise exception
