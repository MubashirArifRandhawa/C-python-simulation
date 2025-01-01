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


def sim_update():
    print("sim_update() is called .. ..")
    pass

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
    