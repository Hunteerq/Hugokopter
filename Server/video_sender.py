import socket
import struct
from threading import Thread, Lock
from picamera import PiCamera
import io
import time
import sys
import os
import json


def _send_image(client, image):
    image_size = image.getbuffer().nbytes
    data_to_send = io.BytesIO()
    data_to_send.write(struct.pack(">i", image_size))
    data_to_send.write(image.getvalue())
    client.sendall(data_to_send.getvalue())


class VideoSender(Thread):

    def __init__(self):
        Thread.__init__(self)
        self._config_file = "config/server_config.json"
        self._camera = None
        self._IP_ADDRESS = None
        self._PORT_NUMBER = None
        self._VIDEO_FRAMERATE = None
        self._VIDEO_WIDTH = None
        self._VIDEO_HEIGHT = None
        self._read_config()
        self._camera_framerate = self._VIDEO_FRAMERATE
        self._camera_resolution = (self._VIDEO_WIDTH, self._VIDEO_HEIGHT)
        self._socket = None
        self._initialize_camera()
        self._initialize_socket()
        self._stopped = False
        self._paused = False
        self._stop_signal_lock = Lock()
        self._pause_signal_lock = Lock()

    def _initialize_camera(self):
        self._camera = PiCamera()
        self._camera.resolution = self._camera_resolution
        self._camera.framerate = self._camera_framerate
        self._camera.rotation = 180

    def _initialize_socket(self):
        try:
            self._socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self._socket.bind((self._IP_ADDRESS, self._PORT_NUMBER))
            self._socket.listen(1)
        except socket.error as msg:
            print(f'Initialize socket failed')
            sys.exit(-1)

    def _read_config(self):
        if os.path.isfile(self._config_file):
            with open(self._config_file) as json_file:
                server_config_json = json.load(json_file)
                self._IP_ADDRESS = server_config_json['ipAddress']
                self._PORT_NUMBER = server_config_json['videoPort']
                self._VIDEO_FRAMERATE = server_config_json['videoFrameRate']
                self._VIDEO_WIDTH = server_config_json['videoWidth']
                self._VIDEO_HEIGHT = server_config_json['videoHeight']
        else:
            print("ERROR READING JSON")

    def stop(self):
        self._stop_signal_lock.acquire()
        self._stopped = True
        self._stop_signal_lock.release()

    def pause(self):
        self._pause_signal_lock.acquire()
        self._paused = True
        self._pause_signal_lock.release()

    def resume(self):
        self._pause_signal_lock.acquire()
        self._paused = False
        self._pause_signal_lock.release()

    def _is_thread_stopped(self):
        self._stop_signal_lock.acquire()
        result = self._stopped
        self._stop_signal_lock.release()
        return result

    def _is_paused(self):
        self._pause_signal_lock.acquire()
        result = self._paused
        self._pause_signal_lock.release()
        return result

    def _stream(self, client):
        stream = io.BytesIO()
        time.sleep(0.5)
        for frame in self._camera.capture_continuous(stream, format="jpeg", use_video_port=True):
            image = stream
            if not self._is_paused():
                _send_image(client, image)
            stream.truncate()
            stream.seek(0)
            if self._is_thread_stopped():
                break

    def run(self):
        self._socket.settimeout(5.0)
        while not self._is_thread_stopped():
            try:
                client, address = self._socket.accept()
                self._stream(client)
            except:
                pass

