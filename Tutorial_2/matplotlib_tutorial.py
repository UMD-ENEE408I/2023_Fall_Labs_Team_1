import matplotlib as mpl
import matplotlib.pyplot as plt
from mpl_toolkits import mplot3d
import numpy as np
# Create a line plot of the sine function over the interval [0,2pi]
fig, ax = plt.subplots(figsize=(6,6))
x = np.linspace(0,2*np.pi)
y = np.sin(x)
ax.plot(x,y,label="Curve")
# Add labels to the axes
ax.set_xlabel("x")
ax.set_ylabel("y")
plt.show()
# Plot the 3d graph of the function z = sin(sqrt(x^2+y^2))
fig = plt.figure(figsize = (12,10))
ax = plt.axes(projection='3d')
x = np.arange(-4,4,0.1)
y = np.arange(-4,4,0.1)
X,Y = np.meshgrid(x,y)
Z = np.sin(np.sqrt(np.square(X)+np.square(Y)))
surf = ax.plot_surface(X,Y,Z,cmap=plt.cm.cividis)
fig.colorbar(surf, shrink=0.5, aspect=8)
plt.show()
