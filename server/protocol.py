import struct
from request import Request
from response import Response
from message import Message

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
        # create payload
        message = self.get_message(request)
        msg_id = self.db.save_message(message)

        # create response
        return Response(2103, message.get_to_client() + str(msg_id))

    def requestMessages(self, request):
        # create payload
        messages = self.db.get_messages(request.get_client_id())
        #payload = ''.join([user_id + user_name for user_id, user_name in messages if user_id != client_id])

        # create response
        return Response(2104, message.get_to_client() + str(msg_id))

    def default(self, request):
        print('Something went wrong ..')
        raise Exception('Request\' code is invalid!')

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
    
    def get_message(self, request):
        data_bytes = bytes(request.get_payload(), 'utf-8')
        content_size = len(data_bytes) - struct.calcsize('16s c 4s');
        to_client, msg_type, content_size, content = struct.unpack("16s c 4s %ds" % content_size, data_bytes)
        return Message(to_client, request.get_client_id(), msg_type, content)