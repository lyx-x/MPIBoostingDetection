__author__ = 'lyx'
from PIL import Image


class Img:
    pixels = []
    height = 92
    width = 112
    path = ""

    def __init__(self, w, h, path):
        self.height = h
        self.width = w
        self.pixels = []
        self.path = path
        with open(path, "rb") as file:
            for i in range(self.height):
                line = []
                for j in range(self.width):
                    byte = file.read(1)
                    line.append(byte[0])
                self.pixels.append(line)

    def __str__(self):
        content = self.path + '\n'
        for line in self.pixels:
            content += '\t'.join(str(pixel) for pixel in line)
            content += '\n'
        return content

    def transpose(self):
        with open(self.path + ".old", "wb") as file:
            for line in self.pixels:
                file.write(bytes(line))
        with open(self.path, "wb") as file:
            for i in range(self.width):
                for j in range(self.height):
                    file.write(bytes([self.pixels[j][i]]))

    def show(self):
        im = Image.new('L', (self.width, self.height))
        data = []
        for line in self.pixels:
            data += line
        im.putdata(data)
        im.show()

    def draw(self):
        im = Image.new('L', (self.width, self.height))
        data = []
        for line in self.pixels:
            data += line
        im.putdata(data)
        return im


def transpose(files):
    for path in files:
        img = Img(92, 112, path)
        img.transpose()


def verify(file):
    img = Img(112, 92, file)
    img.show()
    img = Img(92, 112, file + ".old")
    img.show()


def read(file):
    img = Img(3636, 2425, file)
    img.show()

#read("IMG_8243.raw")