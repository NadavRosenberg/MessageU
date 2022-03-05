class Client(object):
    def __init__(self, id, user_name, public_key, last_seen):
        self.id = id
        self.user_name = user_name
        self.public_key = public_key
        self.last_seen = last_seen