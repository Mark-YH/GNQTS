# coding: UTF-8
# Created by Mark Hsu on 2020/10/23.
#
"""
This program is used to split stock symbol from the symbol column of csv results.
"""
import re


def get_symbol(data):
    pattern = '[A-Z]+\[\d+\]{1}\(\d+\)\(\d+\)'
    symbol_pattern = '\[\d+\]{1}\(\d+\)\(\d+\)'
    print(re.findall(pattern, data))
    results = re.findall(pattern, data)

    for i, v in enumerate(re.finditer(pattern, data)):
        print(i, v)
    rs = []
    for it in results:
        rs.append(re.split(symbol_pattern, it)[0])
    print(rs)
    return rs


if __name__ == '__main__':
    get_symbol('AAPL[0](38888)(1188811) BA[2](2489)(139860) MCD[15](138122)(8671328) ')
