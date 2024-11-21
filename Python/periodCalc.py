"""author@ Henry Centeno
purpose: for lab 2 of module 4 for ef 152 to calculate the period of a physical pendulum at various lengths from the center of mass.
"""

import math

MASS = .07 #mass of the pendulum

G = 9.81 #rate of gravity on Earth

inertia = float(input("enter mass moment of Inertia (Icm): ")) #calulated on paper

r = float(input("enter distance from center (cm): ")) 
r = r/ 100 #convert to meters

period = 2 * math.pi * math.sqrt(inertia / (MASS * G * r)) #taken from formula to find the period of a physical pendulum

print(period)
