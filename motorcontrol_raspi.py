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
    global ser
    ser = None
    while ser is None:
        ser = open_serial()
        time.sleep(0.1)  # Wait a second before retrying
    return ser
    
ser = open_serial()
if not ser:
    ser = reconnect_serial()
    
time.sleep(0.01)  # Wait for the Arduino to initialize

# Initialize Pygame and the joystick
pygame.init()
pygame.joystick.init()

if pygame.joystick.get_count() == 0:
    print("No joystick detected. Exiting...")
    pygame.quit()
    exit()
    
# Get the gamepad
joystick = pygame.joystick.Joystick(0)
joystick.init()

# Initialize motor speed variables
max_velocity = 100#369.35
min_velocity = -100#-362.85

dead_zone = 0.05

# Function to get motor speed from the Y-axis position
def get_velocity(y_axis_position_right,y_axis_position_left):
    # Scale the Y-axis range [-1, 1] to the motor speed range [min_velocity, max_velocity]
    if abs(y_axis_position_right) < dead_zone and abs(y_axis_position_left) < dead_zone:
        return 0.0, 0.0  # No movement in dead zone
        
    velocity_right = (y_axis_position_right) / 2 * (max_velocity - min_velocity) - 1.44
    velocity_left = (y_axis_position_left) / 2 * (max_velocity - min_velocity) - 1.44
    return round(velocity_right, 2), round(velocity_left, 2)

def read_from_arduino():
    global ser
    try:    
        if ser and ser.in_waiting > 0:
            incoming_data = ser.readline().decode('utf-8').strip()
            #print(f'Raw data: {incoming_data}')
            if incoming_data:
                print(f"Received from Arduino: {incoming_data}")
    except serial.SerialException:
        print("Error reading from serial. Attempting to reconnect...")
        ser.close()
        ser = reconnect_serial()
        
def send_data():
    global ser
    try:
        for event in pygame.event.get():  # Update the joystick events
            if event.type == pygame.QUIT:
                pygame.quit()
                exit()

        # Read and print the left-right (Axis 3) and up-down (Axis 4) values
        #for i in range(joystick.get_numaxes()):
           # axis_value = joystick.get_axis(i)
           # print(f"Axis {i}: {axis_value:.2f}")
        left_right = joystick.get_axis(3)  # Axis 3 controls left-right
        up_down1 = joystick.get_axis(4)    # Axis 4 controls up-down
        up_down2 = joystick.get_axis(1)
        #print(f"Left-Right: {left_right}, Up-Down1: {up_down1}, Up-Down2: {up_down2}")  # Debugging print statement

        # Calculate the motor velocity based on the up-down movement (Axis 4)
        velocity_right, velocity_left = get_velocity(-up_down1,-up_down2)

        # Print the current values for left-right and up-down
        #print(f"Left-Right (Axis 3): {left_right:.2f}")
        #print(f"Up-Down (Axis 4): {up_down:.2f}")
            #print(f"{velocity_right}")
            #print(f"{velocity_left}")
        #print()
            #print(f"Velocity Right: {velocity_right:.2f}, Velocity Left: {velocity_left:.2f}")
        # Write data to the Arduino
        data = f"{velocity_right:.2f},{velocity_left:.2f}\n"
        print(f"Sending data to Arduino: {data}")  # Debugging print statement
        try:
            if ser and ser.is_open:
                ser.write(data.encode())
                ser.flush()
            else:
                print("Serial port closed. Reconnecting...")
                ser = reconnect_serial()
        except serial.SerialException:
            print("Serial write failed. Attempting to reconnect...")
            ser.close()
            ser = reconnect_serial()
            
        read_from_arduino()
        time.sleep(0.1)
    except serial.SerialException:
        print("Serial Exception occurred. Reconnecting...")
        ser.close()
        ser = reconnect_serial()

    
    except KeyboardInterrupt:
        print("Exiting...")
        pygame.quit()
        ser.close()
while True:      
    send_data()

