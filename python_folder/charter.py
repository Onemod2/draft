from PIL import Image, ImageTk


class Char:
    def __init__(self, path, x, y, width: int = 32, height: int = 48, count: int = 4):
        self.path = path
        self.x = x
        self.y = y
        self.width = width
        self.height = height
        self.count = count
        self.dir = 0
        self.speed = 2
        self.id = 0

        self.current_pict = -1
        self.image, self.tkImage = None, None

    def copy_obj(self):
        current_char = Char(self.path, self.x, self.y, self.width, self.height)
        current_char.dir = self.dir
        current_char.speed = self.speed
        current_char.current_pict = self.current_pict

        return current_char

    def new_picture(self):
        if self.dir == -1:
            self.image = Image.open(self.path).crop((0, self.dir*self.height,
                                                     self.width, (self.dir+1) * self.height))
            self.tkImage = ImageTk.PhotoImage(self.image)
        else:
            self.current_pict = (self.current_pict + 1) % self.count

            left = self.current_pict * 32
            top = self.dir * self.height
            right = (self.current_pict+1) * 32
            bottom = (self.dir+1) * self.height

            self.image = Image.open(self.path).crop((left, top,
                                                    right, bottom))

            self.tkImage = ImageTk.PhotoImage(self.image)

        return self.tkImage

    def set_dir(self, new_dir):
        self.current_pict = -1
        self.dir = new_dir

    def set_id(self, id):
        self.id = id

    def get_id(self):
        return self.id

    def set_speed(self, speed):
        self.speed = speed

    def change_coord_by(self, dx, dy):
        self.x += dx
        self.y += dy

    def change_by(self):
        if self.dir == 0:
            return 0, self.speed
        elif self.dir == 1:
            return -self.speed, 0
        elif self.dir == 2:
            return self.speed, 0
        elif self.dir == 3:
            return 0, -self.speed
        else:
            return 0, 0

    def get_x(self):
        return self.x

    def get_y(self):
        return self.y
