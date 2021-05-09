# coding: UTF-8
# Created by Mark Hsu on 2021/5/6
#
"""
This file ...
"""

import re
import numpy as np


# finding special case
def negative_return(path, rank_path):
    try:
        sw = None
        with open(path, 'r') as reader:
            for row in reader:
                cols = re.split(',', row)
                if sw is None:
                    sw = cols[0]
                if cols[0] == '\n':
                    break
                elif cols[1] == 'Period':
                    continue
                period = cols[1]
                num = int(cols[2])
                portfolio = re.split(r'([a-zA-Z]+)\[\d+\]\(\d+\)\(\d+\)', cols[3])
                tr = cols[4]
                ret = cols[5]
                risk = cols[6]
                with open(rank_path + "rank_" + period, 'r') as reader:
                    bingo = False
                    for i, row2 in enumerate(reader):
                        cols2 = re.split(',', row2)
                        stock = re.split(r'\(\d+\)', cols2[1])[0]
                        try:
                            ret2 = float(cols2[3])
                        except ValueError as e:
                            continue
                        for s in portfolio:
                            if s == stock and ret2 < 0:
                                bingo = True
                                break
                    if bingo:
                        with open('./py_output/negative_result.csv', 'a') as writer:
                            writer.writelines(sw + ',' + period + ',' + str(num) + ',' +
                                              str(tr) + ',' + str(ret) + ',' + str(risk) + ',1' + '\n')
                    else:
                        with open('./py_output/negative_result.csv', 'a') as writer:
                            writer.writelines(sw + ',' + period + ',' + str(num) + ',' +
                                              str(tr) + ',' + str(ret) + ',' + str(risk) + ',0' + '\n')
    except FileNotFoundError as e:
        print("File not found: ", e.filename)
        pass
    except Exception as e:
        print(e)


# finding special case
def no_1st_stock(path, rank_path):
    try:
        sw = None
        with open(path, 'r') as reader:
            for row in reader:
                cols = re.split(',', row)
                if sw is None:
                    sw = cols[0]
                if cols[0] == '\n':
                    break
                elif cols[1] == 'Period':
                    continue
                period = cols[1]
                num = int(cols[2])
                portfolio = re.split(r'([a-zA-Z]+)\[\d+\]\(\d+\)\(\d+\)', cols[3])
                tr = cols[4]
                ret = cols[5]
                risk = cols[6]
                with open(rank_path + "rank_" + period, 'r') as reader:
                    for i, row2 in enumerate(reader):
                        cols2 = re.split(',', row2)
                        stock = re.split(r'\(1\)', cols2[1])[0]
                        if i == 1:
                            break
                    bingo = True
                    for s in portfolio:
                        if s == stock:
                            bingo = False
                            break
                    if bingo:
                        with open('./py_output/no_1st_result.csv', 'a') as writer:
                            writer.writelines(sw + ',' + period + ',' + str(num) + ',' +
                                              str(tr) + ',' + str(ret) + ',' + str(risk) + ',1' + '\n')
                    else:
                        with open('./py_output/no_1st_result.csv', 'a') as writer:
                            writer.writelines(sw + ',' + period + ',' + str(num) + ',' +
                                              str(tr) + ',' + str(ret) + ',' + str(risk) + ',0' + '\n')
    except FileNotFoundError as e:
        # print("File not found: ", e.filename)
        pass
    except Exception as e:
        print(e)


def get_single_result(path, sw):
    d_tr = {}
    d_ret = {}
    d_risk = {}
    with open(path, 'r') as reader:
        for row in reader:
            cols = row.split(',')
            try:
                period = int(cols[0])
                tr = float(cols[4])
                ret = float(cols[5])
                risk = float(cols[6])
                if tr < 0:
                    tr = ret = risk = 0
                if d_tr[period] < tr:
                    d_tr.update({period: tr})
                    d_ret.update({period: ret})
                    d_risk.update({period: risk})
            except ValueError:
                continue
            except KeyError:
                d_tr.update({period: tr})
                d_ret.update({period: ret})
                d_risk.update({period: risk})
    sum = 0.0
    for value in d_tr.values():
        sum += value
    mean_tr = sum / len(d_tr)
    sum = 0.0
    for value in d_ret.values():
        sum += value
    mean_ret = sum / len(d_ret)
    sum = 0.0
    for value in d_risk.values():
        sum += value
    mean_risk = sum / len(d_risk)

    with open('./py_output/result.csv', 'a') as writer:
        writer.write(sw + ',' + str(mean_ret) + ',' + sw + ',' + str(mean_risk) + ',' + sw + ',' + str(mean_tr) + '\n')


if __name__ == '__main__':
    sws = [
        'Y2Y',
        'Y2H',
        'Y2Q',
        'Y2M',
        'H2H',
        'H2Q',
        'H2M',
        'H*',
        'Q2Q',
        'Q2M',
        'Q*',
        'M2M',
        'M*'
    ]

    # get single stock result
    # for s in sws:
    #     p = r"C:/Users/Lab114/Desktop/2021 SMC Results/log/Top50_2016-2020 short_selling/Single/" + s.replace('*',
    #                                                                         '#') + "/short_selling_Single_" + s.replace(
    #         '*', '#') + "_final_result.csv"
    #     get_single_result(p, s)

    sw = "H2H"
    for s in sws:
        # if s != sw:
        #     continue
        ver = 'FA'
        prefix = 'ANGQTS-EIfixed'
        negative_return(
            r"C:/Users/Lab114/Desktop/DJI30 convergence/result/DJI30 ANGQTS-EIfixed/" + ver + "/" +
            s.replace('*', '#') + "/" + prefix + "_" + ver + "_" + s.replace('*', '#') + "_final_result.csv",
            r"C:/Users/Lab114/Desktop/DJI30 convergence/result/DJI30 Rank/Rank/" +
            s.replace('*', '#') + "/")
        no_1st_stock(
            r"C:/Users/Lab114/Desktop/DJI30 convergence/result/DJI30 ANGQTS-EIfixed/" + ver + "/" +
            s.replace('*', '#') + "/" + prefix + "_" + ver + "_" + s.replace('*', '#') + "_final_result.csv",
            r"C:/Users/Lab114/Desktop/DJI30 convergence/result/DJI30 Rank/Rank/" +
            s.replace('*', '#') + "/")
