# coding: UTF-8
"""
Created on 2021/7/27

@author: Mark Hsu
"""

import numpy as np
import pandas as pd


def get_trendline(fs):
    origin = fs[0]
    x = fs
    tmp = 0
    tmp2 = 0
    trend_line = []

    for i in range(0, len(x)):
        tmp += ((i + 1) * x[i] - (i + 1) * origin)
    for i in range(0, len(x)):
        tmp2 += (i + 1) * (i + 1)

    slope = tmp / tmp2

    for i in range(0, len(x)):
        trend_line.append(slope * (i + 1) + origin)
    return trend_line, slope


def get_mae_risk(fs, tl):
    tmp = 0

    for i in range(len(fs)):
        tmp += np.abs(fs[i] - tl[i])

    risk = tmp / len(fs)
    return risk


def get_rmse_risk(fs, tl):
    tmp = 0

    for i in range(len(fs)):
        tmp += (fs[i] - tl[i]) * (fs[i] - tl[i])
    risk = np.sqrt(tmp / len(fs))
    return risk


def get_mae_tr(fs):
    tl, slope = get_trendline(fs)
    risk = get_mae_risk(fs, tl)
    if slope == 0:
        return 0
    if risk == 0:
        return np.inf
    if slope > 0:
        return slope / risk
    else:
        return slope * risk


def get_rmse_tr(fs):
    tl, slope = get_trendline(fs)
    risk = get_rmse_risk(fs, tl)
    if slope == 0:
        return 0
    if risk == 0:
        return np.inf
    if slope > 0:
        return slope / risk
    else:
        return slope * risk


if __name__ == '__main__':
    df = pd.read_csv('py_output/fs.csv')
    labels = df.keys().tolist()
    for l in labels:
        fs = df[l].tolist()
        print(l, end=",")
        tl, ret = get_trendline(fs)
        print(ret, end=",")
        print(get_rmse_risk(fs, tl), end=",")
        print(get_mae_risk(fs, tl), end=",")
        print(get_rmse_tr(fs), end=",")
        print(get_mae_tr(fs))
