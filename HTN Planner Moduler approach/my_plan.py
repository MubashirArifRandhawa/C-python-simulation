from wrapper import PyAircraft, PyAttackPlanner, PyDefendPlanner
import random
import time

# Create aircraft objects
aircraft1 = PyAircraft("Fighter1", "Blue", 100)
aircraft2 = PyAircraft("Bomber1", "Red", 100)

# Create planners
attack_planner = PyAttackPlanner()
defend_planner = PyDefendPlanner()

# Function to execute the tasks for attack behavior
def execute_attack_plan(aircraft, target):
    attack_plan = attack_planner.plan(aircraft)
    print(f"Attack Plan for {aircraft.get_name()}: {attack_plan}")

    # Simulate execution of the attack plan
    for task in attack_plan:
        print(f"Executing task: {task}")
        if task == "Identify target":
            print(f"{aircraft.get_name()} is identifying target: {target.get_name()}")
        elif task == "Move to target":
            # Simulating a move to the target's position
            target_position = (random.uniform(0, 100), random.uniform(0, 100), random.uniform(0, 100))
            aircraft.move_to(*target_position)
        elif task == "Attack":
            aircraft.attack(target)
            time.sleep(1)  # Simulating the time it takes to attack

# Function to execute the tasks for defend behavior
def execute_defend_plan(aircraft):
    defend_plan = defend_planner.plan(aircraft)
    print(f"Defend Plan for {aircraft.get_name()}: {defend_plan}")

    # Simulate execution of the defend plan
    for task in defend_plan:
        print(f"Executing task: {task}")
        if task == "Identify threats":
            print(f"{aircraft.get_name()} is identifying threats")
        elif task == "Reposition to defensive location":
            # Simulating defensive repositioning
            defensive_position = (random.uniform(0, 100), random.uniform(0, 100), random.uniform(0, 100))
            aircraft.move_to(*defensive_position)
        elif task == "Defend":
            aircraft.defend()
            time.sleep(1)  # Simulating the time it takes to defend

# Simulate a battle between two aircraft
while aircraft1.get_health() > 0 and aircraft2.get_health() > 0:
    if random.choice([True, False]):  # Randomly decide who acts first
        execute_attack_plan(aircraft1, aircraft2)
        if aircraft2.get_health() <= 0:
            print(f"{aircraft2.get_name()} has been destroyed!")
            break
        execute_defend_plan(aircraft2)
    else:
        execute_attack_plan(aircraft2, aircraft1)
        if aircraft1.get_health() <= 0:
            print(f"{aircraft1.get_name()} has been destroyed!")
            break
        execute_defend_plan(aircraft1)
