import aircraft_simulation
import time
import random
import math

# Global variable to store the simulation object
simulation = None

def set_simulation(sim):
    global simulation
    simulation = sim
    print("Simulation object set in Python!")

def control_aircrafts_once():
    print("control called in Python!")
    if simulation is None:
        print("Simulation is not set!")
        return

    # Perform a single update for aircrafts
    aircrafts = simulation.get_aircrafts()
    for aircraft in aircrafts:
        # Generate random new position for each aircraft
        new_x = random.randint(50, 750)
        new_y = random.randint(50, 550)

        print(f"Moving Aircraft {aircraft.get_name()} to ({new_x}, {new_y})")
        aircraft.move_to(new_x, new_y)  # Move aircraft to new random location


# aircraft_deltas = {}
# 
# def move_aircraft_in_linear_pattern():
#     if simulation is None:
#         print("Simulation is not set!")
#         return
# 
#     # Perform a single update for aircrafts
#     aircrafts = simulation.get_aircrafts()
#     for aircraft in aircrafts:
#         # Get the unique name of the aircraft to track its deltas
#         aircraft_name = aircraft.get_name()
# 
#         # Initialize deltas if not already set
#         if aircraft_name not in aircraft_deltas:
#             aircraft_deltas[aircraft_name] = {'delta_x': 1, 'delta_y': 0}  # Start moving right
# 
#         # Retrieve current deltas
#         delta_x = aircraft_deltas[aircraft_name]['delta_x']
#         delta_y = aircraft_deltas[aircraft_name]['delta_y']
# 
#         # Get the current position of the aircraft
#         current_x, current_y = aircraft.get_position()
#         print(f"Current position of {aircraft_name}: ({current_x}, {current_y})")
# 
#         # Update direction (delta_x, delta_y) when hitting boundaries
#         if current_x >= 750:  # Right boundary
#             delta_x = -1  # Move left
#             delta_y = 0   # Stop vertical movement
#             new_heading = math.atan2(-delta_y, delta_x) * 180.0 / math.pi  # Convert radians to degrees
#             aircraft.set_heading(new_heading)  # Update the heading
#         elif current_x <= 5:  # Left boundary
#             delta_x = 1  # Move right
#             delta_y = 0   # Stop vertical movement
#             new_heading = math.atan2(-delta_y, delta_x) * 180.0 / math.pi  # Convert radians to degrees
#             aircraft.set_heading(new_heading)  # Update the heading
# 
#         if current_y >= 550:  # Bottom boundary
#             delta_x = 0   # Stop horizontal movement
#             delta_y = -1  # Move up
#             new_heading = math.atan2(-delta_y, delta_x) * 180.0 / math.pi  # Convert radians to degrees
#             aircraft.set_heading(new_heading)  # Update the heading
#         elif current_y <= 5:  # Top boundary
#             delta_x = 0   # Stop horizontal movement
#             delta_y = 1   # Move down
#             new_heading = math.atan2(-delta_y, delta_x) * 180.0 / math.pi  # Convert radians to degrees
#             aircraft.set_heading(new_heading)  # Update the heading
# 
#         # Store the updated deltas
#         aircraft_deltas[aircraft_name] = {'delta_x': delta_x, 'delta_y': delta_y}
# 
#         # Calculate the new position
#         new_x = current_x + delta_x
#         new_y = current_y + delta_y
# 
#         # Ensure the aircraft stays within the bounds
#         new_x = max(0, min(new_x, 750))
#         new_y = max(0, min(new_y, 550))
# 
#         print(f"Moving Aircraft {aircraft_name} to ({new_x}, {new_y})")
# 
#         # Move the aircraft to the new position
#         aircraft.move_to(new_x, new_y)



# Define the coordinate system boundaries and screen size
min_latitude = -90.0
max_latitude = 90.0
min_longitude = -180.0
max_longitude = 180.0
screen_width = 800
screen_height = 600

def lat_lon_to_screen(lat, lon):
    """
    Convert latitude/longitude to screen coordinates.
    """
    # Normalize latitude and longitude to [0, 1] range
    normalized_lat = (lat - min_latitude) / (max_latitude - min_latitude)
    normalized_lon = (lon - min_longitude) / (max_longitude - min_longitude)

    # Invert latitude Y-axis to match screen coordinates (0 is top of screen, max is bottom)
    normalized_lat = 1.0 - normalized_lat

    # Map normalized latitude and longitude to screen space
    screen_x = int(normalized_lon * screen_width)
    screen_y = int(normalized_lat * screen_height)

    return screen_x, screen_y

def screen_to_lat_lon(screen_x, screen_y):
    """
    Convert screen coordinates back to latitude/longitude.
    """
    # Normalize screen coordinates to [0, 1] range
    normalized_lon = screen_x / screen_width
    normalized_lat = 1.0 - (screen_y / screen_height)

    # Convert to latitude and longitude
    lat = min_latitude + normalized_lat * (max_latitude - min_latitude)
    lon = min_longitude + normalized_lon * (max_longitude - min_longitude)

    return lat, lon

# Dictionary to track aircraft movement deltas (latitude and longitude)
aircraft_deltas = {}

def move_aircraft_in_linear_pattern():
    if simulation is None:
        print("Simulation is not set!")
        return

    # Perform a single update for aircrafts
    aircrafts = simulation.get_aircrafts()
    for aircraft in aircrafts:
        # Get the unique name of the aircraft to track its deltas
        aircraft_name = aircraft.get_name()

        # Initialize deltas if not already set
        if aircraft_name not in aircraft_deltas:
            aircraft_deltas[aircraft_name] = {'delta_lat': 0.1, 'delta_lon': 0}  # Start moving east (longitude)

        # Retrieve current position in lat/lon
        current_lat, current_lon = aircraft.get_position()  # Assuming get_position() returns lat, lon
        print(f"Current position of {aircraft_name} in lat/lon: ({current_lat}, {current_lon})")

        # Retrieve current deltas for lat/lon
        delta_lat = aircraft_deltas[aircraft_name]['delta_lat']
        delta_lon = aircraft_deltas[aircraft_name]['delta_lon']

        # Update direction (delta_lat, delta_lon) when hitting boundaries (latitude/longitude)
        if current_lon >= max_longitude:  # Right boundary (longitude)
            delta_lon = -0.1  # Move left (west)
            aircraft.set_heading(180.0)  # Heading changes to 180 degrees (west)
        elif current_lon <= min_longitude:  # Left boundary (longitude)
            delta_lon = 0.1  # Move right (east)
            aircraft.set_heading(0.0)  # Heading changes to 0 degrees (east)

        if current_lat >= max_latitude:  # Top boundary (latitude)
            delta_lat = -0.1  # Move south
            aircraft.set_heading(270.0)  # Heading changes to 270 degrees (south)
        elif current_lat <= min_latitude:  # Bottom boundary (latitude)
            delta_lat = 0.1  # Move north
            aircraft.set_heading(0.0)  # Heading changes to 0 degrees (north)

        # Store the updated deltas
        aircraft_deltas[aircraft_name] = {'delta_lat': delta_lat, 'delta_lon': delta_lon}

        # Calculate the new latitude/longitude
        new_lat = current_lat + delta_lat
        new_lon = current_lon + delta_lon

        # Ensure the aircraft stays within the bounds of the coordinate system
        new_lat = max(min_latitude, min(new_lat, max_latitude))
        new_lon = max(min_longitude, min(new_lon, max_longitude))

        print(f"Moving Aircraft {aircraft_name} to ({new_lat}, {new_lon}) in lat/lon")

        # Move the aircraft to the new position in lat/lon
        aircraft.move_to(new_lat, new_lon)





def sim_update():
    move_aircraft_in_linear_pattern()
    pass