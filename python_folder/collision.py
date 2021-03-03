import map
import charter
import tkinter as tk


def is_collision(current_char: charter.Char, current_map: map.Map, current_canvas: tk.Canvas):
    c_x, c_y = current_canvas.coords(current_char.id)

    c_x = [c_x, c_x+current_char.width]
    c_y = [c_y, c_y+current_char.height]

    for x in c_x:
        for y in c_y:
            i = int(y / current_map.height_resize)
            j = int(x / current_map.width_resize)

            max_i = len(current_map.bitmap)
            max_j = len(current_map.bitmap[0])

            if (max_i < i or i < 0 or max_j < j or j < 0) or \
               (current_map.bitmap[i][j] == '1') or (current_map.bitmap[i][j] == '2'):

                dx, dy = current_char.change_by()
                current_canvas.move(current_char.get_id(), -dx, -dy)

                return True

    return False

