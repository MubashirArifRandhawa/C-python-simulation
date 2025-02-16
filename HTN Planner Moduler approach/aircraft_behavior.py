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

def call_once():
    print("call_once() is called...")

    if simulation is None:
        print("Simulation is not set!")
        return

    aircrafts = simulation.get_aircrafts()
    waypoints = simulation.get_waypoints()
   
    # Separate waypoints by color
    red_waypoints = [wp for wp in waypoints if wp.get_force() == "Red"]
    blue_waypoints = [wp for wp in waypoints if wp.get_force() == "Blue"]

    # Iterate through each aircraft and move it to a waypoint
    for aircraft in aircrafts:
        # Get the aircraft's current position (latitude, longitude)
        aircraft_lat, aircraft_lon = aircraft.get_position()

        # Initialize the target waypoint
        target_waypoint = None
        
        if aircraft.get_force() == "Red":
            # Assign a red waypoint to the red aircraft
            if red_waypoints:
                target_waypoint = red_waypoints.pop(0)  # Get the first available red waypoint
            else:
                print("No red waypoints available for aircraft", aircraft.get_name())
        elif aircraft.get_force() == "Blue":
            # Assign a blue waypoint to the blue aircraft
            if blue_waypoints:
                target_waypoint = blue_waypoints.pop(0)  # Get the first available blue waypoint
            else:
                print("No blue waypoints available for aircraft", aircraft.get_name())
        
        # If we have a valid target waypoint, move the aircraft
        if target_waypoint:
            # Get the target waypoint's position (latitude, longitude)
            target_lat, target_lon = target_waypoint.get_position()
            
            # Move the aircraft to the target waypoint's latitude and longitude
            aircraft.move_to(target_lat, target_lon)  
            aircraft_name = aircraft.get_name()  # Get the name of the aircraft
            print(f"Moving Aircraft {aircraft_name} from ({aircraft_lat}, {aircraft_lon}) to ({target_lat}, {target_lon}) at waypoint {target_waypoint.get_name()}")
        else:
            print(f"Aircraft {aircraft.get_name()} has no valid waypoint to move to.")



    return {"status": "success", "data": [1,2,3]}

def call_once_2():
    print("call_once() is called...")

    if simulation is None:
        print("Simulation is not set!")
        return

    aircrafts = simulation.get_aircrafts()
   
    # Initialize positions
    red_lon = 0
    blue_lon = 0
    lat = 0  # Fixed: Define `lat`
   
    for aircraft in aircrafts:
        if aircraft.get_force() == "Red":
            red_lon -= 10  # Increment the position for red aircraft
            lon = red_lon  # Use the red longitude
        else:  # Assuming any non-Red aircraft is Blue
            blue_lon += 10  # Increment the position for blue aircraft
            lon = blue_lon  # Use the blue longitude
   
        aircraft.move_to(lat, lon)  # Move the aircraft
        aircraft_name = aircraft.get_name()  # Get the name of the aircraft
        print(f"Moving Aircraft {aircraft_name} to ({lat}, {lon}) in lat/lon")


    return {"status": "success", "data": [1,2,3]}


def call_once_1():
    print("call_once() is called...")

    if simulation is None:
        print("Simulation is not set!")
        return

    aircrafts = simulation.get_aircrafts()

    # Grid parameters
    grid_size = 5  # Grid of 7x7
    spacing = 15   # Distance between each aircraft in the grid
    # Define quadrants
    red_initial_lat = 90-20  # Top-left quadrant
    red_initial_lon = -180+30
    red_target_lat = 90-20  # Top-right quadrant
    red_target_lon = 180-30

    blue_initial_lat = -90+20  # Bottom-right quadrant
    blue_initial_lon = 180-30
    blue_target_lat = -90+20  # Bottom-left quadrant
    blue_target_lon = -180+30

    red_count = 0  # Counter for Red aircraft
    blue_count = 0  # Counter for Blue aircraft

    for aircraft in aircrafts:
        if aircraft.get_force() == "Red":
            # Compute new position for Red aircraft
            row = red_count // grid_size
            col = red_count % grid_size
            lat = red_target_lat - row * spacing
            lon = red_target_lon - col * spacing  # Moving to the top-right
            red_count += 1
        else:  # Blue aircraft
            # Compute new position for Blue aircraft
            row = blue_count // grid_size
            col = blue_count % grid_size
            lat = blue_target_lat + row * spacing
            lon = blue_target_lon + col * spacing  # Moving to the bottom-left
            blue_count += 1

        # Move the aircraft
        aircraft.move_to(lat, lon)
        aircraft_name = aircraft.get_name()  # Get the name of the aircraft
        print(f"Moving Aircraft {aircraft_name} to ({lat}, {lon}) in lat/lon")


# Track the last update time
last_update_time = time.time()
i = 0
def sim_update():
    if simulation is None:
        print("Simulation is not set!")
        return

    # Perform a single update for aircrafts
#    aircrafts = simulation.get_aircrafts()
#
#    target_locations = [
#        [(70, -120), (-70, 120)],
#        [(-70, 120), (70, -120)],
#        [(70, -120), (-70, 120)],
#        [(-70, 120), (70, -120)]
#    ]
#    # Check if 10 seconds have passed
#    global last_update_time, i
#    current_time = time.time()
#    
#    if (current_time - last_update_time >= 10 and i < len(target_locations)):
#        print("move to next location: ", target_locations[i])
#        for aircraft, target in zip(aircrafts, target_locations[i]):
#            aircraft_name = aircraft.get_name()
#            lat, lon = target
#            aircraft.move_to(lat, lon)
#            print(f"Moving Aircraft {aircraft_name} to ({lat}, {lon}) in lat/lon")
#    
#        # Reset the last update time
#        last_update_time = current_time
#        i+=1;