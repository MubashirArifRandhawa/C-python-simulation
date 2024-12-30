import aircraft_simulation
# import time
# import random
# 
# simulation = None  # Global variable to store the simulation object
# 
# def set_simulation(sim):
#     global simulation
#     simulation = sim
#     print("Simulation object set in Python!")
# 
# def control_aircrafts():
#     if simulation is None:
#         print("Simulation is not set!")
#         return
# 
#     # Move aircraft continuously in a loop with different paths
#     while True:
#         aircrafts = simulation.get_aircrafts()
#         
#         for aircraft in aircrafts:
#             # Generate random new position for each aircraft
#             new_x = random.randint(50, 750)
#             new_y = random.randint(50, 550)
# 
#             print(f"Moving Aircraft {aircraft.get_name()} to ({new_x}, {new_y})")
#             aircraft.move_to(new_x, new_y)  # Move aircraft to new random location
# 
#         # Update simulation state
#         simulation.update_aircrafts()
#         
#         # Sleep for a short time before next movement
#         time.sleep(1)  # Adjust the speed of movement by changing the sleep duration
# 
# # Example of running the control
# control_aircrafts()


import threading
import time
import random

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

def move_aircraft_in_linear_pattern():
    if simulation is None:
        print("Simulation is not set!")
        return

    # Perform a single update for aircrafts
    aircrafts = simulation.get_aircrafts()
    for aircraft in aircrafts:
        # Get the current position of the aircraft
        current_x = aircraft.get_x()
        current_y = aircraft.get_y()

        # Movement speeds (small steps for smooth movement)
        delta_x = 1  # Default small movement in the x direction (right)
        delta_y = 0  # No movement in the y direction initially

        # First, handle horizontal movement
        if current_x >= 750:  # If the aircraft reaches the right edge
            delta_x = -1  # Move left
        elif current_x <= 5:  # If the aircraft reaches the left edge
            delta_x = 1  # Move right

        # Once the aircraft has reached either edge (x limits), start moving vertically
        if current_x == 5 or current_x == 750:
            if current_y >= 550:  # If the aircraft reaches the bottom edge
                delta_y = -1  # Move up
            elif current_y <= 5:  # If the aircraft reaches the top edge
                delta_y = 1  # Move down

        # Update the aircraft's position based on the calculated deltas
        new_x = current_x + delta_x
        new_y = current_y + delta_y

        # Ensure the aircraft stays within the bounds of the screen
        new_x = max(0, min(new_x, 750))  # Bound the x position between 0 and 750
        new_y = max(0, min(new_y, 550))  # Bound the y position between 0 and 550

        print(f"Moving Aircraft {aircraft.get_name()} to ({new_x}, {new_y})")

        # Move the aircraft to the new position
        aircraft.move_to(new_x, new_y)




def sim_update():
    move_aircraft_in_linear_pattern()