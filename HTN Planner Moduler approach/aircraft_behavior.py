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

   # aircrafts = simulation.get_aircrafts()
   #
   # # Initialize positions
   # red_lon = 0
   # blue_lon = 0
   # lat = 0  # Fixed: Define `lat`
   #
   # for aircraft in aircrafts:
   #     if aircraft.get_force() == "Red":
   #         red_lon -= 10  # Increment the position for red aircraft
   #         lon = red_lon  # Use the red longitude
   #     else:  # Assuming any non-Red aircraft is Blue
   #         blue_lon += 10  # Increment the position for blue aircraft
   #         lon = blue_lon  # Use the blue longitude
   #
   #     aircraft.move_to(lat, lon)  # Move the aircraft
   #     aircraft_name = aircraft.get_name()  # Get the name of the aircraft
   #     print(f"Moving Aircraft {aircraft_name} to ({lat}, {lon}) in lat/lon")



    

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