# coding: UTF-8
# Created by Mark Hsu on 2021/5/26
#
"""
This file ...
"""
from plot_fs import *
import numpy as np


def get_mdd(fs):
    mdd = 0
    mdd_percent = 0
    for i, val in enumerate(fs):
        if i == 0:
            continue
        histroy_max = np.max(fs[0:i])

        if histroy_max < val:
            continue

        if histroy_max - val > mdd:
            mdd = histroy_max - val
            mdd_percent = mdd / histroy_max
    return mdd, mdd_percent


def run():
    dict_files = get_files()
    for sw, files in dict_files.items():
        with open('./py_output/max_drawdown_' + sw + '.csv', 'a') as writer:
            writer.write('Max drawdown,ANGQTS-EWFA,ANGQTS-EWFA,ANGQTS-FA,ANGQTS-FA,ANGQTS-SR,ANGQTS-SR')
            writer.write('\n')
            for fn in files:
                fp1 = Config.paths[0] + sw + '/' + Config.file_prefix[0] + '_' + fn
                fp2 = Config.paths[1] + sw + '/' + Config.file_prefix[1] + '_' + fn
                fp3 = Config.paths[2] + sw + '/' + Config.file_prefix[2] + '_' + fn
                EWFA = get_fs(fp1)
                FA = get_fs(fp2)
                SR = get_fs(fp3)
                mdd1, mddp1 = get_mdd(EWFA)
                mdd2, mddp2 = get_mdd(FA)
                mdd3, mddp3 = get_mdd(SR)
                writer.write(fn)
                writer.write(',')
                writer.write(str(mdd1))
                writer.write(',')
                writer.write(str(mddp1))
                writer.write(',')
                writer.write(str(mdd2))
                writer.write(',')
                writer.write(str(mddp2))
                writer.write(',')
                writer.write(str(mdd3))
                writer.write(',')
                writer.write(str(mddp3))
                writer.write('\n')


if __name__ == '__main__':
    run()
