import SocketServer
import urlparse
import os
import urllib
from subprocess import Popen, PIPE
from BaseHTTPServer import BaseHTTPRequestHandler
import random
import socket
import sys

class MyHandler(BaseHTTPRequestHandler):
    def do_GET(self):
        parsed = urlparse.urlparse(self.path)
        self.send_response(200)
        self.send_header("Content-Type", "text/html")
        self.end_headers()
        self.wfile.write("done")
        args = urlparse.parse_qs(parsed.query)
        print(self.path)

        handleCommand(args)


def handleCommand(args):
    print(args)
    if "streamName" in args:
	sock.sendall(args["streamName"][0])

# Create a TCP/IP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
# Connect the socket to the port where the server is listening
# Change server address and IP to be the correct versions
server_address = ('192.168.1.1', 12345)
print >>sys.stderr, 'connecting to %s port %s' % server_address
sock.connect(server_address)

port = 12345
httpd = SocketServer.TCPServer(("", port), MyHandler)
httpd.serve_forever()
