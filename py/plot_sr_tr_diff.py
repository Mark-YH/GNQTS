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
    y1 = np.arange(50, 151, 1)
    y2 = np.arange(150, 49, -1)
    avg_line = np.zeros(len(x)) + 100
    lower_err1 = y1 - avg_line
    lower_err2 = y2 - avg_line
    upp_err = [0] * len(x)
    err1 = [lower_err1, upp_err]
    err2 = [lower_err2, upp_err]
    eb1 = plt.errorbar(x, y1, yerr=err1, errorevery=10, ecolor='tab:green', linewidth=0, elinewidth=2)
    eb2 = plt.errorbar(x, y2, yerr=err2, errorevery=10, ecolor='tab:blue', linewidth=0, elinewidth=2)
    eb1[-1][0].set_linestyle('--')
    eb2[-1][0].set_linestyle('--')
    plt.plot(x, y1, c='tab:green', linewidth=2, label='Uptrend portfolio')
    plt.plot(x, y2, c='tab:blue', linewidth=2, label='Downtrend portfolio')
    plt.plot(x, avg_line, c='tab:orange', linewidth=2, label='Average line')

    plt.annotate(s="Risk", xy=(30, 90), xytext=(45, 80),
                 arrowprops=dict(arrowstyle='->', connectionstyle="arc3,rad=-.2"), fontsize=18, c='tab:red')
    plt.annotate(s="Risk", xy=(70, 90), xytext=(45, 80),
                 arrowprops=dict(arrowstyle='->', connectionstyle="arc3,rad=.2"), fontsize=18, c='tab:red')
    plt.xlabel('Day', fontsize=14)
    plt.ylabel('Daily return', fontsize=14)
    plt.legend(fontsize=14, loc='upper center')
    plt.tight_layout()
    plt.savefig('./py_output/sr_explanation.pdf')
    plt.show()


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
    plt.xlabel('Day', fontsize=14)
    plt.ylabel('Funds standardization', fontsize=14)
    plt.legend(fontsize=14, loc='upper center')
    plt.tight_layout()
    plt.savefig('./py_output/tr_explanation.pdf')
    plt.show()


def ei():
    plt.figure(figsize=(7.2, 4.8))
    x = np.arange(0, 11, 1)
    y = np.arange(50, 151, 10)
    for i in range(len(y)):
        if i < 5:
            y[i] = y[i] + 100 * np.power(-1, i)
        else:
            y[i] = y[i] + 30 * np.power(-1, i)
    y[0] = y[0] - 100
    y[-1] = y[-1] - 30
    first2last_line = np.arange(50, 151, 10)
    lower_err = y - first2last_line
    upp_err = [0] * len(x)
    err = [lower_err, upp_err]
    eb = plt.errorbar(x, y, yerr=err, errorevery=1, ecolor='tab:red', linewidth=0, elinewidth=2)
    eb[-1][0].set_linestyle('--')
    plt.plot(x, y, c='tab:blue', linewidth=2, label='Portfolio')
    plt.plot(x, first2last_line, c='tab:green', linewidth=2, label='First-to-last line')

    plt.annotate(s="", xy=(3, 45), xytext=(3.8, 37),
                 arrowprops=dict(arrowstyle='->', connectionstyle="arc3,rad=.2"), fontsize=18, c='tab:red')
    plt.annotate(s="Emotional ups and downs", xy=(4, 135), xytext=(3.8, 25),
                 arrowprops=dict(arrowstyle='->', connectionstyle="arc3,rad=-.3"), fontsize=18, c='tab:red')
    plt.xlabel('Day', fontsize=14)
    plt.ylabel('Funds standardization', fontsize=14)
    plt.legend(fontsize=14, loc='best')
    plt.tight_layout()
    plt.savefig('./py_output/ei_explanation.pdf')
    plt.show()


if __name__ == '__main__':
    sr()
    tr()
    ei()
