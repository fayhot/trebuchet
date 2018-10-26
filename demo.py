#!/usr/bin/env python3

import trebuchet


def main():
    recognizer = trebuchet.GestureRecognizer()
    recognizer.start()
    while True:
        for gesture, event in recognizer.update():
            print(event, gesture)


if __name__ == '__main__':
    main()
