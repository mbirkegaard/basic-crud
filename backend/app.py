from flask import Flask
from flask_cors import CORS

app = Flask(__name__)
CORS(app)

app.config.from_envvar('APP_CONFIG_FILE')

from flask_graphql import GraphQLView
from server.schema import schema
from server.graphql_view import View
from server.models import init_db

init_db()

app.add_url_rule('/api', view_func=View.as_view('graphql',
                                                schema=schema, graphiql=True))
