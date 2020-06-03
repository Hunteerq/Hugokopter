import json
import os
import socket
import sys
from threading import Thread, Lock

from command.command_client import Client


class CommandServer(Thread):

    def __init__(self):
        Thread.__init__(self)
        self.__config_file = "config/server_config.json"
        self.__IP_ADDRESS = None
        self.__PORT_NUMBER = None
        self.__MAX_PACKAGE = None
        self.__read_config()
        self.__socket = None
        self.__initialize_socket()
        self.__stopped = False
        self.__paused = False
        self.__stop_signal_lock = Lock()
        self.__pause_signal_lock = Lock()


    def __initialize_socket(self):
        try:
            self.__socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.__socket.bind((self.__IP_ADDRESS, self.__PORT_NUMBER))
            self.__socket.listen(1)
            print(f'Server initialized')
        except socket.error as msg:
            print(f'Initialize socket failed')
            sys.exit(-1)
            
    def __read_config(self):
        if os.path.isfile(self.__config_file):
            with open(self.__config_file) as json_file:
                server_config_json = json.load(json_file)
                self.__IP_ADDRESS = server_config_json['ipAddress']
                self.__PORT_NUMBER = server_config_json['port']
                self.__MAX_PACKAGE = server_config_json['maxPackage']
        else:
            print("ERROR READING JSON")

    def stop(self):
        self.__stop_signal_lock.acquire()
        self.__stopped = True
        self.__stop_signal_lock.release()

    def pause(self):
        self.__pause_signal_lock.acquire()
        self.__paused = True
        self.__pause_signal_lock.release()

    def resume(self):
        self.__pause_signal_lock.acquire()
        self.__paused = False
        self.__pause_signal_lock.release()

    def __is_thread_stopped(self):
        self.__stop_signal_lock.acquire()
        result = self.__stopped
        self.__stop_signal_lock.release()
        return result

    def __is_paused(self):
        self.__pause_signal_lock.acquire()
        result = self.__paused
        self.__pause_signal_lock.release()
        return result

    def run(self):
        while not self.__is_thread_stopped():
            try:
                connection, address = self.__socket.accept()
                self.__handle_commands(connection, address)
            except socket.timeout:
                pass
            except Exception as e:
                print(f'Unexpected error server will be stopped {str(e)}')
                self.__stopped = True

    def __handle_commands(self, connection, address):
        client = Client(connection, address, self.__MAX_PACKAGE, self)
        client.start()



