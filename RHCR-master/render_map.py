import matplotlib.pyplot as plt
import numpy as np
import os

map_file = "c:/Users/raoni/OneDrive/Desktop/GitHub - Copy/MAPF-with-multi-level-architecture/RHCR-master/kiva_shaved.map"
output_file = "c:/Users/raoni/OneDrive/Desktop/GitHub - Copy/MAPF-with-multi-level-architecture/RHCR-master/kiva_shaved_preview.png"

with open(map_file, 'r') as f:
    lines = f.read().splitlines()

dimensions = lines[0].strip().split(',')
height = int(dimensions[0])
width = int(dimensions[1])

map_lines = lines[4:]

# Create background image
background = np.ones((height, width, 3))

for y in range(height):
    if y >= len(map_lines): break
    for x in range(width):
        if x >= len(map_lines[y]): break
        cell = map_lines[y][x]
        if cell == '@':
            background[y, x] = [0.6, 0.4, 0.2]  # Brown
        elif cell == 'e':
            background[y, x] = [1.0, 0.6, 0.0]  # Orange 
        elif cell == 'r':
            background[y, x] = [0.7, 0.8, 1.0]  # Light blue
        elif cell == '.':
            background[y, x] = [1.0, 1.0, 1.0]

fig, ax = plt.subplots(figsize=(15, 10))
ax.set_title("Corner-Shaved Kiva Map - Intersection Plazas", fontsize=16, fontweight='bold')
ax.imshow(background, aspect='equal')
ax.set_xticks([])
ax.set_yticks([])

plt.tight_layout()
plt.savefig(output_file, dpi=150, bbox_inches='tight')
print(f"Saved preview to {output_file}")
