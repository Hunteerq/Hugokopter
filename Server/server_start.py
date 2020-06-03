import video_sender
from command import command_server

vs = video_sender.VideoSender()
vs.start()
sv = command_server.CommandServer()
sv.start()
input("Press enter to exit.\n")
vs.stop()
vs.join()
sv.stop()
sv.join()


