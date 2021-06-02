# coding: UTF-8
# Created by Mark Hsu on 2021/5/26
#
"""
This file ...
"""

import file_reader as fr
import os
from sliding_window import sws


# rs = {
#     version1: {sw1: pf1,
#                sw2: pf2,
#                ...
#                swn: pfn
#               },
#     version2: {sw1: pf1,
#                sw2: pf2,
#                ...
#                swn: pfn
#               },
#     .
#     .
#     .
#     versionn: {sw1: pf1,
#                sw2: pf2,
#                ...
#                swn: pfn
#               }
# }
def output(rs):
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
        result[label].update({sw: test_pf_pos / test_pf_neg})


if __name__ == '__main__':
    p = os.path.realpath('C:/Users/Lab114/Desktop/DJI30 convergence/result/DJI30 ANGQTS/')
    file_prefix = 'ANGQTS_EWFA'
    ver = 'EWFA'
    label = 'ANGQTS-EWFA'
    result = {}
    run(p, file_prefix, ver, label, result)
    file_prefix = 'ANGQTS_FA'
    ver = 'FA'
    label = 'ANGQTS-FA'
    run(p, file_prefix, ver, label, result)
    p = os.path.realpath('C:/Users/Lab114/Desktop/DJI30 convergence/result/DJI30 ANGQTS-SR/EWFA/DJI30 SR2TR/')
    file_prefix = 'SR2TR_Other'
    ver = 'EWFA'
    label = 'ANGQTS-SR'
    run(p, file_prefix, ver, label, result)
    output(result)
