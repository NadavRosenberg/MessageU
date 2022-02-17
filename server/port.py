MY_PORT_INFO = 'myport.info'

# extract port to listen
def get_port():
    try:
        with open(MY_PORT_INFO, "r") as f:
            port = int(f.read())
            return port
    except Exception as e:
        print ("Failed to open '" + MY_PORT_INFO + "', Error:", e)
        exit(0)