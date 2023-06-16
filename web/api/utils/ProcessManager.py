import socket
import os
from Crypto.Cipher import AES
from Crypto.Util.Padding import pad

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
            self.key = self.validate_key(os.environ.get("KEY_PATH", None))
            self.secure = bool(int(os.environ.get("SECURE", "0"))) and self.key != None
            self.socket = None
    
    def validate_key(self, key_path):
        if key_path == None:
            return None
        
        try:
            with open(key_path, 'r') as f:
                key = f.read()
                key = key[:-1]
        except:
            print('Key file not found')
            return None
        if len(key) > 16:
            print('Data too long')
            return None
        
        if len(key) != 16:
            print('Key length must be 16')
            return None
        
        return key

    def connect(self):
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.socket.connect(DEFAULT_ADDRESS)

    def encrypt(self, raw):
        raw = pad(raw.encode(),16)
        cipher = AES.new(self.key.encode('utf-8'), AES.MODE_ECB)
        return cipher.encrypt(raw).hex().encode('utf-8') + b'\n'

    def send_to_daemon(self, message):
        if self.secure:
            message = self.encrypt(message)
        else:
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

