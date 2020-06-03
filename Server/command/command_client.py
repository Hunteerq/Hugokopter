import json
import socket
import smbus
from threading import Thread


class Client(Thread):

    def __init__(self, connection, address, max_package, server=None):
        Thread.__init__(self)
        self.__connection = connection
        self.__address = address
        self.__data_type_to_i2c_byte_dictionary = self.__get_i2c_dictionary()
        self.__data_type_dictionary = self.__get_data_type_dictionary()
        self.__MAX_PACKAGE = max_package
        self.__server = server
        self.__end = False
        self.__bus = smbus.SMBus(1)
        self.__DEVICE_ADDRESS = 0x13

    def stop(self):
        self.__end = True

    @staticmethod
    def __get_i2c_dictionary():
        return {
            'forward': 0x01,
            'right': 0x02,
            'backward': 0x03,
            'left': 0x04,
            'up': 0x05,
            'down': 0x06,
            'stable': 0x07,
            'connect': 0x08,
            'disconnect': 0x09
        }

    def __get_data_type_dictionary(self):
        return {
            'control': lambda _deserialized_data: self.__send_command(_deserialized_data),
            'info': lambda _deserialized_data: self.__send_info(_deserialized_data)
        }

    def run(self):
        print(f'open connection {self.__connection}')
        data = True
        while data and not self.__end:
            try:
                self.__connection.settimeout(4)
                data = self.__connection.recv(self.__MAX_PACKAGE)
                if data == b'':
                    break
                deserialized_data = self.__deserialize_object(data)
                self.__data_type_dictionary[self.__get_request_type(deserialized_data)].__call__(deserialized_data)
            except KeyError:
                print('request handler not founded')
                self.__connection.sendall(self.__serialize_object({'respond': 'request cannot be handled}'}))
            except socket.timeout:
                pass
            except Exception as e:
                print(f'Error when handle request {type(e)} {e}')
                break
        print(f'close connection {self.__connection}')
        self.__connection.close()

    @staticmethod
    def __get_request_type(data):
        try:
            return data['data_type']
        except KeyError as e:
            print(f'Exception in parsing json file {e}')
            return
    
    @staticmethod
    def __get_control_type(data):
        try:
            return data['control_type']
        except KeyError as e:
            print(f'Exception in parsing json file {e}')
            return

    @staticmethod
    def __get_speed(data):
        try:
            return int(data['speed'])
        except KeyError as e:
            print(f'Exception in parsing json file {e}')
            return

    @staticmethod
    def __get_command(data):
        try:
            return data['command']
        except KeyError as e:
            print(f'Exception in parsing json file {e}')
            return

    @staticmethod
    def __serialize_object(sending_object):
        return json.dumps(sending_object)

    @staticmethod
    def __deserialize_object(sending_object):
        return json.loads(sending_object)

    def __send_command(self, deserialized_data):
        speed = self.__get_speed(deserialized_data)
        control_type = self.__get_control_type(deserialized_data)
        self.__send_i2c_bytes(self.__data_type_to_i2c_byte_dictionary[control_type], speed)

    def __send_info(self, deserialized_data):
        command = self.__get_command(deserialized_data)
        self.__send_i2c_bytes(self.__data_type_to_i2c_byte_dictionary[command], 0)

    def __send_i2c_bytes(self, byte, speed):
        print(f'Sending: byte = {byte}, speed = {speed}')
        self.__bus.write_byte_data(self.__DEVICE_ADDRESS, byte, speed)


