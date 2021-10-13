# coding: UTF-8
"""
Created on 2021/9/21

@author: Mark Hsu
"""

import os
import re
import matplotlib.pyplot as plt
import matplotlib.patches as patches
import matplotlib
import pandas as pd
from multiprocessing import Process

matplotlib.use('macosx')
title = 'Test 1$-$4'


def get_files():
    root = os.path.realpath('/Users/Mark/Desktop/Fine-tune/Fine-tune experimental results/Fine-tune_Test1+2+3+4')
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
    fig = plt.figure(figsize=(7.2, 4.8))
    plt.rcParams['font.family'] = "Times New Roman"
    ax = plt.axes(projection="3d")
    img = ax.scatter(x, y, z, c=u, cmap=plt.jet(), alpha=0.7, marker='x')
    ax.set_title(title, fontsize=14)
    ax.set_xlabel(r'$\theta_u$', fontsize=12)
    ax.set_ylabel(r'$\theta_l$', fontsize=12)
    ax.set_zlabel('Times', fontsize=12)
    fig.colorbar(img, pad=0.1)
    fig.tight_layout()
    plt.savefig('./py_output/' + title + '_3D-2.svg')
    plt.savefig('./py_output/' + title + '_3D-2.pdf')
    plt.show()


def d3(x, y, z):
    fig = plt.figure(figsize=(7.2, 4.8))
    plt.rcParams['font.family'] = "Times New Roman"
    ax = plt.axes(projection="3d")
    img = ax.scatter(x, y, z, c=z, cmap=plt.jet(), alpha=0.7, marker='x')
    ax.set_title(title, fontsize=14)
    ax.set_xlabel(r'$\theta_u$', fontsize=12)
    ax.set_ylabel(r'$\theta_l$', fontsize=12)
    ax.set_zlabel('Trend ratio', fontsize=12)
    fig.colorbar(img, pad=0.1)
    fig.tight_layout()
    plt.savefig('./py_output/' + title + '_3D-1.svg')
    plt.savefig('./py_output/' + title + '_3D-1.pdf')
    plt.show()


def d2(x, y, z):
    fig = plt.figure(figsize=(7.2, 4.8), constrained_layout=True)
    plt.rcParams['font.family'] = "Times New Roman"
    ax = fig.add_subplot()
    ax.set_title(title, fontsize=14)
    ax.set_xlabel(r'$\theta_u$', fontsize=12)
    ax.set_ylabel(r'$\theta_l$', fontsize=12)
    ax.set_xlim([-0.0005, 0.0105])
    ax.set_ylim([-0.0002, 0.0052])

    ax.ticklabel_format(style='sci', axis='both')
    ax.grid(True, linestyle='-', color='0.75')
    img = ax.scatter(x, y, c=z, cmap=plt.jet(), alpha=0.7, marker='x')
    fig.colorbar(img)
    rect = patches.Rectangle((0.001832868, 0.000395417), 0.002704485, 0.000990735, edgecolor='lime', linewidth=2,
                             fill=False)
    ax.add_patch(rect)
    plt.savefig('./py_output/' + title.replace('$', '') + '_2D_with rectangle_zoom in-1.svg')
    plt.savefig('./py_output/' + title.replace('$', '') + '_2D_with rectangle_zoom in-1.pdf')
    plt.show()


def draw():
    df = pd.read_csv('./py_output/fine-tune.csv')
    x = df['theta_u'].to_numpy()
    y = df['theta_l'].to_numpy()
    z = df['Times'].to_numpy()
    u = df['trend ratio'].to_numpy()

    p1 = Process(target=d2, args=(x, y, u))
    p2 = Process(target=d3, args=(x, y, u))
    p3 = Process(target=d4, args=(x, y, z, u))
    p4 = Process(target=test_1234_mean_std_top5())
    p1.start()
    p2.start()
    p3.start()
    p4.start()


def analyze():
    file_paths = get_files()
    with open('./py_output/fine-tune.csv', 'w') as writer:
        writer.write('Times,theta_u,theta_l,trend ratio\n')
        for fp in file_paths:
            tr = get_tr(fp)
            fns = re.search(r'(\d+\.?\d*)times_(0\.\d*)_(0\.\d*)', fp)
            times, upper_bound, lower_bound = fns.groups()
            writer.write(times)
            writer.write(',')
            writer.write(upper_bound)
            writer.write(',')
            writer.write(lower_bound)
            writer.write(',')
            writer.write(tr)
            writer.write('\n')


def test_1234_mean_std_top5():
    plt.rcParams['font.family'] = "Times New Roman"
    fig = plt.figure(figsize=(7.2, 4.8), constrained_layout=True)
    ax = fig.add_subplot()
    mean_x = [0.003185111]
    mean_y = [0.000890785]
    x1 = [0.004537353,
          0.001832868,
          0.004537353,
          0.001832868]
    y1 = [0.001386152,
          0.000395417,
          0.000395417,
          0.001386152, ]
    x2 = [0.004283041,
          0.003037037,
          0.004118129,
          0.00279883,
          0.00345848]
    y2 = [0.000955556,
          0.000268889,
          0.001096296,
          0.000460819,
          0.000955556]
    ax.grid(True, linestyle='-', color='0.75')
    ax.scatter(mean_x, mean_y, c='tab:red', marker='x', label='Mean', s=100)
    ax.scatter(x1, y1, c='tab:orange', marker='+', label='Std', s=100)
    ax.scatter(x2, y2, c='tab:blue', marker='o', label='Top 5', s=100)
    ax.set_xlabel(r'$\theta_u$', fontsize=14)
    ax.set_ylabel(r'$\theta_l$', fontsize=14)
    ax.legend(fontsize=14, loc='center left')
    plt.savefig('./py_output/Test 1+2+3+4 mean & std & top 5.pdf')
    plt.savefig('./py_output/Test 1+2+3+4 mean & std & top 5.svg')
    plt.show()


if __name__ == '__main__':
    analyze()
    draw()
