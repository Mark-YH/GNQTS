# coding: UTF-8
# Created by Mark Hsu on 2021/1/21
#
"""
This file ...
"""

import matplotlib.pyplot as plt
import os
import re


def run():
    root = 'C:/Users/Lab114/Desktop/DJI30 convergence/result/'
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
    version = ['ANGQTS', 'GNQTS']
    mode = ['FA']
    for sw in sliding_windows:
        p = os.path.join(root, 'DJI30 ' + version[0], mode[0], sw)
        files = []
        for dir in os.listdir(p):
            if str(dir).__contains__('convergence'):
                files.append(dir)
        for file in files:
            data = []
            for ver in version:
                for m in mode:
                    with open(os.path.join(root, 'DJI30 ' + ver, m, sw,
                                           file.replace('ANGQTS', ver).replace('EWFA', m)), 'r') as reader:
                        d = []
                        for row in reader:
                            if row.__contains__('Generation'):
                                continue
                            try:
                                d.append(float(row.split(',')[1]))
                            except Exception as e:
                                print(e)
                    data.append(d)
            plot_data(get_title(sw, file), sw, data)


def get_title(sw, filename):
    months = {'01': 'Jan', '02': 'Feb', '03': 'Mar', '04': 'Apr', '05': 'May', '06': 'Jun', '07': 'Jul', '08': 'Aug',
              '09': 'Sep', '10': 'Oct', '11': 'Nov', '12': 'Dec'}
    period = re.split(r'(test|train)_|\(.+\)\.csv', filename)[2]
    period = period.replace('_', ' ').replace('~', ' - ')
    for key, val in months.items():
        period = period.replace(' ' + key, ' ' + val).replace('-' + key, '-' + val)
    return sw.replace('#', '*') + ' $-$ ' + period


def plot_data(title, sw, data):
    plt.figure(figsize=(7.2, 4.8))
    x = [i + 1 for i in range(0, 10000)]
    plt.plot(x, data[0], label='ANGQTS-FA')
    plt.plot(x, data[1], label='GNQTS-FA')
    # plt.plot(data[2])
    # plt.plot(data[3])
    plt.legend(fontsize=14)
    plt.xlabel('Generation', fontsize=14)
    plt.ylabel('Fitness', fontsize=14)
    plt.title(title, fontsize=14)
    plt.tight_layout()
    file_path = 'py_output/convergence/' + sw + '/FA'
    extension = '.svg'
    if not os.path.exists(file_path):
        os.makedirs(file_path)
    plt.savefig(file_path + '/' + title.replace('$', '').replace('*', '#') + extension)
    extension = '.pdf'
    plt.savefig(file_path + '/' + title.replace('$', '').replace('*', '#') + extension)
    plt.close()
    # plt.show()


if __name__ == '__main__':
    run()
