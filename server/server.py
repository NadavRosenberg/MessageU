from connection import Connection

MY_PORT_INFO = 'myport.info'

def get_port():
    global port

    # extract port to listen
    try:
        f = open(MY_PORT_INFO, "r")
        port = int(f.read())
        f.close()
    except Exception as e:
        print ("Failed to open '" + MY_PORT_INFO + "', Error:", e)
        exit(0)

def init():
    get_port()
    Connection(port)

def main():
    init()
    #runexample()

init()   
if __name__ == "__main__":
    main()
    