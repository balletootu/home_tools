

dbfilename = 'people-file'
END_DB = 'enddb.'
END_REC = 'endrec.'
REC_SEP = ' =>'

def storeDbase(db, dbfilename = dbfilename):
    "save db to file"
    dbfile = open(dbfilename, 'w')
    for key in db:
        print(key, file=dbfile)
        for (name, value) in db[key].items():
            print(name + REC_SEP + repr(value), file = dbfile)
        print(END_REC, file=dbfile)
    print(END_DB, file=dbfile)
    dbfile.close()
    
def loadDbase(dbfilename = dbfilename):
    dbfile = open(dbfilename)
    import sys
    sys.stdin = dbfile
    db = {}
    key = input()
    while key != END_DB:
        rec = {}
        field = input()
        while field != END_REC :
            name, value = field.split(REC_SEP)
            rec[name] = eval(value)
            field = input()
        db[key] = rec
        key = input()
    return db
    
if __name__ == '__main__':
    from initdata import db
    storeDbase(db)