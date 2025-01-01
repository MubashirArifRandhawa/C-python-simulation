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

    # Perform a single update for aircrafts
    aircrafts = simulation.get_aircrafts()
    lat = 70
    lon = 120
    for aircraft in aircrafts:
        # Get the unique name of the aircraft to track its deltas
        aircraft_name = aircraft.get_name()
        aircraft.move_to(lat, lon)
        print(f"Moving Aircraft {aircraft_name} to ({lat}, {lon}) in lat/lon")
        lat = -70
        lon = -120
    

# Track the last update time
last_update_time = time.time()

def sim_update():
    print("sim_update() is called...")
    if simulation is None:
        print("Simulation is not set!")
        return

    # Perform a single update for aircrafts
    aircrafts = simulation.get_aircrafts()

    # Define target locations for each aircraft
    target_locations = [
        (70, -120),   # Target for the first aircraft
        (-70, 120)  # Target for the second aircraft
    ]

    # Ensure the number of target locations matches the number of aircrafts
    if len(aircrafts) > len(target_locations):
        print("Not enough target locations for all aircraft. Assigning default (0, 0).")
        target_locations.extend([(0, 0)] * (len(aircrafts) - len(target_locations)))

    # Check if 10 seconds have passed
    global last_update_time
    current_time = time.time()
    if current_time - last_update_time >= 10:
        print("10 seconds passed, updating aircraft locations.")
        for aircraft, target in zip(aircrafts, target_locations):
            aircraft_name = aircraft.get_name()
            lat, lon = target
            aircraft.move_to(lat, lon)
            print(f"Moving Aircraft {aircraft_name} to ({lat}, {lon}) in lat/lon")

        # Reset the last update time
        last_update_time = current_time
    else:
        print("10 seconds have not yet passed since the last update.")