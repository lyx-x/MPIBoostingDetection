__author__ = 'lyx'
from PIL import ImageDraw
from RawFile import Img
import sys

faces = []
with open("../photo/photo1.raw.res", 'r') as file:
    for line in file:
        faces.append(tuple(map(int, line.split('\t'))))
image = Img(3636, 2425, sys.argv[1])
img = image.draw()
paint = ImageDraw.Draw(img)
for face in faces:
    for i in range(5):
        rect = (face[0] + 2 * i, face[1] + 2 * i, face[0] + face[2] - 2 * i * 2, face[1] + face[3] - 2 * i * 2)
        paint.rectangle(rect, outline=250)
        rect = (face[0] + 2 * i + 1, face[1] + 2 * i + 1, face[0] + face[2] - 2 * i * 2 - 2, face[1] + face[3] - 2 * i * 2 - 2)
        paint.rectangle(rect, outline=50)
img.show()
img.save("../photo/photo1.result.jpg")
