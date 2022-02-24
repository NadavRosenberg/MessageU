##import random
##import struct
##from enum import Enum
##import time #dev
##import os

##RESPONSE_SIZE = 1024
##DEFAULT_ID = 1

##class OP(Enum):
##    UPLOAD_FILE = 100
##    DONWLOAD_FILE = 200
##    DELETE_FILE = 201
##    SHOW_FILES = 202

##class STATUS(Enum):
##    FILE_RETRIEVED = 210
##    FILE_LIST = 211
##    UPLOAD_OR_DELETE_SUCCEEDED = 212
##    FILE_NOT_FOUND = 1001
##    NO_FILES = 1002
##    INTERNAL_SERVER_ERROR = 1003

#def printresponse(data):
#    print('###### Response ######')
#    version, status, name_len = struct.unpack('<c H H', data[:5])
#    print('version:  ', version.decode("utf-8"))
#    print('status:   ', status)
#    print('name_len: ', name_len)
#    filename, size = struct.unpack('<%ds I' % name_len, data[5:name_len + 9])
#    if (name_len > 0):
#        filename = filename.decode("utf-8")
#        print('filename: ', filename)
#    print('size:     ', size)
#    if (size > 0 and size <= RESPONSE_SIZE):
#        payload = struct.unpack('<%ds' % size, data[name_len + 9:name_len + 9 + size])[0]
#        payload = payload.decode("utf-8")
#        print('payload:  ', payload)
#    print('######################')

#def printresult(status, filename):
#    if (status == STATUS.FILE_RETRIEVED.value):
#        print('File', filename, 'retrieved successfully!')
#    elif (status == STATUS.FILE_LIST.value):
#        print('File list retrieved successfully!')
#    elif (status == STATUS.UPLOAD_OR_DELETE_SUCCEEDED.value):
#        print('File', filename, 'uploaded/deleted successfully!')
#    elif (status == STATUS.FILE_NOT_FOUND.value):
#        print('File', filename, 'does not exist!')
#    elif (status == STATUS.NO_FILES.value):
#        print("You don't have any files yet!")
#    elif (status == STATUS.INTERNAL_SERVER_ERROR.value):
#        print('Something went wrong! please try again later..')
#    else:
#        print('Illegal status!')

#def send(msg):
#    try:
#        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
#        s.connect((ip, port))

#        # send the request to the server
#        s.send(msg)
        
#        # receive the response from the server
#        data = s.recv(RESPONSE_SIZE)

#        # print the response
#        printresponse(data)

#        # deconstruct the response
#        version, status, name_len = struct.unpack('<c H H', data[:5])
#        filename = struct.unpack('<%ds' % name_len, data[5:name_len + 5])[0]
#        filename = filename.decode("utf-8")

#        # print the result
#        printresult(status, filename)

#    except Exception as e:
#        print ("Could not open socket with", ip + ", Error:", e)
#    print()

#def uploadFile(filename):
#    try:
#        f = open(filename, "rb")
#        payload = f.read()
#        f.close()
#    except Exception as e:
#        print ("Error while opening the file", filename + ", Error:", e, '\n')
#        return;

#    file_stats = os.stat(filename)

#    packed = struct.pack(
#        '<i c B H %ds I %ds' % (len(filename), len(payload)), 
#        id, 
#        version, 
#        OP.UPLOAD_FILE.value, 
#        len(filename), 
#        filename.encode(), 
#        file_stats.st_size, 
#        payload
#    )
#    send(packed)

#def downloadFile(filename):
#    packed = struct.pack(
#        '<i c B H %ds' % (len(filename)), 
#        id, 
#        version, 
#        OP.DONWLOAD_FILE.value,
#        len(filename), 
#        filename.encode()
#    )
#    send(packed)

#def deleteFile(filename):
#    packed = struct.pack(
#        '<i c B H %ds' % (len(filename)), 
#        id, 
#        version, 
#        OP.DELETE_FILE.value,
#        len(filename), 
#        filename.encode()
#    )
#    send(packed)

#def showFiles():
#    packed = struct.pack(
#        '<i c B', 
#        id, 
#        version, 
#        OP.SHOW_FILES.value
#    )
#    send(packed)
    
### run large variety of flows
##def runedgecases():
##    # extract filenames
##    f = open(BACKUP_INFO, "r")
##    filenames = f.read().split('\n') # [mmn14.pdf, terminator2.avi, test.txt]
##    f.close()
    
##    print('> Uploading file', filenames[0], '(which is too big):')
##    uploadFile(filenames[0])

##    print('> Uploading file hello_world.jpg (which is not exist):')
##    uploadFile('hello_world.jpg')

##    print('> Uploading file', filenames[2] + ':')
##    uploadFile(filenames[2])

##    print('> Show files:')
##    showFiles()

##    print('> Downloading file', filenames[2] + ':')
##    downloadFile(filenames[2])

##    print('> Deleting file', filenames[2] + ':')
##    deleteFile(filenames[2])

##    print('> Downloading file', filenames[2], '(while the user don\'t have any files):')
##    downloadFile(filenames[2])

##    print('> Deleting file hello_world.txt (which is not exist):')
##    deleteFile('hello_world.txt')
    
### run a simple example
##def runexample():
##    # extract filenames
##    f = open(BACKUP_INFO, "r")
##    filenames = f.read().split('\n') # [mmn14.pdf, terminator2.avi, test.txt]
##    f.close()

##    print('> Show files:')
##    showFiles()

##    print('> Uploading file', filenames[0] + ':')
##    uploadFile(filenames[0])

##    print('> Uploading file', filenames[1] + ':')
##    uploadFile(filenames[1])

##    print('> Show files:')
##    showFiles()

##    print('> Downloading file', filenames[0] + ':')
##    downloadFile(filenames[0])

##    print('> Deleting file', filenames[0] + ':')
##    deleteFile(filenames[0])

##    print('> Downloading file', filenames[0] + ':')
##    downloadFile(filenames[0])
    