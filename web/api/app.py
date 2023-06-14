from flask import Flask
from dotenv import load_dotenv, find_dotenv
from flask_cors import CORS

load_dotenv(find_dotenv())

def create_app(test_config=None):
    """
    The flask application factory. To run the app somewhere else you can:
    """
    app = Flask(__name__)

    CORS(app, origins=['http://127.0.0.1:3000', 'http://localhost:3000'])  # add CORS
    
    # import and register blueprints
    from routes import main_bp

    app.register_blueprint(main_bp.mod)
    return app


# sets up the app
app = create_app()

if __name__ == '__main__':
    app.run(host='0.0.0.0', debug=True)
