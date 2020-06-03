from server.tcp.server_tcp import ServerTcp

if __name__ == '__main__':
    server = ServerTcp()
    server.start()
    input("Press enter to exit ;)\n")
    print("DEBUG send signal to stop sever")
    server.stop()
    server.join()
    print("DEBUG end main thread")
