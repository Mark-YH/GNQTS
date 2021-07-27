# coding: UTF-8
"""
Created on 2021/7/21

@author: Mark Hsu
"""
import numpy as np
import pandas as pd
import os
import measurements


class Measurement:
    def __init__(self, fs):
        self.fs = fs

    def get_ei(self):
        def get_first2last_line():
            line = []
            for i in range(0, len(self.fs)):
                line.append((self.fs[-1] - self.fs[0]) / (len(self.fs) - 1) * i + self.fs[0])
            return line

        def get_fluctuation(line):
            tmp = 0
            for i in range(len(self.fs)):
                tmp += (self.fs[i] - line[i]) * (self.fs[i] - line[i])

            return np.sqrt(tmp / len(self.fs))

        seq = get_first2last_line()
        fluctuation = get_fluctuation(seq)

        if fluctuation == 0:
            return 0
        return ((self.fs[-1] - self.fs[0]) / (len(self.fs) - 1)) / fluctuation

    def get_tr(self):
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

        def get_risk(fs, tl):
            tmp = 0

            for i in range(len(fs)):
                tmp += (fs[i] - tl[i]) * (fs[i] - tl[i])

                risk = np.sqrt(tmp / len(fs))
            return risk

        tl, slope = get_trendline(self.fs)
        risk = get_risk(self.fs, tl)

        if slope == 0:
            return 0
        if risk == 0:
            return np.inf
        return slope / risk

    def get_sr(self):
        return measurements.get_sr(self.fs)

    def get_net_profit(self):
        return self.fs[-1] - self.fs[0]

    def get_pf(self):
        return measurements.get_daily_pf(self.fs)

    def get_mdd(self):
        return measurements.get_mdd(self.fs)


def get_fs():
    path = os.path.realpath('../data/DJI30/Y2Y/test')
    files = os.listdir(path)
    files.sort()
    df = pd.concat([pd.read_csv(os.path.join(path, file)) for file in files])
    df = df.drop('Unnamed: 30', axis=1)
    total_fs = np.zeros(df.shape[0])
    init_fund = 1e7
    fund = int(init_fund / df.shape[1])
    alloc_remain = init_fund - fund * df.shape[1]

    price = df.to_numpy()
    amount = np.floor(fund / price[0])
    buy_remain = fund - amount * price[0]
    each_stock_fs = amount * price + buy_remain
    each_stock_fs[0] = [fund for i in range(df.shape[1])]

    for i in range(df.shape[0]):
        total_fs[i] = np.sum(each_stock_fs[i]) + alloc_remain
    return total_fs


def run():
    fs = get_fs()
    m = Measurement(fs)

    with open('./py_output/UBH_result.csv', 'w') as writer:
        writer.write(',emotion index,trend ratio,net profit,profit factor,mdd percentage,mdd,sharpe ratio\n')
        writer.write('UBH,')
        writer.write(str(m.get_ei()))
        writer.write(',')
        writer.write(str(m.get_tr()))
        writer.write(',')
        writer.write(str(m.get_net_profit()))
        writer.write(',')
        writer.write(str(m.get_pf()))
        writer.write(',')
        writer.write(str(m.get_mdd()[1]))
        writer.write(',')
        writer.write(str(m.get_mdd()[0]))
        writer.write(',')
        writer.write(str(m.get_sr()))
        writer.write('\n')

        for i, item in enumerate(fs):
            writer.write('FS(')
            writer.write(str(i + 1))
            writer.write('),')
            writer.write(str(item))
            writer.write('\n')
        print('emotion index', m.get_ei())
        print('trend ratio', m.get_tr())
        print('sharpe ratio', m.get_sr())
        print('profit factor', m.get_pf())
        print('max drawdown', m.get_mdd()[0])
        print('max drawdown percentage', m.get_mdd()[1])
        print('net profit', m.get_net_profit())


if __name__ == '__main__':
    run()
