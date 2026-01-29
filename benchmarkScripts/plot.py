import matplotlib.pyplot as plt
import numpy as np
with open("foo") as f:
    data = f.read().splitlines()
    data = [float(i) for i in data]

# Define X and Y variable data
x = [i for i in range(len(data))]
y=np.array(data)
x2 = [elem * 6320 for elem in x]
size=15
plt.subplots_adjust(top=0.85)
plt.plot(x2, y)
plt.xticks(fontsize=size*0.5)
plt.yticks(fontsize=size*0.5)
plt.xlabel("Faces in the scene",fontsize=size)  # add X-axis label
plt.ylabel("frames per second",fontsize=size)  # add Y-axis label
plt.yscale("log")
plt.twiny()
plt.plot(x,y)
plt.xticks(fontsize=size*0.5)
plt.xlabel("Number of objects",fontsize=size)
plt.title("FPS change with the number of objects",fontsize=size)
plt.yticks([1, 10, 100, 1000], ['1', '10', '100', '1000'],fontsize=size*0.5)
plt.savefig("plot.png")
# plt.show()
