# coding: UTF-8
# Created by Mark Hsu on 2021/1/8
#
"""
This file ...
"""

import numpy as np
import matplotlib.pyplot as plt


def run():
    x, a, b, c, d = get_seq_sin()
    x, a, b, c, d = get_seq_rotation()
    # plt.figure(figsize=(10.8, 7.2))
    plt.figure(figsize=(7.2, 4.8))
    plt.xlabel('Day', fontsize=14)
    plt.ylabel('Daily return', fontsize=14)
    plt.plot(x, a, 'g', x, b, 'b', x, c, 'y', x, d, 'r')
    plt.legend(('Stock A', 'Stock B', 'Equally weighted portfolio', 'Optimal portfolio'), fontsize=10)
    plt.tight_layout()
    plt.savefig('./py_output/ideal_situation.pdf')
    plt.show()


def get_seq_sin():
    n = 101
    x = [i + 1 for i in range(101)]
    a = np.zeros(n)
    b = np.zeros(n)
    c = np.zeros(n)
    d = np.zeros(n)

    for i in range(len(a)):
        a[i] = np.sin(i / (n - 1) * 2 * np.pi) * 25
        b[i] = a[i] * 4 * -1
        c[i] = (a[i] + b[i]) / 2
    a = a + 100
    b = b + 100
    c = c + 100
    d = d + 100
    return x, a, b, c, d


def get_seq_rotation():
    n = 121
    x = [i + 1 for i in range(101)]
    a = np.zeros(n)
    b = np.zeros(n)
    c = np.zeros(n)
    d = np.zeros(n)

    for i in range(len(a)):
        a[i] = np.sin(i / (n - 1) * 2 * np.pi + np.pi) * 50

    a = a[10:111]
    b = b[10:111]
    c = c[10:111]
    d = d[10:111]

    d[0] = a[0]
    d[-1] = a[-1]
    for _x in np.arange(1, len(x) - 1):
        d[_x] = d[0] + (d[-1] - d[0]) * (x[_x] - x[0]) / (x[-1] - x[0])
    b = (d - a) * 4 + a
    c = (a + b) / 2

    a = a + 100
    b = b + 100
    c = c + 100
    d = d + 100
    print(np.min(a))
    print(np.max(b))
    print(np.max(c))
    print(d)
    return x, a, b, c, d


if __name__ == '__main__':
    run()
