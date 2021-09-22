# coding: UTF-8
"""
Created on 2021/9/21

@author: Mark Hsu
"""

import os
import re
import matplotlib.pyplot as plt
import matplotlib
import pandas as pd


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


def d4(x, y, z, u):
    matplotlib.use('macosx')
    fig = plt.figure()
    ax = plt.axes(projection="3d")
    img = ax.scatter(x, y, z, c=u, cmap=plt.jet(), alpha=0.7, marker='x')
    ax.set_xlabel(r'$\theta_u$')
    ax.set_ylabel(r'$\theta_l$')
    ax.set_zlabel('Times')
    fig.colorbar(img, pad=0.1)
    fig.tight_layout()
    plt.show()


def d3(x, y, z):
    matplotlib.use('macosx')
    fig = plt.figure()
    ax = plt.axes(projection="3d")
    img = ax.scatter(x, y, z, c=z, cmap=plt.jet(), alpha=0.7, marker='x')
    ax.set_xlabel(r'$\theta_u$')
    ax.set_ylabel(r'$\theta_l$')
    ax.set_zlabel('Trend ratio')
    fig.colorbar(img, pad=0.1)
    fig.tight_layout()
    plt.show()


def d2(x, y, z):
    fig = plt.figure()
    ax = fig.add_subplot()
    ax.set_xlabel(r'$\theta_u$')
    ax.set_ylabel(r'$\theta_l$')
    ax.grid(True, linestyle='-', color='0.75')
    ax.scatter(x, y, c=z, cmap=plt.jet(), alpha=0.7, marker='x')
    fig.tight_layout()
    plt.show()


def draw():
    mode = 2

    df = pd.read_csv('./py_output/fine-tune.csv')
    x = df['theta_u'].to_numpy()
    y = df['theta_l'].to_numpy()
    z = df['Times'].to_numpy()
    u = df['trend ratio'].to_numpy()

    if mode == 2:
        d2(x, y, u)
    elif mode == 3:
        d3(x, y, u)
    elif mode == 4:
        d4(x, y, z, u)


def analyze():
    file_paths = get_files()
    with open('./py_output/fine-tune.csv', 'w') as writer:
        writer.write('Times,theta_u,theta_l,trend ratio\n')
        for fp in file_paths:
            tr = get_tr(fp)
            fns = re.search(r'(\d+)times_(0\.\d*)_(0\.\d*)', fp)
            times, upper_bound, lower_bound = fns.groups()
            writer.write(times)
            writer.write(',')
            writer.write(upper_bound)
            writer.write(',')
            writer.write(lower_bound)
            writer.write(',')
            writer.write(tr)
            writer.write('\n')


if __name__ == '__main__':
    analyze()
    draw()
