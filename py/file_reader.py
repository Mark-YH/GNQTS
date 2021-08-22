# coding: UTF-8
# Created by Mark Hsu on 2020/11/19
#

import re
from os import path, makedirs
from sliding_window import sws


class Result:
    def __init__(self):
        self.period_num = -1
        self.period = ''
        self.stock_number = -1
        self.stock_fund = []
        self.stock_share = []
        self.tr = -1
        self.ret = -1
        self.risk = -1
        self.ei = -1
        self.profit = -1
        self.fluctuation = -1
        self.round = -1
        self.generation = -1
        self.count = -1


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
        self.file_prefix = _file_prefix

    def get_final_result(self, filename):
        if not path.exists(path.join('./py_output/', self.version, self.sliding_window)):
            makedirs(path.join('./py_output/', self.version, self.sliding_window))
        container = []
        with open(path.join(self.path,
                            self.file_prefix + self.sliding_window + '_' + filename + '.csv')) as csvfile:
            for i, row in enumerate(csvfile):
                if i == 0:
                    continue
                rs = row.split(',')
                try:
                    if re.search(r'(Execution time)|(\n)', rs[0]):
                        continue
                    container.append(Result())
                    container[-1].period_num = int(rs[0])
                    container[-1].period = rs[1]
                    container[-1].stock_number = int(rs[2])
                    container[-1].tr = float(rs[4])
                    container[-1].ret = float(rs[5])
                    container[-1].risk = float(rs[6])
                    if filename.__contains__('test'):
                        container[-1].ei = float(rs[7])
                        container[-1].profit = float(rs[8])
                        container[-1].fluctuation = float(rs[9].replace('\n', ''))
                    else:
                        container[-1].round = int(rs[7])
                        container[-1].generation = int(rs[8])
                        container[-1].count = int(rs[9].replace('\n', ''))
                    with open(path.join('./py_output/', self.version, self.sliding_window, rs[0] + '.csv'),
                              'a') as writer:
                        rs = re.findall(r'[A-Z]+\[\d+\]\(\d+\)\(\d+\)', rs[3])
                        for stock in rs:
                            group = re.search(r'([a-zA-Z]+)\[(\d+)\]\((\d+)\)\((\d+)\)', stock)
                            symbol = group.group(1)
                            stock_index = group.group(2)
                            amount = group.group(3)
                            allocated_funds = group.group(4)
                            writer.write(stock_index + ' ' + allocated_funds + '\n')
                            # print(symbol, amount, allocated_funds)
                            container[-1].stock_fund.append({symbol: int(allocated_funds)})
                            container[-1].stock_share.append({symbol: int(amount)})
                except Exception as e:
                    print(e, 'row:', row)
        return container

    def get_period_result(self, testing_period=False):
        pass


if __name__ == '__main__':
    p = 'C:/Users/Lab114/Desktop/DJI30 convergence/result/DJI30 ANGQTS-SR/'
    file_prefix = 'ANGQTS-SR_EWFA'
    for sw in sws:
        reader = Reader(_path=p, _ver='EWFA', _sliding_window=sw, _file_prefix=file_prefix)
        reader.get_final_result('final_result')
