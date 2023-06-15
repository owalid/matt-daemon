from flask import Blueprint, request
from utils.mixins import create_response
from utils.ProcessManager import ProcessManager

mod = Blueprint('main_routes', __name__, url_prefix='/api')

@mod.route('/post_input', methods=[ 'POST' ])
def process_content():
    data = request.get_json()
    if data:
        content = data.get('content')
    proc_manager = ProcessManager.getInstance()

    ret = proc_manager.send_to_daemon(content)
    if ret:
        return create_response(data={ 'status': 'success' })

    return create_response(data={ 'status': 'error' }, status=500)
