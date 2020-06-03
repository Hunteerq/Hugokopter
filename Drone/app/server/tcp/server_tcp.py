import json
import os
import socket
import sys
from threading import Thread, Lock


class ServerTcp(Thread):

    def __init__(self):
        Thread.__init__(self)
        self.__IP_ADDRESS = None
        self.__PORT_NUMBER = None
        self.__MAX_HOSTS = None
        self.__MAX_PACKAGE = None
        self.__socket = None
        self.__config_file = "res/tcp_server_config.json"
        self.__read_config()
        self.__bind_socket()
        self.__stopped = False
        self.__stop_signal_lock = Lock()

    def __read_config(self):
        if os.path.isfile(self.__config_file):
            with open(self.__config_file) as json_file:
                server_config_json = json.load(json_file)
                self.__IP_ADDRESS = server_config_json['serverIp']
                self.__PORT_NUMBER = server_config_json['serverPort']
                self.__MAX_HOSTS = server_config_json['maxHosts']
                self.__MAX_PACKAGE = server_config_json['maxPackage']
        else:
            print("ERROR READING JSON")

    def __bind_socket(self):
        try:
            self.__socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.__socket.bind((self.__IP_ADDRESS, self.__PORT_NUMBER))
        except socket.error as msg:
            print(f'Failed binding specified interface {self.__IP_ADDRESS} and port {self.__PORT_NUMBER} error {msg}')
            sys.exit(-10)

    def stop(self):
        self.__stop_signal_lock.acquire()
        self.__stopped = True
        self.__stop_signal_lock.release()

    def __run(self):
        self.__listen_for_connections()

    def __listen_for_connections(self):
        self.__socket.listen(self.__MAX_HOSTS)
        print('Server initialized')
        while not self.__check_stop():
            try:
                self.__socket.settimeout(1)
                connection, address = self.__socket.accept()
                print(f'Connected client {connection} + {address}')
            except socket.timeout:
                pass
            except Exception as e:
                print("unexpected error server will be stopped" + str(e))
                self.__stopped = True

    def __check_stop(self):
        self.__stop_signal_lock.acquire()
        result = self.__stopped
        self.__stop_signal_lock.release()
        return result


