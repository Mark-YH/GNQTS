# coding: UTF-8
# Created by Mark Hsu on 2021/5/26
#
"""
This file ...
"""
import numpy as np
import file_reader as fr
import os
from sliding_window import sws
from plot_fs import *


def output(rs):
    with open('./py_output/profit_factor.csv', 'w'):
        pass
    with open('./py_output/profit_factor.csv', 'a') as writer:
        writer.write('Profit factor,')
        for key in rs.keys():
            writer.write(key)
            writer.write(',')
        writer.write('\n')
        for sw in sws:
            writer.write(sw)
            for key, value in rs.items():
                writer.write(',')
                writer.write(str(value[sw]))
            writer.write('\n')
    for key, value in rs.items():
        print(key, value)


def run(p, file_prefix, ver, label, result):
    result.update({label: {}})
    for sw in sws:
        reader = fr.Reader(_path=p, _ver=ver, _sliding_window=sw, _file_prefix=file_prefix)
        test_rs = reader.get_final_result('final_test_result')
        test_pf_pos = 0
        test_pf_neg = 0
        for rs in test_rs:
            if rs.profit > 0:
                test_pf_pos += rs.profit
            else:
                test_pf_neg += -rs.profit
        if test_pf_neg == 0:
            result[label].update({sw: np.inf})
        else:
            result[label].update({sw: test_pf_pos / test_pf_neg})


def get_fs_obj():
    fs_result = {}
    for i in range(len(Config.paths)):
        fs_result.update({Config.labels[i]: {}})
    Config.mode = 'test'
    dict_files = get_files()

    for sw, files in dict_files.items():
        for i in fs_result.keys():
            fs_result[i].update({sw: {}})
        for fn in files:
            for i, path in enumerate(Config.paths):
                fp = path + sw + '/' + Config.file_prefix[i] + '_' + fn
                fs_result[Config.labels[i]][sw].update({fn: get_fs(fp)})
    Config.mode = 'total'
    dict_files = get_files()
    for sw, files in dict_files.items():
        for fn in files:
            for i, path in enumerate(Config.paths):
                fp = path + sw + '/' + Config.file_prefix[i] + '_' + sw + '_' + fn
                fs = []
                with open(fp) as reader:
                    for row in reader:
                        if re.match(r'FS\(\d+\)', row):
                            result = row.split(',')
                            fs.append(float(result[1]))
                fs_result[Config.labels[i]][sw].update({'total': fs})
    return fs_result


def get_daily_pf(fs):
    positive = 0
    negative = 0
    for i in range(len(fs)):
        if i == 0:
            continue
        diff = fs[i] - fs[i - 1]
        if diff > 0:
            positive += diff
        else:
            negative += abs(diff)
    if negative == 0:
        return np.inf
    return positive / negative


def get_mdd(fs):
    mdd = 0
    mdd_percent = 0
    if np.sum(fs) == 0:
        return 0, 0
    for i, val in enumerate(fs):
        if i == 0:
            continue
        histroy_max = np.max(fs[0:i])

        if histroy_max < val:
            continue

        if (histroy_max - val) / histroy_max > mdd_percent:
            mdd = histroy_max - val
            mdd_percent = mdd / histroy_max
    return mdd, mdd_percent


def get_net_profit(fs):
    return fs[-1] - fs[0]


def get_sr(fs):
    if fs[0] == 0 and np.sum(fs) == 0:
        return 0
    retRate = (fs[-1] - fs[0]) / fs[0] - 0.0087
    avg = np.mean(fs)

    tmp = 0.0
    for i in range(len(fs)):
        tmp += (fs[i] - avg) * (fs[i] - avg)

    risk = np.sqrt(tmp / len(fs)) / avg
    if risk == 0:
        return 0
    sr = retRate / risk
    return sr


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


def get_tr(fs):
    tl, slope = get_trendline(fs)
    risk = get_risk(fs, tl)
    if risk == 0:
        return 0
    return slope / risk


def get_first2last_line(fs):
    line = []
    for i in range(0, len(fs)):
        line.append((fs[-1] - fs[0]) / (len(fs) - 1) * i + fs[0])
    return line, ((fs[-1] - fs[0]) / (len(fs) - 1))


def get_fluctuation(fs, line):
    tmp = 0
    for i in range(len(fs)):
        tmp += (fs[i] - line[i]) * (fs[i] - line[i])

    return np.sqrt(tmp / len(fs))


def get_ei(fs):
    seq, slope = get_first2last_line(fs)
    fluctuation = get_fluctuation(fs, seq)

    if fluctuation == 0:
        return 0
    return slope / fluctuation


# collect all results calculated by fund standardization, including shrape ratio, max drawdown, and profit factor.
def collect_results():
    fs_result = get_fs_obj()
    rs = {}
    for version in Config.labels:
        rs.update({version: {}})
        for sw in fs_result[version].keys():
            rs[version].update({sw: {}})
            for period, fs in fs_result[version][sw].items():
                rs[version][sw].update({period: {}})
                rs[version][sw][period].update({'profit factor': get_daily_pf(fs)})
                mdd, mdd_percentage = get_mdd(fs)
                rs[version][sw][period].update({'mdd percentage': mdd_percentage})
                rs[version][sw][period].update({'mdd': mdd})
                rs[version][sw][period].update({'sharpe ratio': get_sr(fs)})
                rs[version][sw][period].update({'net profit': get_net_profit(fs)})
                rs[version][sw][period].update({'trend ratio': get_tr(fs)})
                rs[version][sw][period].update({'emotion index': get_ei(fs)})
    output_results(rs)


def output_results(rs):
    with open('./py_output/total_results.csv', 'w') as writer:
        for version in rs.keys():
            sws = rs[version].keys()
            for sw in rs[version].keys():
                for period in rs[version][sw].keys():
                    measurements = rs[version][sw][period].keys()
        for measurement in measurements:
            writer.write(measurement)
            writer.write(',')
            for ver in rs.keys():
                writer.write(ver)
                writer.write(',')
        writer.write('\n')
        for sw in sws:
            for measurement in measurements:
                writer.write(sw)
                writer.write(',')
                for version in rs.keys():
                    writer.write(str(rs[version][sw]['total'][measurement]))
                    writer.write(',')
            writer.write('\n')

    for version in rs.keys():
        for sw in rs[version].keys():
            for period in rs[version][sw].keys():
                if period == 'total':
                    rs[version][sw].pop(period)
                    rs[version][sw].update({'mean': {}})
                    for measurement in measurements:
                        rs[version][sw]['mean'].update({measurement: 0})
                    break
    for version in rs.keys():
        for sw in rs[version].keys():
            for measurement in measurements:
                for period in rs[version][sw].keys():
                    if period == 'mean':
                        continue
                    rs[version][sw]['mean'][measurement] += rs[version][sw][period][measurement]
                rs[version][sw]['mean'][measurement] /= len(rs[version][sw].keys()) - 1
    for sw in sws:
        with open('./py_output/test_results_' + sw + '.csv', 'w') as writer:
            for measurement in measurements:
                writer.write(measurement)
                writer.write(',')
                for ver in rs.keys():
                    writer.write(ver)
                    writer.write(',')
            writer.write('\n')

            for period in rs[list(rs.keys())[0]][sw].keys():
                for measurement in measurements:
                    writer.write(period)
                    writer.write(',')
                    for version in rs.keys():
                        writer.write(str(rs[version][sw][period][measurement]))
                        writer.write(',')
                writer.write('\n')


def profit_factor_formula1():
    p = os.path.realpath('/Users/Mark/Desktop/2010-202106/DJI30_2010-202106 ANGQTS/')
    file_prefix = 'ANGQTS_EWFA_'
    ver = 'EWFA'
    label = 'ANGQTS-EWFA'
    result = {}
    run(p, file_prefix, ver, label, result)
    file_prefix = 'ANGQTS_FA_'
    ver = 'FA'
    label = 'ANGQTS-FA'
    run(p, file_prefix, ver, label, result)
    p = os.path.realpath('/Users/Mark/Desktop/2010-202106/DJI30_2010-202106 ANGQTS-SR/EWFA/DJI30_2010-202106 SR2TR/')
    file_prefix = 'SR2TR_Other_'
    ver = 'Other'
    label = 'ANGQTS-SR'
    run(p, file_prefix, ver, label, result)
    output(result)


if __name__ == '__main__':
    profit_factor_formula1()
    collect_results()
