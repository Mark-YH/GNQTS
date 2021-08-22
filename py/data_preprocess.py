# coding: UTF-8
"""
Created on 2021/8/15

@author: Mark Hsu
"""
import os
from sliding_window import sws


def run():
    path = os.path.realpath('/Users/Mark/Downloads/2020-202106/')
    dirs = os.listdir(path)
    for dir in dirs:
        if dir in sws:
            src = os.path.join(path, dir)
            os.makedirs(os.path.join(src, 'train'), exist_ok=True)
            os.makedirs(os.path.join(src, 'test'), exist_ok=True)
            files = os.listdir(src)
            for file in files:
                if os.path.isdir(os.path.join(src, file)):
                    continue
                if str(file).__contains__('train'):
                    os.replace(os.path.join(src, file), os.path.join(src, 'train', file))
                elif str(file).__contains__('test'):
                    os.replace(os.path.join(src, file), os.path.join(src, 'test', file))
                elif file == '.DS_Store':
                    os.remove(os.path.join(src, file))
        elif dir == '.DS_Store':
            os.remove(os.path.join(path, dir))


if __name__ == '__main__':
    run()
