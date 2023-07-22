import serial
import RPi.GPIO as GPIO

class DripperatorDriver():
    UART_BAUD_RATE = 38400
    UART_FRAME_SIZE = serial.EIGHTBITS
    UART_PARITY = serial.PARITY_NONE
    UART_STOP = serial.STOPBITS_ONE

    # Constants for header construction
    SYNC_POSITION = 7
    SYNC_MASK = 0x1
    SYNC = SYNC_MASK << SYNC_POSITION
    ADDRESS_POSITION = 0
    ADDRESS_MASK = 0x0F
    ADDRESS = ADDRESS_MASK << ADDRESS_POSITION


    def __init__(self, uart_path: str, flow_ctrl_gpio: int, num_pumps: int) -> None:
        self.serial_port = serial.Serial(uart_path,
                                         baudrate=DripperatorDriver.UART_BAUD_RATE,
                                         bytesize=DripperatorDriver.UART_FRAME_SIZE,
                                         parity=DripperatorDriver.UART_PARITY)
        self.num_pumps = num_pumps
        self.flow_ctrl_pin = flow_ctrl_gpio
        # default to flow control high because we're the only one who will ever talk
        GPIO.setmode(GPIO.BCM)
        GPIO.setup(self.flow_ctrl_pin, GPIO.OUT)
        GPIO.output(self.flow_ctrl_pin, GPIO.HIGH)

    def display_row(self, row: bytes):
        if (len(row) < self.num_pumps):
            raise ValueError(f"Row must be {self.num_pumps} bytes long")
        for i in range(0, self.num_pumps):
            self.__send_solenoid_states(i, row[i])
        self.__sync_and_open()

    def __send_solenoid_states(self, address: int, solenoid_states: int):
        data_to_send = [
            self.__encode_header(False, address),
            solenoid_states,
        ]
        self.serial_port.write(bytes(data_to_send))
        
    def __sync_and_open(self):
        data_to_send = [
            self.__encode_header(True, 0),
        ]
        self.serial_port.write(bytes(data_to_send))

    def __encode_header(self, sync :bool, addr: int) -> int:
        return (((sync << self.SYNC_POSITION) & self.SYNC) +
                ((addr << self.ADDRESS_POSITION) & self.ADDRESS_MASK))

if __name__ == "__main__":
    print("Dripperator Tester")
    dripperator = DripperatorDriver("/dev/serial0", 17, 1)
    dripperator.display_row(bytes.fromhex("00"))