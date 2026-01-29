import matplotlib.pyplot as plt
import numpy as np
with open("foo") as f:
    data = f.read().splitlines()
    data = [row.split(" ") for row in data]
    sizes = [int(row[0]) for row in data]
    data = [float(row[1]) for row in data]

x = sizes
y = np.array(data)
x2 = [640 ,1600 ,2480 ,4160 ,5520 ,7920 ,9760 ,12880 ,15200 ,19040 ,21840 ,26400 ,29680 ,34960 ,38720 ,44720 ,48960 ,55680 ,60400 ,67840 ,73040 ,81200 ,86880 ,95760 ,101920 ,111520 ,118160 ,128480 ,135600 ,146640 ,154240 ,166000 ,174080 ,186560 ,195120 ,208320 ,217360 ,231280]
# x2 = [elem * 6320 for elem in x]
size=15
plt.subplots_adjust(top=0.85)
plt.plot(x, y)
plt.xticks(fontsize=size*0.5)
plt.yticks(fontsize=size*0.5)
plt.xlabel("graph width/height",fontsize=size)  # add X-axis label
plt.ylabel("frames per second",fontsize=size)  # add Y-axis label
plt.yscale("log")
plt.xticks(fontsize=size*0.5)
plt.title("FPS of an animated graph",fontsize=size)
plt.yticks([1, 10, 100, 1000], ['1', '10', '100', '1000'],fontsize=size*0.5)
plt.savefig("plot.png")
# plt.show()
