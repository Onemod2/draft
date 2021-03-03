from PIL import Image, ImageTk


class Map:
    def __init__(self, path, width_first: int = 32, height_first: int = 32
                     , width_resize: int = 32, height_resize: int = 32):
        self.path = path

        self.bitmap = \
       [
        "111111111",
        "100000001",
        "100000201",
        "101000201",
        "101000201",
        "101000201",
        "101000201",
        "100000201",
        "101000001",
        "111111111"
       ]

        self.width_first = width_first
        self.height_first = height_first

        self.width_resize = width_resize
        self.height_resize = height_resize

        self.images = []
        self.tk_images = []

        self.create_images()

    def get_pict_block_coord(self, i, j):
        if self.bitmap[i][j] == '0':
            left = 2 * self.width_first
            top = 2 * self.height_first
            right = 3 * self.width_first
            bottom = 3 * self.height_first

            return left, top, right, bottom
        elif self.bitmap[i][j] == '1':
            left = 0 * self.width_first
            top = 3 * self.height_first
            right = 1 * self.width_first
            bottom = 4 * self.height_first

            return left, top, right, bottom
        else:
            left = 3 * self.width_first
            top = 1 * self.height_first
            right = 4 * self.width_first
            bottom = 2 * self.height_first

            return left, top, right, bottom

    def get_block_coord(self, i, j):
        return self.width_resize*j, self.height_resize*i

    def get_image(self, i, j):
        return self.tk_images[i][j]

    def create_images(self):
        for i in range(len(self.bitmap)):
            self.images.append([])
            self.tk_images.append([])
            for j in range(len(self.bitmap[-1])):
                left, top, right, bottom = self.get_pict_block_coord(i, j)

                self.images[-1].append(Image.open(self.path).crop((left, top, right, bottom))
                                       .resize((self.width_resize, self.height_resize)))

                self.tk_images[-1].append(ImageTk.PhotoImage(self.images[-1][-1]))
