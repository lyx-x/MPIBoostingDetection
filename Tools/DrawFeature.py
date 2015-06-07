__author__ = 'lyx'
from PIL import ImageDraw, Image
from RawFile import Img
import sys

inc = 8;

def decode(feature):
    m = max(92, 112) // inc + 1
    h = (feature % m) * inc
    feature //= m
    w = (feature % m) * inc
    feature //= m
    y = (feature % m) * inc
    feature //= m
    x = (feature % m) * inc
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


def drawface():
    features = []
    with open("adaboost.jrl", 'r') as file:
        for line in file:
            features.append(tuple(map(int, line.split('\t')[0:2])))
    image = Img(112, 92, "im0.raw")
    img = image.draw()
    paint = ImageDraw.Draw(img)
    for feature in features:
        detail = decode(feature[1])
        print(detail)
        draw(paint, detail)
    img.show()
    img.save("adaboost3.jpg")


def features():
    image = Image.new('L', (478, 92))
    paint = ImageDraw.Draw(image)
    for i in range(4):
        rect = (i * 122, 0, 112 + i * 122 - 1, 92 - 1)
        print(rect)
        paint.rectangle(rect, outline=125, fill=255)

    rect = (112, 0, 121, 91)
    paint.rectangle(rect, outline=255, fill=255)
    rect = (234, 0, 243, 91)
    paint.rectangle(rect, outline=255, fill=255)
    rect = (356, 0, 365, 91)
    paint.rectangle(rect, outline=255, fill=255)

    rect = (20, 20, 39, 49)
    paint.rectangle(rect, outline=125, fill=255)
    rect = (40, 20, 59, 49)
    paint.rectangle(rect, outline=125, fill=0)

    rect = (142, 20, 161, 49)
    paint.rectangle(rect, outline=125, fill=255)
    rect = (142, 50, 161, 79)
    paint.rectangle(rect, outline=125, fill=0)

    rect = (264, 20, 283, 49)
    paint.rectangle(rect, outline=125, fill=255)
    rect = (284, 20, 303, 49)
    paint.rectangle(rect, outline=125, fill=0)
    rect = (304, 20, 323, 49)
    paint.rectangle(rect, outline=125, fill=255)

    rect = (386, 20, 405, 49)
    paint.rectangle(rect, outline=125, fill=255)
    rect = (406, 20, 425, 49)
    paint.rectangle(rect, outline=125, fill=0)
    rect = (386, 50, 405, 79)
    paint.rectangle(rect, outline=125, fill=0)
    rect = (406, 50, 425, 79)
    paint.rectangle(rect, outline=125, fill=255)

    image.show()
    image.save("features.png")

drawface()
