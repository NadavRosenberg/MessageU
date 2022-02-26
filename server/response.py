import struct

VERSION = 2

class Response:
    def __init__(self, code, payload):
        self.code = code
        self.payload_size = len(payload);
        self.payload = payload

    def print(self):
        print("#####################################################################")
        print("############################### Response ############################")
        print('version:      ', VERSION)
        print('code:         ', self.code)
        print('payload_size: ', self.payload_size)
        print('payload:      ', self.payload, '\n')

    def to_bytes(self):
        return struct.pack("<c H I %ds\n" % self.payload_size, bytes(str(VERSION), "utf-8"), self.code, self.payload_size, bytes(self.payload, "utf-8"))
