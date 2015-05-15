__author__ = 'lyx'
from PIL import Image


path = "IMG_8243"
img = Image.open(path + '.PNG').convert('L')
print(img)
img.save(path + '.PNG')
data = list(img.getdata())
with open(path + '.raw', "wb") as file:
    file.write(bytes(data))