HEADER_SIZE = 23

class Request:
    def __init__(self, client_id, version, code, payload_size, payload):
        self.client_id = client_id.decode('utf-8')
        self.version = int.from_bytes(version, "little")
        self.code = int.from_bytes(code, "little")
        self.payload_size = len(payload)
        self.payload = payload.decode('utf-8')

    def print(self):
        print("#####################################################################")
        print("############################### Request #############################")
        print("client_id:    ", self.client_id)
        print("version:      ", self.version)
        print("code:         ", self.code)
        print("payload_size: ", self.payload_size)
        print("payload:      ", self.payload, '\n')
        
    def get_client_id(self):
        return self.client_id

    def get_code(self):
        return self.code

    def get_payload(self):
        return self.payload