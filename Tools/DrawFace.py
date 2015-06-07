__author__ = 'lyx'
from PIL import ImageDraw
from RawFile import Img

number = 3
faces = []
width = 544
height = 432
with open("../photo/photo{0}.raw.res".format(number), 'r') as file:
    for line in file:
        faces.append(tuple(map(int, line.split('\t'))))
image = Img(width, height, "../photo/photo{0}.raw".format(number))
img = image.draw()
paint = ImageDraw.Draw(img)
for face in faces:
    for i in range(2):
        rect = (face[0] + 2 * i, face[1] + 2 * i, face[0] + face[2] - 2 * i * 2, face[1] + face[3] - 2 * i * 2)
        paint.rectangle(rect, outline=250)
        rect = (face[0] + 2 * i + 1, face[1] + 2 * i + 1, face[0] + face[2] - 2 * i * 2 - 2, face[1] + face[3] - 2 * i * 2 - 2)
        paint.rectangle(rect, outline=50)
img.show()
img.save("../photo/photo{0}.result.jpg".format(number))
