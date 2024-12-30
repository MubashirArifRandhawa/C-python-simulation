import pygame
import random

# Initialize Pygame
pygame.init()

# Define window size
WIDTH, HEIGHT = 800, 600
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("Aircraft Simulation")

# Define colors
WHITE = (255, 255, 255)
BLUE = (0, 0, 255)
RED = (255, 0, 0)

# Aircraft class for Pygame visualization
class Aircraft:
    def __init__(self, name, force, health):
        self.name = name
        self.force = force
        self.health = health
        self.x = random.uniform(100, WIDTH - 100)  # Initial random position within the window
        self.y = random.uniform(100, HEIGHT - 100)
        self.color = BLUE if force == "Blue" else RED
        self.radius = 15  # Smaller size for aircraft
        self.speed = 2  # Adjust the speed of movement

    def draw(self, screen):
        pygame.draw.circle(screen, self.color, (int(self.x), int(self.y)), self.radius)

    def move_to(self, x, y):
        # Move the aircraft to a target position gradually, instead of jumping
        direction_x = x - self.x
        direction_y = y - self.y
        distance = (direction_x ** 2 + direction_y ** 2) ** 0.5
        if distance > self.speed:
            self.x += direction_x / distance * self.speed
            self.y += direction_y / distance * self.speed
        else:
            self.x = x
            self.y = y

    def attack(self, target):
        damage = 10
        target.health -= damage

    def is_alive(self):
        return self.health > 0

# Create aircraft objects
aircraft1 = Aircraft("Fighter1", "Blue", 100)
aircraft2 = Aircraft("Bomber1", "Red", 100)

# Simulation loop
running = True
while running:
    screen.fill(WHITE)
    
    # Draw aircraft
    aircraft1.draw(screen)
    aircraft2.draw(screen)
    
    # Move aircrafts randomly but slower
    aircraft1.move_to(random.uniform(100, WIDTH - 100), random.uniform(100, HEIGHT - 100))
    aircraft2.move_to(random.uniform(100, WIDTH - 100), random.uniform(100, HEIGHT - 100))
    
    # Simple attack simulation with a random chance
    if random.random() > 0.95:  # Random chance to attack
        if random.choice([True, False]):
            aircraft1.attack(aircraft2)
        else:
            aircraft2.attack(aircraft1)
    
    # Check if any aircraft is destroyed
    if not aircraft1.is_alive() or not aircraft2.is_alive():
        print(f"Simulation ended. {aircraft1.name if not aircraft1.is_alive() else aircraft2.name} is destroyed.")
        running = False
    
    # Update the display
    pygame.display.flip()
    
    # Control the frame rate
    pygame.time.Clock().tick(30)

# Quit Pygame
pygame.quit()
