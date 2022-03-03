import selectors
import socket
import port
import types
from response import Response
#from protocol import protocol

HOST = '127.0.0.1'
HEADER_SIZE = 23
NOISE_SIZE = 1024

class connection:
    def __init__(self, protocol):
        self.port = port.get_port()
        self.host = HOST
        self.protocol = protocol

    def service_connection(self, key, mask):
        sock = key.fileobj
        data = key.data
        if mask & selectors.EVENT_READ:
            try:
                header_b = sock.recv(HEADER_SIZE)  # Should be ready to read
                payload_size = self.protocol.get_payload_size(header_b)
                payload_b = sock.recv(payload_size)
                noise = sock.recv(NOISE_SIZE)
                if header_b:
                    data.outb += header_b + payload_b
                else:
                    self.close(data, sock)
            except:
                self.close(data, sock)

        if mask & selectors.EVENT_WRITE:
            if data.outb:
                try:
                    print('DELETE', data.outb)
                    req = self.protocol.get_request(data.outb)
                    req.print()
                    res = self.protocol.handler(req)
                    res.print()
                    sock.send(res.to_bytes())  # Should be ready to write
                except Exception as e:
                    print('Something went wrong, Error:', e)
                    sock.send(Response(9000, str(e)))
                finally:
                    data.outb = b''

    def accept_wrapper(self, sock):
        conn, addr = sock.accept()  # Should be ready to read
        print('accepted connection from', addr)
        conn.setblocking(False)
        data = types.SimpleNamespace(addr=addr, inb=b'', outb=b'')
        events = selectors.EVENT_READ | selectors.EVENT_WRITE
        self.sel.register(conn, events, data=data)

    def start_listen(self):
        self.sel = selectors.DefaultSelector()
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.bind((self.host, self.port))
            s.listen()
            print('listening on port', self.port)
            s.setblocking(False)

            # registers the socket to be monitored with sel.select() for the read events
            self.sel.register(s, selectors.EVENT_READ, data=None)

            while True:
                # blocks until there are sockets ready for I/O
                events = self.sel.select(timeout=None)
                for key, mask in events:
                    if key.data is None: # from the listening socket
                        self.accept_wrapper(key.fileobj)
                    else:
                        self.service_connection(key, mask)

    def close(self, data, sock):
        print('closing connection to', data.addr)
        self.sel.unregister(sock)
        sock.close()