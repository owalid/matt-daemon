import argparse as ap
from argparse import RawTextHelpFormatter
import socket
from Crypto.Cipher import AES
from Crypto.Util.Padding import pad


def encrypt(key, raw):
    raw = pad(raw.encode(),16)
    cipher = AES.new(key.encode('utf-8'), AES.MODE_ECB)
    return cipher.encrypt(raw).hex()

if __name__ == "__main__":
    parser = ap.ArgumentParser(formatter_class=RawTextHelpFormatter)
    parser.add_argument("-k", "--key_path", required=True, type=str, help='Path to the key file')
    parser.add_argument("-d", "--data", required=True, type=str, help='Data to encrypt and send')
    parser.add_argument("-host", "--host", required=False, default="127.0.0.1", type=str, help='Host to connect to')
    parser.add_argument("-port", "--port", required=False, default="4242", type=str, help='Port to connect to')

    args = parser.parse_args()
    key_path = args.key_path
    data = args.data
    host = args.host
    port = args.port

    try:
        with open(key_path, 'r') as f:
            key = f.read()
            key = key[:-1]
    except:
        print('Key file not found')
        exit()
    if len(key) > 16:
        print('Data too long')
        exit()
    
    if len(key) != 16:
        print('Key length must be 16')
        exit()
    
    encrypted = encrypt(key, data)
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((host, int(port)))
    s.send(encrypted.encode('utf-8') + b'\n')
