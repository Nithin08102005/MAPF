import sys
import os
import numpy as np

def shave_corners(input_map, output_map):
    with open(input_map, 'r') as f:
        lines = f.read().splitlines()

    print(f"Reading {input_map}...")
    dimensions = lines[0].strip().split(',')
    height = int(dimensions[0])
    width = int(dimensions[1])

    grid = []
    for row in lines[4:]:
        grid.append(list(row))
        
    grid = np.array(grid)
    new_grid = np.copy(grid)

    def is_free(y, x):
        return 0 <= y < height and 0 <= x < width and grid[y, x] in ['.', 'e', 'r']

    for y in range(height):
        for x in range(width):
            if grid[y, x] == '.':
                # Check for crossroads (horizontal AND vertical paths meeting)
                has_vertical = is_free(y-1, x) or is_free(y+1, x)
                has_horizontal = is_free(y, x-1) or is_free(y, x+1)
                
                # If it's a junction (or a pure corner), shave the 3x3 box around it
                if has_vertical and has_horizontal:
                    for dy in [-1, 0, 1]:
                        for dx in [-1, 0, 1]:
                            ny, nx = y + dy, x + dx
                            if 0 <= ny < height and 0 <= nx < width:
                                if grid[ny, nx] == '@':
                                    new_grid[ny, nx] = '.'

    with open(output_map, 'w') as f:
        f.write(lines[0] + "\n")
        f.write(lines[1] + "\n")
        f.write(lines[2] + "\n")
        f.write(lines[3] + "\n")
        for row in new_grid:
            f.write("".join(row) + "\n")
            
    print(f"Successfully shaved corners and exported to {output_map}")

if __name__ == "__main__":
    shave_corners(
        "c:/Users/raoni/OneDrive/Desktop/GitHub - Copy/MAPF-with-multi-level-architecture/RHCR-master/kiva.map",
        "c:/Users/raoni/OneDrive/Desktop/GitHub - Copy/MAPF-with-multi-level-architecture/RHCR-master/kiva_shaved.map"
    )
