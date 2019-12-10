import numpy as np; np.random.seed(0)
import seaborn as sns; sns.set()
import matplotlib.pyplot as plt

data = np.loadtxt('result.txt')
#plt.imshow(data, cmap='hot', interpolation='nearest')
ax = sns.heatmap(data, cmap="YlGnBu")
plt.show()