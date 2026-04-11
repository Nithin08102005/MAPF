import matplotlib.pyplot as plt
import numpy as np

# Agents
agents = np.array([20, 30, 40, 50])

# Values at w = 50 (multiplied by 1000)
capacity = np.array([1.22, 1.74, 2.16, 2.71]) * 1000
normal   = np.array([0.85, 1.165, 1.556, 1.923]) * 1000

plt.figure(figsize=(7,5))

plt.plot(agents, capacity, marker='o', lw=2.5, color='crimson', label='Capacity')
plt.plot(agents, normal, marker='s', lw=2.5, color='royalblue', label='Normal')

plt.xlabel("Number of Agents (k)", fontsize=12)
plt.ylabel("Tasks Completed", fontsize=12)
plt.title("Simulation Window = 1000, Planning Window w = 50", fontsize=13)
plt.grid(True, linestyle='--', alpha=0.6)
plt.legend()
plt.tight_layout()
plt.show()
