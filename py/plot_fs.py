# coding: UTF-8
# Created by Mark Hsu on 2020/10/24
#
import matplotlib.pyplot as plt
import re
import os
import numpy as np


class Config:
    paths = ['C:/Users/Lab114/Desktop/DJI30/DJI30 Sharpe ratio/平分/',
             'C:/Users/Lab114/Desktop/DJI30/DJI30 心情波動/平分/',
             'C:/Users/Lab114/Desktop/DJI30/DJI30 心情波動/分配/']
    labels = ['SR', 'TR EWFA', 'TR FA']
    colors = [(0.2, 0.4, 1), (1, 0.5, 0.2), (0.2, 0.6, 0.4)]
    mode = ['train']
    sliding_windows = [
        'Y2Y',
        'Y2H',
        'Y2Q',
        'Y2M',
        'H2H',
        'H2Q',
        'H2M',
        'H#',
        'Q2Q',
        'Q2M',
        'Q#',
        'M2M',
        'M#'
    ]


def get_trend_line(_fs):
    origin = 1e7
    x = _fs
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

    return trend_line


def get_files():
    results = {}
    cwd = os.getcwd()
    for path in Config.paths:
        for sw in Config.sliding_windows:
            os.chdir(cwd)
            os.chdir(path + sw)
            _files = []
            for item in os.listdir():
                for m in Config.mode:
                    if re.match(r'output_' + m, item):
                        _files.append(item)
            results.update({sw: _files})
    os.chdir(cwd)
    return results


def get_fs(file_path):
    fs = []
    with open(file_path, 'r') as file:
        no_invest = False
        col = 0
        for row in file:
            if re.match(r'Number of chosen,\d+', row):
                result = row.split(',')
                if int(result[1]) == 0:
                    no_invest = True
            if re.match(r'Balance of each stock,+', row):
                result = row.split(',')
                col = len(result) - 1
            if re.match(r'FS\(\d+\)', row):
                result = row.split(',')
                if no_invest:
                    fs.append(float(result[1]))
                else:
                    fs.append(float(result[col]))
    return fs


def add_fs(_fs, label='?', color=(0, 0, 0)):
    avg = []
    for i in range(len(_fs)):
        avg.append(np.average(_fs))
    r, g, b = color

    plt.plot(_fs, label=label, color=color)
    plt.plot(avg, label=label + ' avg.', linestyle='dotted', color=(r, g, b, 0.7))
    plt.plot(get_trend_line(_fs), label=label + ' tl.', linestyle='dashed', color=(r, g, b, 0.7))


def save_fs(sw, filename):
    if filename.__contains__('test'):
        mode = 'test'
    if filename.__contains__('train'):
        mode = 'train'
    period = re.split(r'output_(test|train)_|\(.+\)\.csv', filename)[2]
    os.makedirs('./img/' + mode + '/' + sw, exist_ok=True)
    plt.title(sw.replace('#', '*') + ' ' + period)
    plt.xlabel('Day')
    plt.ylabel('Funds Standardization')
    plt.legend()
    plt.savefig('img/' + mode + '/' + sw + '/' + filename.split('.csv')[0] + '.svg')
    # plt.show()
    plt.clf()


def run():
    dict_files = get_files()
    for sw, files in dict_files.items():
        for fn in files:
            for i, path in enumerate(Config.paths):
                fp = path + sw + '/' + fn
                add_fs(get_fs(fp), Config.labels[i], Config.colors[i])
            save_fs(sw, fn)


def single():
    for i, path in enumerate(Config.paths):
        fp = path + 'M#/output_train_2018_12(2018 Q1).csv'
        add_fs(get_fs(fp), Config.labels[i], Config.colors[i])
    save_fs('M#', 'output_train_2018_12(2018 Q1).csv')


if __name__ == '__main__':
    run()
    # single()
