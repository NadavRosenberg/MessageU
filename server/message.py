from request import Request

class Message(Request):
    def __init__(self, to_client, from_client, msg_type, content):
        self.id = -1
        self.to_client = to_client.decode('utf-8', 'backslashreplace')
        self.from_client = from_client
        self.msg_type = int.from_bytes(msg_type, "little")
        self.content = content.decode('utf-8', 'backslashreplace')

    def get_data(self):
        return (self.to_client, self.from_client, self.msg_type, self.content)

    def get_to_client(self):
        return self.to_client

    def get_type(self):
        return self.msg_type