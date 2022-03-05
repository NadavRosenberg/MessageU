FILENAME = 'myport.info'

# extract port to listen
def get_port():
    try:
        with open(FILENAME, "r") as f:
            port = int(f.read())
            return port
    except Exception as e:
        print ("Failed to open '" + FILENAME + "', Error:", e)
        exit(0)