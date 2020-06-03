from flight_controller.flight_controller import FlightController
from time import sleep

if __name__ == "__main__":
    flightController = FlightController()
    #flightController.start()
    flightController.calibrate()
    flightController.turn_on()
    flightController.set_throttle(300)
    flightController.run()
    # flightController.turn_off()
    # flightController.stop()
    # flightController.join()