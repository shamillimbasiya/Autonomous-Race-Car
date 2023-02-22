import socket


class Client:

    def __init__(self):
        self.HEADER = 360
        self.PORT = 2525
        self.FORMAT = 'utf-8'
        self.DISCONNECT_MESSAGE = "!DISCONNECT"
        self.SERVER = "10.241.216.203"
        self.ADDR = (self.SERVER, self.PORT)

        self.client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.client.connect(self.ADDR)

        self.recieve_num = 0
        self.message = ""

    def send(self, msg):
        message = f"{len(msg):<{self.HEADER}}" + msg
        self.client.send(bytes(message, self.FORMAT))

    def recieve(self):
        msg = self.client.recv(self.HEADER).decode(self.FORMAT)
        print("Recieved: " + msg)
        self.recieve_num += 1
        print(f"Num of times recieved LIDAR-data: {self.recieve_num}")
        return msg
            
