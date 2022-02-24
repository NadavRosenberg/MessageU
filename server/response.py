import struct

VERSION = 1

class Response:
    def __init__(self, code, payload):
        self.code = code
        self.payload_size = 7 + len(payload);
        self.payload = payload

    def print(self):
        print("#####################################################################")
        print("############################### Response ############################")
        print('version:      ', VERSION)
        print('code:         ', self.code)
        print('payload_size: ', self.payload_size)
        print('payload:      ', self.payload, '\n')

    def to_bytes(self):
        a = struct.pack("c H", bytes(str(VERSION), "utf-8"), self.code)
        b = struct.pack("<c H I %ds\n" % self.payload_size, bytes(str(VERSION), "utf-8"), self.code, self.payload_size, bytes(self.payload, "utf-8"))
        return b