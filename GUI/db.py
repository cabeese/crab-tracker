import sqlite3
from crab import Crab

database_url = "crabs.db"
create_database = """
                   CREATE TABLE crabs(
                   crab_id INTEGER PRIMARY KEY,
                   sex text,
                   species text,
                   color text,
                   damage text,
                   carapace DOUBLE,
                   mass DOUBLE,
                   epibiont text,
                   molt text
                   )"""
insert_query = "INSERT INTO crabs VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)"
read_crab_query = "SELECT * FROM crabs WHERE crab_id=?"
read_all_query = "SELECT * FROM crabs"


def create():
    conn = sqlite3.connect(database_url)
    cur = conn.cursor()
    cur.execute(create_database)
    conn.commit()
    conn.close()


def insert_crab(crab):
    conn = sqlite3.connect(database_url)
    cur = conn.cursor()
    cur.execute(insert_query, crab.get_tuple())

    conn.commit()
    conn.close()


def read_crab(crab_id):
    conn = sqlite3.connect(database_url)
    cur = conn.cursor()
    cur.execute(read_crab_query, (crab_id,))
    crab_result = cur.fetchone()

    conn.close()
    return crab_result


def read_first():
    conn = sqlite3.connect(database_url)
    cur = conn.cursor()
    cur.execute(read_all_query)
    crab_result = cur.fetchone()
    print(crab_result)

    conn.close()
    return crab_result


if __name__ == '__main__':
    #create()
    read_first()
