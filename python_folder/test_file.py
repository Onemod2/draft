import tkinter as tk
import socket
import pickle
import copy

import charter
import map
import collision


RESIZE_SIDE = 64

WIDTH = RESIZE_SIDE * 9
HEIGHT = RESIZE_SIDE * 10

MIN_SPEED = 0
MAX_SPEED = 50

STD_SPEED = 2

SERVER_IP = "192.168.31.77"
SERVER_PORT = 24


def get_ip_by_server(char, ip=SERVER_IP, port=SERVER_PORT):
    with socket.create_connection((ip, port)) as s:
        s.send(pickle.dumps(char))

        data = s.recv(1024)
        my_ip = data.decode().strip()

        return my_ip


class MainWindow(tk.Tk):
    def __init__(self, width: int, height: int):
        super().__init__()

        self.coord = {}
        self.ids_of_another_chars = []
        self.geometry = f"{width}x{height}"

        w, h = self.geometry.split('x')

        self.canvas = tk.Canvas(self, width=width, height=height)

        self.current_map = map.Map("map.png", 32, 32, RESIZE_SIDE, RESIZE_SIDE)
        self.map_ids = []
        self.char = charter.Char("1.png", int(w)/2, int(h)/2)

        self.my_ip = get_ip_by_server(self.char)

        self.create_map()

        char_id = self.char_add_to_canv(self.char)
        self.char.set_id(char_id)

        self.animation()
        self.paint()

    def char_add_to_canv(self, char):
        char_id = self.canvas.create_image(char.get_x(), char.get_y(),
                                           image=char.new_picture(),
                                           anchor='nw')
        return char_id

    def get_coord(self, ip=SERVER_IP, port=SERVER_PORT):
        with socket.create_connection((ip, port)) as s:

            current_char = self.char.copy_obj()
            s.send(pickle.dumps(current_char))

            data = s.recv(1024)
        self.coord = pickle.loads(data)
        del(self.coord[self.my_ip])

    def animation_another_player(self):
        self.get_coord()
        self.add_chars_to_cnv()

    def add_chars_to_cnv(self):
        dif = len(self.ids_of_another_chars) - len(self.coord)
        for i in range(dif):
            self.canvas.delete(self.ids_of_another_chars.pop(-1))

        for i in range(len(self.ids_of_another_chars)):
            current_id = self.ids_of_another_chars[i]
            current_char = self.coord[i][1]
            self.change_canvas_obj(current_id, current_char)

        start = len(self.ids_of_another_chars)
        for i in range(start, len(self.coord)):
            current_char = self.coord[i][1]
            current_id = self.char_add_to_canv(current_char)
            self.ids_of_another_chars.append(current_id)

    def change_canvas_obj(self, id, char):
        self.update_picture(char, id, char.get_x(), char.get_y())

    def animation(self):
        self.canvas_animation()

        self.after(125, self.animation)

    def canvas_animation(self):
        d_x, d_y = self.char.change_by()
        self.char.change_coord_by(d_x, d_y)

        self.canvas.move(self.char.get_id(), d_x, d_y)
        collision.is_collision(self.char, self.current_map, self.canvas)
        self.update_picture(self.char)

        self.animation_another_player()
        self.canvas.update()

    def create_map(self):
        for i in range(len(self.current_map.bitmap)):
            self.map_ids.append([])
            for j in range(len(self.current_map.bitmap[0])):
                x_coord, y_coord = self.current_map.get_block_coord(i, j)
                self.map_ids[-1].append(self.canvas.create_image(
                                        x_coord, y_coord, image=self.current_map.get_image(i, j), anchor='nw'))

    def paint(self):
        self.canvas.grid()

    def move_binds(self):
        self.bind("<Down>", self.move_down)
        self.bind("<Left>", self.move_left)
        self.bind("<Right>", self.move_right)
        self.bind("<Up>", self.move_up)
        self.bind("s", self.move_stop)
        self.bind("n", self.speed)
        self.bind("b", self.speed)
        self.bind("<Button-1>", self.test)

    def test(self, event):
        print(event.x, event.y)

    def speed(self, event):
        if event.char == 'n':
            self.char.speed = min(self.char.speed+2, MAX_SPEED)
        else:
            self.char.speed = max(self.char.speed-2, MIN_SPEED)

    def move_down(self, event):
        self.char.set_dir(0)

    def move_left(self, event):
        self.char.set_dir(1)

    def move_right(self, event):
        self.char.set_dir(2)

    def move_up(self, event):
        self.char.set_dir(3)

    def move_stop(self, event):
        self.char.set_dir(-1)

    def update_picture(self, char, current_id=None, x=None, y=None):
        if current_id is None:
            current_id = char.get_id()

        if (x is None) and (y is None):
            self.canvas.itemconfigure(current_id, image=char.new_picture())
        else:
            self.canvas.itemconfigure(current_id, image=char.new_picture())
            self.canvas.coords(x, y)


if __name__ == "__main__":
    app = MainWindow(WIDTH, HEIGHT)
    app.move_binds()
    app.mainloop()
