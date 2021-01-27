# coding: UTF-8
# Created by Mark Hsu on 2020/11/19
#

import re
from os import path, makedirs


class Reader:
    def __init__(self, _sliding_window=None, _ver=None, _path=None, _file_prefix=None):
        if _sliding_window is None:
            _sliding_window = 'Y2Y'
        if _ver is None:
            _ver = ''
        if _path is None:
            _path = path.curdir
        if _file_prefix is None:
            _file_prefix = ''
        self.sliding_window = _sliding_window
        self.version = _ver
        self.path = path.join(_path, _ver, _sliding_window)

    def get_final_result(self):
        if not path.exists(path.join('./py_output/', self.version, self.sliding_window)):
            makedirs(path.join('./py_output/', self.version, self.sliding_window))
        with open(path.join(self.path, file_prefix + '_' + self.sliding_window + '_final_result.csv')) as csvfile:
            for i, row in enumerate(csvfile):
                if i == 0:
                    continue
                rs = row.split(',')
                try:
                    if re.search(r'Execution time | \n', rs[0]):
                        continue
                    with open(path.join('./py_output/', self.version, self.sliding_window, rs[0] + '.csv'),
                              'a') as writer:
                        rs = re.findall(r'[A-Z]+\[\d+\]\(\d+\)\(\d+\)', rs[3])
                        print('')
                        for stock in rs:
                            group = re.search(r'([a-zA-Z]+)\[(\d+)\]\((\d+)\)\((\d+)\)', stock)
                            symbol = group.group(1)
                            stock_index = group.group(2)
                            amount = group.group(3)
                            allocated_funds = group.group(4)
                            writer.write(stock_index + ' ' + allocated_funds + '\n')
                            print(symbol, amount, allocated_funds)
                except Exception as e:
                    print(e, 'row:', row)

    def get_period_result(self, testing_period=False):
        pass


if __name__ == '__main__':
    p = 'C:/Users/Lab114/Desktop/DJI 30 convergence/result/DJI30 ANGQTS-SR/'
    file_prefix = 'ANGQTS-SR_EWFA'
    sws = ['Y2Y', 'Y2H', 'Y2Q', 'Y2M', 'H2H', 'H2Q', 'H2M', 'H#', 'Q2Q', 'Q2M', 'Q#', 'M2M', 'M#']
    for sw in sws:
        reader = Reader(_path=p, _ver='EWFA', _sliding_window=sw, _file_prefix=file_prefix)
        reader.get_final_result()
