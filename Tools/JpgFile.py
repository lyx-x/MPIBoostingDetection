__author__ = 'lyx'
from PIL import Image as Img

img = Img.open("im0.jpg")
img = img.resize((92, 112), Img.ANTIALIAS)
img.show()
