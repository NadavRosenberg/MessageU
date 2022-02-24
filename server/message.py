class message(request):
    def __init__(self, id, to_client, from_client, type, content):
        self.id = id
        self.to_client = to_client
        self.from_client = from_client
        self.type = type
        self.content = content