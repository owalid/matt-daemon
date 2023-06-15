import socket
import os

DEFAULT_ADDRESS = (os.environ.get("FLASK_ENV", "127.0.0.1"), 4242)
class ProcessManager:
    # singleton
    __instance = None

    @staticmethod
    def getInstance():
        """ Static access method. """
        if ProcessManager.__instance == None:
            ProcessManager()
        return ProcessManager.__instance
    
    def __init__(self):
        """ Virtually private constructor. """
        if ProcessManager.__instance != None:
            raise Exception("This class is a singleton!")
        else:
            ProcessManager.__instance = self
            self.processes = {}
            self.socket = None
    
    def connect(self):
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.socket.connect(DEFAULT_ADDRESS)

    def send_to_daemon(self, message):
        message = message.encode('utf-8') + b'\n'
        try:
            if self.socket:
                try:
                    self.socket.send(message)
                # try to reconnect if the socket is closed
                except BrokenPipeError:
                    self.connect()
                    self.socket.send(message)
            else:
                self.connect()
                self.socket.send(message)
            return True
        except Exception as e:
            print(e)
            self.socket = None
            return False

