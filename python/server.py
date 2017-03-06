import socketserver
import os
from urllib.parse import urlparse
from subprocess import Popen, PIPE
from http.server import BaseHTTPRequestHandler
import random
import socket
import sys

class MyHandler(BaseHTTPRequestHandler):
    def do_GET(self):
        parsed = urlparse(self.path)
        self.send_response(200)
        self.send_header("Content-Type", "text/html")
        self.end_headers()
        self.wfile.write(bytearray("done",'utf8'))
        args = dict(query.split("=") for query in parsed.query.split("&"))
        print(self.path)

        handleCommand(args)


def handleCommand(args):
    print(args)
    if "streamName" in args:
        sName = args["streamName"]
        sock.send(sName.encode())

# Create a TCP/IP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
# Connect the socket to the port where the server is listening
# Change server address and IP to be the correct versions
server_address = ('10.42.42.114', 3181)
print('connecting to %s port %s' % server_address,file=sys.stderr)
sock.connect(server_address)

port = 1234
httpd = socketserver.TCPServer(("", port), MyHandler)
httpd.serve_forever()
