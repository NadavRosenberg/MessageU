import struct
from request import Request
from response import Response

class protocol:
    HEADER_SIZE = 23

    def __init__(self, db):
        self.db = db
    
    def registerUser(self, request):
        try:
            payload = request.get_payload()
            name = payload[:255]
            public_key = payload[255:]

            uuid = self.db.create_user(name, public_key)
            res = Response(2100, uuid) #str
            return res
        except Exception as e:
            print('Something went wrong, Error:', e)

    def requestClientsList(self, request):
        res = Response(2101, str(uuid.uuid1()))
        print('in 1101')

    def requestPublicKey(self, request):
        print('in 1102')

    def sendMessage(self, request):
        print('in 1103')

    def requestMessages(self, request):
        print('in 1104')

    def default(self):
        print('Something went wrong ..')

    def handler(self, request):
        switcher = {
            1100: self.registerUser,
            1101: self.requestClientsList,
            1102: self.requestPublicKey,
            1103: self.sendMessage,
            1104: self.requestMessages
        }

        def switch(request):
            return switcher.get(request.get_code(), self.default)(request)

        return switch(request)

    def get_request(self, data_bytes):
        payload_size = len(data_bytes) - struct.calcsize('16s c 2s 4s');
        unpacked = struct.unpack("16s c 2s 4s %ds" % payload_size, data_bytes)
        return Request(*unpacked)


    def get_payload_size(self, header_bytes):
        unpacked = struct.unpack("16s c 2s 4s", header_bytes)
        return int.from_bytes(unpacked[3], "little")