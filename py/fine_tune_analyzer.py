# coding: UTF-8
"""
Created on 2021/9/21

@author: Mark Hsu
"""

import os
import re


def get_files():
    root = os.path.realpath('/Users/Mark/Desktop/Fine-tune/')
    file_paths = []

    outer_dirs = os.listdir(root)
    for outer in outer_dirs:
        p1 = os.path.join(root, outer)
        if not os.path.isdir(p1):
            continue
        inner_dirs = os.listdir(p1)
        for inner in inner_dirs:
            p2 = os.path.join(p1, inner)
            if not os.path.isdir(p2):
                continue
            sws = os.listdir(p2)
            for sw in sws:
                p3 = os.path.join(p2, sw)
                if not os.path.isdir(p3):
                    continue

                files = os.listdir(p3)

                for file in files:
                    if 'final_result.csv' in file:
                        file_paths.append(os.path.join(p3, file))
    return file_paths


def get_tr(fp):
    with open(fp, 'r') as reader:
        return reader.readline().split(',')[4]


if __name__ == '__main__':
    file_paths = get_files()
    with open('./py_output/fine-tune.csv', 'w') as writer:
        writer.write('Times,theta_u,theta_l,trend ratio\n')
        for fp in file_paths:
            tr = get_tr(fp)
            fns = re.match(r'.*(1\d+)times_(0\.\d*1)_(0\.\d*1)_', fp)
            times, upper_bound, lower_bound = fns.groups()
            writer.write(times)
            writer.write(',')
            writer.write(upper_bound)
            writer.write(',')
            writer.write(lower_bound)
            writer.write(',')
            writer.write(tr)
            writer.write('\n')
