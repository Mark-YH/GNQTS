# coding: UTF-8
# Created by Mark Hsu on 2021/1/12
#
"""
This file ...
"""
import numpy as np
import matplotlib.pyplot as plt


def get_return_trendline(fs):
    trend_line = []
    origin = fs[0]
    tmp = 0
    tmp2 = 0
    for j in range(0, len(fs)):
        tmp += ((j + 1) * fs[j] - (j + 1) * origin)
    for j in range(0, len(fs)):
        tmp2 += (j + 1) * (j + 1)
    slope = tmp / tmp2
    for j, __fs in enumerate(fs):
        trend_line.append(slope * (j + 1) + origin)
    return slope, trend_line


def sr():
    plt.figure(figsize=(7.2, 4.8))
    x = np.arange(0, 101, 1)
    y = np.arange(50, 151, 1)
    avg_line = np.zeros(len(x)) + 100
    lower_err = y - avg_line
    upp_err = [0] * len(x)
    err = [lower_err, upp_err]
    eb = plt.errorbar(x, y, yerr=err, errorevery=10, ecolor='tab:red', linewidth=0, elinewidth=2)
    eb[-1][0].set_linestyle('--')
    plt.plot(x, y, c='tab:blue', linewidth=2, label='Stable uptrend portfolio')
    plt.plot(x, avg_line, c='tab:green', linewidth=2, label='Average')

    plt.annotate(s="Risk", xy=(70, 110), xytext=(60, 80),
                 arrowprops=dict(arrowstyle='->', connectionstyle="arc3,rad=-.2"), fontsize=18, c='tab:red')
    plt.annotate(s="Risk", xy=(40, 95), xytext=(60, 80),
                 arrowprops=dict(arrowstyle='->', connectionstyle="arc3,rad=.2"), fontsize=18, c='tab:red')
    show('sr_explanation.pdf')


def tr():
    plt.figure(figsize=(7.2, 4.8))
    x = np.arange(0, 11, 1)
    y = np.arange(50, 151, 10)
    for i in range(len(y)):
        y[i] = y[i] + 20 * np.power(-1, i)

    trend_line = get_return_trendline(y)[1]
    print(y)
    print(trend_line)
    lower_err = y - trend_line
    upp_err = [0] * len(x)
    err = [lower_err, upp_err]
    eb = plt.errorbar(x, y, yerr=err, errorevery=1, ecolor='tab:red', linewidth=0, elinewidth=2)
    eb[-1][0].set_linestyle('--')
    plt.plot(x, y, c='tab:blue', linewidth=2, label='Stable uptrend portfolio')
    plt.plot(x, trend_line, c='tab:green', linewidth=2, label='Trend line')

    plt.annotate(s="Risk", xy=(5, 95), xytext=(6.5, 80),
                 arrowprops=dict(arrowstyle='->', connectionstyle="arc3,rad=.2"), fontsize=18, c='tab:red')
    plt.annotate(s="Risk", xy=(8, 135), xytext=(6.5, 80),
                 arrowprops=dict(arrowstyle='->', connectionstyle="arc3,rad=-.1"), fontsize=18, c='tab:red')
    show('tr_explanation.pdf')


def show(filename, x_label='Day', y_label='Daily return'):
    plt.xlabel(x_label, fontsize=14)
    plt.ylabel(y_label, fontsize=14)
    plt.legend(fontsize=14)
    plt.tight_layout()
    plt.savefig('./py_output/' + filename)
    plt.show()


if __name__ == '__main__':
    sr()
    tr()
