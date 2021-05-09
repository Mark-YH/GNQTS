# coding: UTF-8
# Created by Mark Hsu on 2020/07/12.
#

import csv
from os import listdir

ver_dir = ["EWFA", "FA"]
sliding_windows = ["Y2Y", "Y2H", "Y2Q", "Y2M", "H2H", "H2Q", "H2M", "H#", "Q2Q", "Q2M", "Q#", "M2M", "M#"]
rs_training = [[], []]
rs_testing = [[], []]
rs_total_testing = [[], []]
rs_djia_total = None


class Result:
    def __init__(self, _alloc_type, _sw):
        self.alloc_type = _alloc_type
        self.sw = _sw
        self.tr = -1.0
        self.exp_return = -1.0
        self.risk = -1.0
        self.profit = -1.0
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
                    total_testing_search(path, file, version, sw)
                    print(path + file + " done.")
                elif file.__contains__("final_test_result"):
                    testing_search(path, file, version, sw)
                    print(path + file + " done.")


def djia_search(path, file):
    with open(path + file) as csvfile:
        rows = csv.reader(csvfile)
        global rs_djia_total
        rs_djia_total = Result('DJIA', 'DJIA')
        for row in rows:
            if row.__contains__('Real return'):
                rs_djia_total.profit = row[1]
            elif row.__contains__('Fluctuation'):
                rs_djia_total.fluctuation = row[1]
            elif row.__contains__('Fluctuation ratio') or row.__contains__('Emotion index'):
                rs_djia_total.emotion_index = row[1]
            elif row.__contains__('Expected return') or row.__contains__('Return'):
                rs_djia_total.exp_return = row[1]
            elif row.__contains__('Risk'):
                rs_djia_total.risk = row[1]
            elif row.__contains__('Global best') or row.__contains__('Trend ratio'):
                rs_djia_total.tr = row[1]


def total_testing_search(path, file, ver, sw):
    with open(path + file) as csvfile:
        rows = csv.reader(csvfile)
        rs = Result(ver, sw)
        for row in rows:
            if row.__contains__('Real return'):
                rs.profit = row[1]
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
                        rs_total_testing[i].append(rs)
                        break


def testing_search(path, file, ver, sw):
    with open(path + file) as csvfile:
        rows = csv.reader(csvfile)
        sum_risk = 0
        sum_exp_return = 0
        sum_tr = 0
        sum_fluc = 0
        sum_profit = 0
        sum_ei = 0
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
                    elif label.__contains__('Emotion index'):
                        col_ei = col_count
                    elif label.__contains__('Real return'):
                        col_profit = col_count
                    elif label.__contains__('Fluctuation'):
                        col_fluc = col_count
                    col_count += 1
            else:
                if len(row) == 10:  # data row
                    data_count += 1
                    if float(row[col_tr]) > 0:
                        sum_tr += float(row[col_tr])
                    elif float(row[col_tr]) < 0:
                        sum_tr += float(row[col_exp_return]) / float(row[col_risk])

                    sum_exp_return += float(row[col_exp_return])
                    sum_risk += float(row[col_risk])
                    sum_ei += float(row[col_ei])
                    sum_profit += float(row[col_profit])
                    sum_fluc += float(row[col_fluc])
        rs = Result(ver, sw)
        rs.tr = sum_tr / data_count
        rs.exp_return = sum_exp_return / data_count
        rs.risk = sum_risk / data_count
        rs.emotion_index = sum_ei / data_count
        rs.profit = sum_profit / data_count
        rs.fluctuation = sum_fluc / data_count
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


def write_testing(ver):
    with open('py_output/analysis_testing.csv', 'a+', newline='') as csvfile:
        writer = csv.writer(csvfile, delimiter=',')
        subject = ['真實報酬', '波動', '心情指數', '預期報酬', '風險', '趨勢值']
        title = []
        for s in subject:
            title.append(s)
            for v in ver:
                title.append(v)
        writer.writerow(title)

        for i in range(len(sliding_windows)):
            context = []
            for s in subject:
                context.append(sliding_windows[i])
                for index, v in enumerate(ver):
                    if s == '真實報酬':
                        context.append(rs_testing[index][i].profit)
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
            writer.writerow(context)


def write_training(ver):
    with open('py_output/analysis_training.csv', 'a+', newline='') as csvfile:
        writer = csv.writer(csvfile, delimiter=',')
        subject = ['預期報酬', '風險', '趨勢值']
        title = []
        for s in subject:
            title.append(s)
            for v in ver:
                title.append(v)
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
            writer.writerow(context)


def write_total_testing(ver):
    with open('py_output/analysis_total_testing.csv', 'a+', newline='') as csvfile:
        writer = csv.writer(csvfile, delimiter=',')
        subject = ['真實報酬', '波動', '心情指數', '預期報酬', '風險', '趨勢值']
        title = []
        for s in subject:
            title.append(s)
            for v in ver:
                title.append(v)
            if rs_djia_total is not None:
                title.append('DJIA')
        writer.writerow(title)
        for i in range(len(sliding_windows)):
            context = []
            for s in subject:
                context.append(sliding_windows[i])
                for index, v in enumerate(ver):
                    if s == '真實報酬':
                        context.append(rs_total_testing[index][i].profit)
                    elif s == '波動':
                        context.append(rs_total_testing[index][i].fluctuation)
                    elif s == '心情指數':
                        context.append(rs_total_testing[index][i].emotion_index)
                    elif s == '預期報酬':
                        context.append(rs_total_testing[index][i].exp_return)
                    elif s == '風險':
                        context.append(rs_total_testing[index][i].risk)
                    elif s == '趨勢值':
                        context.append(rs_total_testing[index][i].tr)
                if rs_djia_total is not None:
                    if s == '真實報酬':
                        context.append(rs_djia_total.profit)
                    elif s == '波動':
                        context.append(rs_djia_total.fluctuation)
                    elif s == '心情指數':
                        context.append(rs_djia_total.emotion_index)
                    elif s == '預期報酬':
                        context.append(rs_djia_total.exp_return)
                    elif s == '風險':
                        context.append(rs_djia_total.risk)
                    elif s == '趨勢值':
                        context.append(rs_djia_total.tr)
            writer.writerow(context)


if __name__ == '__main__':
    read()
    write_training(ver_dir)
    write_testing(ver_dir)
    write_total_testing(ver_dir)
