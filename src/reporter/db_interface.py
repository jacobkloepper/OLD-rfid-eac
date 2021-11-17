import sqlite3

class db_interface:

    def __init__(self):
        # "Default constructor"
        self.path = "src/data/user_data/users.db"
    
        # Setup connection
        self.conn = sqlite3.connect(self.path)
        self.c = self.conn.cursor()

    def get_usernames(self):
        self.c.execute("SELECT firstname FROM users")
        firsts = self.c.fetchall()
        firsts = [item for tup in firsts for item in tup]

        self.c.execute("SELECT lastname FROM users")
        lasts = self.c.fetchall()
        lasts = [item for tup in lasts for item in tup]

        return firsts,lasts