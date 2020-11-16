# coding: UTF-8
# Created by Mark Hsu on 2020/11/12
#
"""
This file ...
"""
from os import path

paths = ['C:/Users/Lab114/Desktop/DJI30/DJI30 心情波動',
         'C:/Users/Lab114/Desktop/DJI30/DJI30 linear adaptive',
         'C:/Users/Lab114/Desktop/DJI30/DJI30 exponential adaptive']
ver = ['平分', '分配']
sliding_windows = ["Y2Y", "Y2H", "Y2Q", "Y2M", "H2H", "H2Q", "H2M", "H#", "Q2Q", "Q2M", "Q#", "M2M", "M#"]
specific_columns = ['at round', 'at generation', 'Found best count']


def read():
    for p in paths:
        for v in ver:
            for sw in sliding_windows:
                file_path = path.join(path.join(path.join(path.realpath(p), v), sw), sw + '_final_result.csv')
                with open(file_path) as csvfile:
                    i_round = 0
                    avg_round = 0
                    i_gen = 0
                    avg_gen = 0
                    i_best_count = 0
                    avg_best_count = 0
                    period_count = 0
                    for i, row in enumerate(csvfile):
                        if i == 0:
                            for ic, c in enumerate(str(row).split(',')):
                                if c.replace('\n', '') == specific_columns[0]:
                                    i_round = ic
                                elif c.replace('\n', '') == specific_columns[1]:
                                    i_gen = ic
                                elif c.replace('\n', '') == specific_columns[2]:
                                    i_best_count = ic
                        else:
                            try:
                                gen = int(str(row).split(',')[i_gen])
                                bc = int(str(row).split(',')[i_best_count])
                                round = int(str(row).split(',')[i_round])
                                if int(str(row).split(',')[2]) == 0:
                                    continue
                                avg_gen += gen
                                avg_best_count += bc
                                avg_round += round
                                period_count += 1
                            except Exception as e:
                                # print(e, ':', str(row).replace('\n', '\\n'))
                                pass
                    avg_gen /= period_count
                    avg_best_count /= period_count
                    avg_round /= period_count
                    print(
                        '{}, Periods count: {}, avg round: {:.2f}, avg generation: {:.2f}, avg best count: {:.2f}'.format(
                            file_path, period_count, avg_round, avg_gen, avg_best_count))


if __name__ == '__main__':
    read()
