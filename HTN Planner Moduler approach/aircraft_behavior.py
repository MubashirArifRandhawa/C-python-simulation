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
            aircraft_deltas[aircraft_name] = {'delta_x': 1, 'delta_y': 0}  # Start moving right

        # Retrieve current deltas
        delta_x = aircraft_deltas[aircraft_name]['delta_x']
        delta_y = aircraft_deltas[aircraft_name]['delta_y']

        # Get the current position of the aircraft
        current_x = aircraft.get_x()
        current_y = aircraft.get_y()

        # Update direction (delta_x, delta_y) when hitting boundaries
        if current_x >= 750:  # Right boundary
            delta_x = -1  # Move left
            delta_y = 0   # Stop vertical movement
            new_heading = math.atan2(-delta_y, delta_x) * 180.0 / math.pi  # Convert radians to degrees
            aircraft.set_heading(new_heading)  # Update the heading
        elif current_x <= 5:  # Left boundary
            delta_x = 1  # Move right
            delta_y = 0   # Stop vertical movement
            new_heading = math.atan2(-delta_y, delta_x) * 180.0 / math.pi  # Convert radians to degrees
            aircraft.set_heading(new_heading)  # Update the heading

        if current_y >= 550:  # Bottom boundary
            delta_x = 0   # Stop horizontal movement
            delta_y = -1  # Move up
            new_heading = math.atan2(-delta_y, delta_x) * 180.0 / math.pi  # Convert radians to degrees
            aircraft.set_heading(new_heading)  # Update the heading
        elif current_y <= 5:  # Top boundary
            delta_x = 0   # Stop horizontal movement
            delta_y = 1   # Move down
            new_heading = math.atan2(-delta_y, delta_x) * 180.0 / math.pi  # Convert radians to degrees
            aircraft.set_heading(new_heading)  # Update the heading

        # Store the updated deltas
        aircraft_deltas[aircraft_name] = {'delta_x': delta_x, 'delta_y': delta_y}

        # Calculate the new position
        new_x = current_x + delta_x
        new_y = current_y + delta_y

        # Ensure the aircraft stays within the bounds
        new_x = max(0, min(new_x, 750))
        new_y = max(0, min(new_y, 550))

        print(f"Moving Aircraft {aircraft_name} to ({new_x}, {new_y})")

        # Move the aircraft to the new position
        aircraft.move_to(new_x, new_y)






def sim_update():
    move_aircraft_in_linear_pattern()