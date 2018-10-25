#!/usr/bin/env python3

import trebuchet


def main():
    recognizer = trebuchet.GestureRecognizer()
    recognizer.start()
    while True:
        for gesture, event in recognizer.update():
            tp_ids = ", ".join([str(tp.id) for tp in gesture.touch_points])
            print(event, type(gesture), "<{}>".format(tp_ids))


if __name__ == '__main__':
    main()
