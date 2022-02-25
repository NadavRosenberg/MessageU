import sqlite3
import uuid
from os import path
from datetime import datetime

DB_NAME = 'server.db'

class db:
    def __init__(self):
        try:
            self.conn = sqlite3.connect(DB_NAME)
                
            if self.is_clients_table_exist() == False:
                self.create_clients_table()

        except:
            print('Failed to open db!')

    def __del__(self):
        self.conn.commit()
        self.conn.close()

    def is_clients_table_exist(self):
        cur = self.conn.cursor()
        cur.execute('''
            SELECT name FROM sqlite_master WHERE type='table' AND name='clients'
        ''')
        return len(cur.fetchall()) > 0

    def create_clients_table(self):
        cur = self.conn.cursor()
        cur.execute('''
            CREATE TABLE clients(id VARCHAR(16) PRIMARY KEY, user_name VARCHAR(255) UNIQUE, public_key VARCHAR(160), last_seen DATE)
        ''')
        self.conn.commit()

    def is_user_exist(self, user_name):
        cur = self.conn.cursor()
        cur.execute('''
            SELECT * from clients WHERE name='?'
        ''', user_name)
        return len(cur.fetchall()) > 0

    def create_user(self, user_name, public_key):
        cur = self.conn.cursor()

        client_id = str(uuid.uuid1())[:16]
        time = datetime.now().strftime("%B %d, %Y %I:%M%p")

        cur.execute('''
            INSERT INTO clients VALUES(?, ?, ?, ?)
        ''', (client_id, user_name, public_key, time))

        self.conn.commit()
        return client_id

    def get_users(self):
        cur = self.conn.cursor()

        cur.execute('''
            SELECT id, user_name FROM clients
        ''')

        return cur.fetchall()

    def get_user_public_key(self, client_id):
        cur = self.conn.cursor()

        cur.execute('''
            SELECT public_key FROM clients WHERE id=?
        ''', (client_id,))

        return cur.fetchone()[0]

