__author__ = 'lyx'
from PIL import Image


path = "../photo/photo3"
extend = ".jpg"
img = Image.open(path + extend).convert('L')
print(img)
data = list(img.getdata())
with open(path + '.raw', "wb") as file:
    file.write(bytes(data))