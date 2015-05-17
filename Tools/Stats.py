__author__ = 'lyx'
import matplotlib.pyplot as plt

coords = []
correct = []
fauxNeg = []
fauxPos = []
with open("../adaboost.theta", 'r') as file:
    for line in file:
        result = tuple(map(float, line.split('\t')))
        coords.append((result[3], result[2]))
        correct.append((result[0], result[1]))
        fauxNeg.append((result[0], result[2]))
        fauxPos.append((result[0], result[3]))
plt.scatter(*zip(*coords))
plt.xlabel("Taux de faux positifs", fontsize=20)
plt.ylabel("Taux de faux négatifs", fontsize=20)
plt.show()
plt.scatter(*zip(*correct))
plt.ylabel("Taux de réussite", fontsize=20)
plt.xlabel("Theta", fontsize=20)
plt.show()