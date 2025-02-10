import pygame
import time
import serial

def open_serial():
    try:
        return serial.Serial('/dev/ttyACM0', 9600, timeout=1)
    except serial.SerialException:
        print("Error opening serial port. Retrying...")
        return None

def reconnect_serial():
    """Attempt to reconnect to the serial port if disconnected"""
    ser = None
    while ser is None:
        ser = open_serial()
        if ser is None:
            time.sleep(1)  # Wait before retrying
    return ser

ser = open_serial()
if not ser:
    ser = reconnect_serial()

time.sleep(0.5)  # Give time for Arduino to initialize

# Initialize Pygame and the joystick
pygame.init()
pygame.joystick.init()

# Get the gamepad
joystick = pygame.joystick.Joystick(0)
joystick.init()

# Initialize motor speed variables
max_velocity = 50
min_velocity = -50
dead_zone = 0.1

def get_velocity(y_axis_position_right, y_axis_position_left):
    if abs(y_axis_position_right) < dead_zone and abs(y_axis_position_left) < dead_zone:
        return 0.0, 0.0  # No movement in dead zone
    velocity_right = (y_axis_position_right) / 2 * (max_velocity - min_velocity) 
    velocity_left = (y_axis_position_left) / 2 * (max_velocity - min_velocity) 
    return velocity_right, velocity_left

def send_data():
    global ser
    try:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                exit()

        left_right = joystick.get_axis(3)
        up_down1 = joystick.get_axis(4)
        up_down2 = joystick.get_axis(1)

        velocity_right, velocity_left = get_velocity(-up_down1, -up_down2)

        try:
            if ser and ser.is_open:
                data = f"{velocity_right:.2f},{velocity_left:.2f}"
                #print(data)
                ser.write(data.encode())
                ser.flush()
                print(f"Sent: {data.strip()}")
            else:
                print("Serial port closed. Reconnecting...")
                ser = reconnect_serial()
        except serial.SerialException:
            print("Serial write failed. Attempting to reconnect...")
            ser.close()
            ser = reconnect_serial()

        try:
            if ser and ser.in_waiting > 0:
                incoming_data = ser.readline().decode('utf-8').strip()
                if incoming_data:
                    velocity_right, velocity_left = map(float, incoming_data.split(','))
                    print(f"Received: {velocity_right},{velocity_left}")
                else:
                    print("Received empty data, skipping.")

            #else:
                #print("No data available from Arduino.")
        except serial.SerialException:
            print("Error reading from serial. Reconnecting...")
            ser.close()
            ser = reconnect_serial()

        time.sleep(0.2)  # Small delay for stability

    except KeyboardInterrupt:
        print("Exiting...")
        pygame.quit()
        ser.close()
        exit()

while True:
    send_data()

