import struct
from request import Request
from response import Response

UUID_SIZE = 16
HEADER_SIZE = 23

class protocol:

    def __init__(self, db):
        self.db = db
    
    def registerUser(self, request):
        # create payload
        payload = request.get_payload()
        name = payload[:255]
        public_key = payload[255:]
        
        # create response
        try:
            uuid = self.db.create_user(name, public_key)
            return Response(2100, uuid)
        except:
            raise Exception('Username already exists!')

    def requestClientsList(self, request):
        # create payload
        users = self.db.get_users()
        client_id = request.get_client_id()
        payload = ''.join([user_id + user_name for user_id, user_name in users if user_id != client_id])

        # create response
        return Response(2101, payload)

    def requestPublicKey(self, request):
        # create payload
        client_id = request.get_payload()
        if len(client_id) != UUID_SIZE:
            raise Exception() 
        public_key = self.db.get_user_public_key(client_id);

        # create response
        return Response(2102, client_id + public_key)

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
            try:
                return switcher.get(request.get_code(), self.default)(request)
            except Exception as e:
                return Response(9000, str(e))

        return switch(request)

    def get_request(self, data_bytes):
        payload_size = len(data_bytes) - struct.calcsize('16s c 2s 4s');
        unpacked = struct.unpack("16s c 2s 4s %ds" % payload_size, data_bytes)
        return Request(*unpacked)


    def get_payload_size(self, header_bytes):
        unpacked = struct.unpack("16s c 2s 4s", header_bytes)
        return int.from_bytes(unpacked[3], "little")