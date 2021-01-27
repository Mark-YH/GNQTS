# coding: UTF-8
# Created by Mark Hsu on 2020/07/12.
#

import csv
from os import listdir

ver_dir = ["EWFA", "FA"]
sliding_windows = ["Y2Y", "Y2H", "Y2Q", "Y2M", "H2H", "H2Q", "H2M", "H#", "Q2Q", "Q2M", "Q#", "M2M", "M#"]
rs_training = [[], []]
rs_testing = [[], []]
rs_djia = None


class Result:
    def __init__(self, _alloc_type, _sw):
        self.alloc_type = _alloc_type
        self.sw = _sw
        self.tr = -1.0
        self.exp_return = -1.0
        self.risk = -1.0
        self.real_return = -1.0
        self.fluctuation = -1.0
        self.emotion_index = -1.0


def read():
    root = "C:/Users/Lab114/Desktop/convergence/result/DJI30 ANGQTS/"
    djia_search(root, "DJIA_origin.csv")
    for sw in sliding_windows:
        for version in ver_dir:
            path = root + version + "/" + sw + "/"
            files = listdir(path)
            for file in files:
                if file.__contains__("final_result"):
                    training_search(path, file, version, sw)
                    print(path + file + " done.")
                elif file.__contains__("total_test"):
                    testing_search(path, file, version, sw)
                    print(path + file + " done.")


def djia_search(path, file):
    with open(path + file) as csvfile:
        rows = csv.reader(csvfile)
        global rs_djia
        rs_djia = Result('DJIA', 'DJIA')
        for row in rows:
            if row.__contains__('Real return'):
                rs_djia.real_return = row[1]
            elif row.__contains__('Fluctuation'):
                rs_djia.fluctuation = row[1]
            elif row.__contains__('Fluctuation ratio') or row.__contains__('Emotion index'):
                rs_djia.emotion_index = row[1]
            elif row.__contains__('Expected return') or row.__contains__('Return'):
                rs_djia.exp_return = row[1]
            elif row.__contains__('Risk'):
                rs_djia.risk = row[1]
            elif row.__contains__('Global best') or row.__contains__('Trend ratio'):
                rs_djia.tr = row[1]


def testing_search(path, file, ver, sw):
    with open(path + file) as csvfile:
        rows = csv.reader(csvfile)
        rs = Result(ver, sw)
        for row in rows:
            if row.__contains__('Real return'):
                rs.real_return = row[1]
            elif row.__contains__('Fluctuation'):
                rs.fluctuation = row[1]
            elif row.__contains__('Fluctuation ratio') or row.__contains__('Emotion index'):
                rs.emotion_index = row[1]
            elif row.__contains__('Expected return'):
                rs.exp_return = row[1]
            elif row.__contains__('Risk'):
                rs.risk = row[1]
            elif row.__contains__('Global best'):
                rs.tr = row[1]
                for i, value in enumerate(ver_dir):
                    if ver == value:
                        rs_testing[i].append(rs)
                        break


def training_search(path, file, ver, sw):
    with open(path + file) as csvfile:
        rows = csv.reader(csvfile)
        sum_risk = 0
        sum_exp_return = 0
        sum_tr = 0
        data_count = 0

        for row in rows:
            if rows.line_num == 1:
                col_count = 0
                for label in row:
                    if label.__contains__('Risk'):
                        col_risk = col_count
                    elif label.__contains__('Expected return'):
                        col_exp_return = col_count
                    elif label.__contains__('gBest'):
                        col_tr = col_count
                    col_count += 1
            else:
                if len(row) == 10:  # data row
                    data_count += 1
                    if float(row[col_tr]) > 0:
                        sum_tr += float(row[col_tr])
                        sum_exp_return += float(row[col_exp_return])
                        sum_risk += float(row[col_risk])
                elif len(row) == 2:  # end of file
                    rs = Result(ver, sw)
                    rs.tr = sum_tr / data_count
                    rs.exp_return = sum_exp_return / data_count
                    rs.risk = sum_risk / data_count
                    for i, value in enumerate(ver_dir):
                        if ver == value:
                            rs_training[i].append(rs)
                            break


def write_training(ver):
    with open('py_output/analysis_training.csv', 'a+', newline='') as csvfile:
        writer = csv.writer(csvfile, delimiter=',')
        subject = ['預期報酬', '風險', '趨勢值']
        title = []
        for s in subject:
            title.append(s)
            for v in ver:
                title.append(v)
            title.append('')
        writer.writerow(title)

        for i in range(len(sliding_windows)):
            context = []
            for s in subject:
                context.append(sliding_windows[i])
                for index, v in enumerate(ver):
                    if s == '預期報酬':
                        context.append(rs_training[index][i].exp_return)
                    elif s == '風險':
                        context.append(rs_training[index][i].risk)
                    elif s == '趨勢值':
                        context.append(rs_training[index][i].tr)
                context.append('')
            writer.writerow(context)


def write_testing(ver):
    with open('py_output/analysis_testing.csv', 'a+', newline='') as csvfile:
        writer = csv.writer(csvfile, delimiter=',')
        subject = ['真實報酬', '波動', '心情指數', '預期報酬', '風險', '趨勢值']
        title = []
        for s in subject:
            title.append(s)
            for v in ver:
                title.append(v)
            if rs_djia is not None:
                title.append('DJIA')
        writer.writerow(title)
        for i in range(len(sliding_windows)):
            context = []
            for s in subject:
                context.append(sliding_windows[i])
                for index, v in enumerate(ver):
                    if s == '真實報酬':
                        context.append(rs_testing[index][i].real_return)
                    elif s == '波動':
                        context.append(rs_testing[index][i].fluctuation)
                    elif s == '心情指數':
                        context.append(rs_testing[index][i].emotion_index)
                    elif s == '預期報酬':
                        context.append(rs_testing[index][i].exp_return)
                    elif s == '風險':
                        context.append(rs_testing[index][i].risk)
                    elif s == '趨勢值':
                        context.append(rs_testing[index][i].tr)
                if rs_djia is not None:
                    if s == '真實報酬':
                        context.append(rs_djia.real_return)
                    elif s == '波動':
                        context.append(rs_djia.fluctuation)
                    elif s == '心情指數':
                        context.append(rs_djia.emotion_index)
                    elif s == '預期報酬':
                        context.append(rs_djia.exp_return)
                    elif s == '風險':
                        context.append(rs_djia.risk)
                    elif s == '趨勢值':
                        context.append(rs_djia.tr)
            writer.writerow(context)


if __name__ == '__main__':
    read()
    write_training(ver_dir)
    write_testing(ver_dir)
