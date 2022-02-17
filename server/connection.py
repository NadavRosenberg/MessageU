import selectors
import socket
import port

HOST = '127.0.0.1'

class Connection:
    def __init__(self):
        self.port = port.get_port()
        self.host = HOST
        self.start_listen()

    def service_connection(cls, key, mask):
        sock = key.fileobj
        data = key.data
        if mask & selectors.EVENT_READ:
            recv_data = sock.recv(1024)  # Should be ready to read
            if recv_data:
                data.outb += recv_data
            else:
                print('closing connection to', data.addr)
                sel.unregister(sock)
                sock.close()
        if mask & selectors.EVENT_WRITE:
            if data.outb:
                print('echoing', repr(data.outb), 'to', data.addr)
                sent = sock.send(data.outb)  # Should be ready to write
                data.outb = data.outb[sent:]

    def accept_wrapper(cls, sock):
        conn, addr = sock.accept()  # Should be ready to read
        print('accepted connection from', addr)
        conn.setblocking(False)
        data = types.SimpleNamespace(addr=addr, inb=b'', outb=b'')
        events = selectors.EVENT_READ | selectors.EVENT_WRITE
        sel.register(conn, events, data=data)

    def start_listen(self):
        sel = selectors.DefaultSelector()
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.bind((self.host, self.port))
            s.listen()
            print('listening on port', self.port)
            s.setblocking(False)

            # registers the socket to be monitored with sel.select() for the read events
            sel.register(s, selectors.EVENT_READ, data=None)

            while True:
                # blocks until there are sockets ready for I/O
                events = sel.select(timeout=None)
                for key, mask in events:
                    if key.data is None: # from the listening socket
                        self.accept_wrapper(key.fileobj)
                    else:
                        self.service_connection(key, mask)