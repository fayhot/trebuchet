#!/usr/bin/env python3

import trebuchet


def main():
    app = trebuchet.Application()
    app.screen_resolution = (3840, 2160)
    app.start()
    while True:
        for gesture, event in app.update():
            print(event, gesture)


if __name__ == '__main__':
    main()
