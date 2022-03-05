import db
from connection import connection
from protocol import protocol

def main():
    clients_db = db.db()
    prcl = protocol(clients_db)
    conn = connection(prcl)
    conn.start_listen()

if __name__ == "__main__":
    main()
