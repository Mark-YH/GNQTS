# coding: UTF-8
"""
Created on 2021/8/20

@author: Mark Hsu
"""
import os.path as path
import pandas as pd
import numpy as np
import measurements as m


def get_fs():
    p = path.realpath('../data/DJIA/DJI_2010-202106.csv')
    df = pd.read_csv(p)

    price = df['Close'].to_numpy()
    init_fund = 1e7
    amount = np.floor(init_fund / price[0])
    buy_remain = init_fund - amount * price[0]
    return amount * price + buy_remain


def run():
    fs = get_fs()
    tl, ret = m.get_trendline(fs)
    risk = m.get_risk(fs, tl)
    tr = m.get_tr(fs)
    l, profit = m.get_first2last_line(fs)
    f = m.get_fluctuation(fs, l)
    ei = m.get_ei(fs)

    with open('py_output/DJIA_result.csv', 'w') as writer:
        writer.write('Period,2010-202106\n')
        writer.write('Expected return,')
        writer.write(str(ret))
        writer.write('\n')
        writer.write('Risk,')
        writer.write(str(risk))
        writer.write('\n')
        writer.write('Trend ratio,')
        writer.write(str(tr))
        writer.write('\n')
        writer.write('Real return,')
        writer.write(str(profit))
        writer.write('\n')
        writer.write('Fluctuation,')
        writer.write(str(f))
        writer.write('\n')
        writer.write('Emotion index,')
        writer.write(str(ei))
        writer.write('\n')

        for i, it in enumerate(fs):
            writer.write(str(i + 1))
            writer.write(',')
            writer.write(str(it))
            writer.write('\n')


if __name__ == '__main__':
    run()
