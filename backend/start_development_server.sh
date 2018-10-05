#!/bin/sh

export FLASK_APP=./app.py
export FLASK_DEBUG=1
export FLASK_ENV=development
export APP_CONFIG_FILE=./dev_config.py
source $(pipenv --venv)/bin/activate
flask run -h 0.0.0.0
