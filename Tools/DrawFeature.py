__author__ = 'lyx'
from PIL import ImageDraw
from RawFile import Image
import sys


def decode(feature):
    m = max(92, 112) // 4 + 1
    h = (feature % m) * 4
    feature //= m
    w = (feature % m) * 4
    feature //= m
    y = (feature % m) * 4
    feature //= m
    x = (feature % m) * 4
    feature //= m
    t = feature % m
    return x, y, w, h, t


def draw(p, detail):
    x, y, w, h, t = detail
    if t == 0:
        rect = (x, y, x + 2 * w - 1, y + h - 1)
    if t == 1:
        rect = (x, y, x + w - 1, y + 2 * h - 1)
    if t == 2:
        rect = (x, y, x + 3 * w - 1, y + h - 1)
    if t == 3:
        rect = (x, y, x + 2 * w - 1, y + 2 * h - 1)
    p.rectangle(rect, fill=220, outline=200)

features = []
with open("../adaboost.pos", 'r') as file:
    for line in file:
        features.append(tuple(map(int, line.split('\t')[0:3])))
image = Image(112, 92, sys.argv[1])
img = image.draw()
paint = ImageDraw.Draw(img)
for feature in features:
    detail = decode(feature[1])
    print(detail)
    draw(paint, detail)
img.show()
