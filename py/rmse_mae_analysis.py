# coding: UTF-8
"""
Created on 2021/7/28
This script is used to analyze the experiment of RMSE vs. MAE.

1. generate RMSE risk and MAE risk for portfolio (RMSE) and portfolio (MAE)
2. generate portfolio FS and single stock fs

@author: Mark Hsu
"""
from file_reader import Reader
from sliding_window import sws
from MAE_RMSE import get_rmse_risk, get_trendline, get_rmse_tr, get_mae_risk, get_mae_tr
from plot_fs import get_fs, add_fs, save_fs, Config
import numpy as np
import os
import pandas as pd

p_mae = '/Users/Mark/Desktop/RMSE MAE/DJI30/DJI30 心情波動 ABS/'
file_prefix_mae = ''
p_rmse = '/Users/Mark/Desktop/RMSE MAE/DJI30/DJI30 GNQTS/'
file_prefix_rmse = 'GNQTS_EWFA_'


def get_single_fs(sw, mode, period, stock):
    p = os.path.join(os.path.realpath('../data/DJI30/'), sw, mode, period)
    df = pd.read_csv(p)
    df = df.drop('Unnamed: 30', axis=1)

    fund = 1e7
    price = df[stock].to_numpy()
    amount = np.floor(fund / price[0])
    buy_remain = fund - amount * price[0]
    single_stock_fs = amount * price + buy_remain
    return single_stock_fs


def get_spec_fs(sw, mode, period, portfolio):
    p = os.path.join(os.path.realpath('../data/DJI30/'), sw, mode, period)
    df = pd.read_csv(p)
    df = df.drop('Unnamed: 30', axis=1)

    for item in df.keys():
        if item not in portfolio:
            df = df.drop(item, axis=1)

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


def write_title():
    with open('py_output/result.csv', 'w') as writer:
        writer.write(
            'Sliding window,Period,Number of assets,Type,Expected return,RMSE risk,MAE risk,RMSE TR,MAE TR,Assets\n')


def write_contents(sw, period, stock_number, portfolio, fs, label):
    with open('py_output/result.csv', 'a') as writer:
        writer.write(sw + ',' + period + ',')
        writer.write(str(stock_number) + ',' + label + ',')
        tl, ret = get_trendline(fs)
        writer.write(str(ret) + ',')
        writer.write(str(get_rmse_risk(fs, tl)) + ',')
        writer.write(str(get_mae_risk(fs, tl)) + ',')
        writer.write(str(get_rmse_tr(fs)) + ',')
        writer.write(str(get_mae_tr(fs)) + ',')
        for stock in portfolio:
            writer.write(stock + ',')
        writer.write('\n')


def run():
    count = 0
    write_title()
    for sw in sws:
        reader_mae = Reader(_path=p_mae, _ver='平分', _sliding_window=sw, _file_prefix=file_prefix_mae)
        rs_mae = reader_mae.get_final_result('final_result')

        reader_rmse = Reader(_path=p_rmse, _ver='EWFA', _sliding_window=sw, _file_prefix=file_prefix_rmse)
        rs_rmse = reader_rmse.get_final_result('final_result')

        if len(rs_mae) != len(rs_rmse):
            raise ValueError

        for i in range(len(rs_mae)):
            if rs_mae[i].stock_number != rs_rmse[i].stock_number or rs_rmse[i].stock_number == 0:
                continue
            portfolio_mae = []
            portfolio_rmse = []
            for j in range(rs_mae[i].stock_number):
                portfolio_mae.append(next(iter(rs_mae[i].stock_fund[j])))
                portfolio_rmse.append(next(iter(rs_rmse[i].stock_fund[j])))

            count_mae_in_rmse = 0
            count_rmse_in_mae = 0
            diff_rmse = []
            diff_mae = []
            for stock in portfolio_mae:
                if stock in portfolio_rmse:
                    count_mae_in_rmse += 1
                else:
                    diff_mae.append(stock)

            for stock in portfolio_rmse:
                if stock in portfolio_mae:
                    count_rmse_in_mae += 1
                else:
                    diff_rmse.append(stock)

            if count_mae_in_rmse == count_rmse_in_mae and count_mae_in_rmse == rs_mae[i].stock_number - 1:
                count += 1
                fs_mae = get_fs(p_mae + '平分/' + sw + '/output' + file_prefix_mae + '_' + rs_mae[i].period)
                fs_rmse = get_fs(p_rmse + 'EWFA/' + sw + '/' + file_prefix_rmse + rs_rmse[i].period)
                add_fs(fs_rmse, 'Portfolio (RMSE)', Config.colors[0])
                add_fs(fs_mae, 'Portfolio (MAE)', Config.colors[1])
                save_fs(sw, rs_mae[i].period)

                write_contents(sw, rs_rmse[i].period, rs_rmse[i].stock_number, portfolio_rmse, fs_rmse,
                               'Portfolio (RMSE)')
                write_contents(sw, rs_mae[i].period, rs_mae[i].stock_number, portfolio_mae, fs_mae, 'Portfolio (MAE)')

                # single stock
                add_fs(get_single_fs(sw, 'train', rs_rmse[i].period, diff_rmse[0]), diff_rmse[0] + ' (by RMSE)',
                       Config.colors[0])
                add_fs(get_single_fs(sw, 'train', rs_mae[i].period, diff_mae[0]), diff_mae[0] + ' (by MAE)',
                       Config.colors[1])

                # n-1 portfolio
                portfolio_mae.remove(diff_mae[0])
                fs_n_1 = get_spec_fs(sw, 'train', rs_mae[i].period, portfolio_mae)
                write_contents(sw, rs_mae[i].period, rs_mae[i].stock_number, portfolio_mae, fs_n_1, 'Portfolio (n-1)')
                add_fs(fs_n_1, 'Portfolio (n-1)', Config.colors[2])
                save_fs(sw, 'Single_' + rs_mae[i].period)

    print('Total: ', count)


if __name__ == '__main__':
    run()
