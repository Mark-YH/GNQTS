# coding: UTF-8
# Created by Mark Hsu on 2020/10/24
#
import matplotlib.pyplot as plt
import re
import os


class Config:
    paths = ['C:/Users/Lab114/Desktop/DJI30 convergence/result/DJI30 ANGQTS/EWFA/',
             # 'C:/Users/Lab114/Desktop/DJI30 convergence/result/DJI30 ANGQTS/FA/',
             # 'C:/Users/Lab114/Desktop/DJI30 convergence/result/DJI30 ANGQTS-SR/EWFA/'
             ]
    labels = ['ANGQTS-EWFA', 'ANGQTS-FA', 'ANGQTS-SR']
    file_prefix = ['ANGQTS_EWFA', 'ANGQTS_FA', 'ANGQTS-SR_EWFA']
    colors = [(0.2, 0.4, 1), (1, 0.5, 0.2), (0.2, 0.6, 0.4)]
    mode = 'train'
    sliding_windows = [
        'Y2Y',
        'Y2H',
        'Y2Q',
        'Y2M',
        'H2H',
        'H2Q',
        'H2M',
        'H#',
        'Q2Q',
        'Q2M',
        'Q#',
        'M2M',
        'M#'
    ]


def get_trend_line(_fs):
    origin = 1e7
    x = _fs
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

    return trend_line


def get_files():
    results = {}
    for i, path in enumerate(Config.paths):
        for sw in Config.sliding_windows:
            _files = []
            for item in os.listdir(path + sw):
                if re.match(r'.+_' + Config.mode, item) and not item.__contains__('convergence'):
                    _files.append(Config.mode + re.split(r'.+_' + Config.mode, item)[1])
            results.update({sw: _files})
    return results


def get_fs(file_path):
    fs = []
    with open(file_path, 'r') as file:
        no_invest = False
        col = 0
        for row in file:
            if re.match(r'Number of chosen,\d+', row):
                result = row.split(',')
                if int(result[1]) == 0:
                    no_invest = True
            if re.match(r'Balance of each stock,+', row):
                result = row.split(',')
                col = len(result) - 1
            if re.match(r'FS\(\d+\)', row):
                result = row.split(',')
                if no_invest:
                    fs.append(float(result[1]))
                else:
                    fs.append(float(result[col]))
    return fs


def add_fs(_fs, label='?', color=None, first_to_last=None):
    # avg = []
    # for i in range(len(_fs)):
    #     avg.append(np.average(_fs))

    plt.plot(_fs, label=label, color=color)
    # plt.plot(avg, label=label + ' avg.', linestyle='dotted', color=color, alpha=0.7)
    if first_to_last is None:
        plt.plot(get_trend_line(_fs), label=label + ' TL.', linestyle='dashed', color=color, alpha=0.5)
    else:
        plt.plot(first_to_last, label=label + ' FL.', linestyle='dashed', color=color, alpha=0.5)


def save_fs(sw, filename):
    if filename.__contains__('test'):
        mode = 'test'
    if filename.__contains__('train'):
        mode = 'train'
    if filename.__contains__('total'):
        mode = 'total test'

    os.makedirs('./img/' + mode + '/' + sw, exist_ok=True)
    plt.title(get_title(sw, filename), fontsize=14)
    plt.xlabel('Day', fontsize=14)
    plt.ylabel('Funds Standardization', fontsize=14)
    plt.legend(fontsize=12)
    plt.tight_layout()
    extension = '.svg'
    plt.savefig('img/' + mode + '/' + sw + '/' + filename.split('.csv')[0] + extension)
    extension = '.pdf'
    plt.savefig('img/' + mode + '/' + sw + '/' + filename.split('.csv')[0] + extension)
    # plt.show()
    plt.clf()


def get_title(sw, filename):
    months = {'01': 'Jan', '02': 'Feb', '03': 'Mar', '04': 'Apr', '05': 'May', '06': 'Jun', '07': 'Jul', '08': 'Aug',
              '09': 'Sep', '10': 'Oct', '11': 'Nov', '12': 'Dec'}
    period = re.split(r'(test|train)_|\(.+\)\.csv', filename)[2]
    period = period.replace('_', ' ').replace('~', ' - ')
    for key, val in months.items():
        period = period.replace(' ' + key, ' ' + val).replace('-' + key, '-' + val)
    if Config.mode == 'total':
        return sw.replace('#', '*')
    return sw.replace('#', '*') + ' $-$ ' + period


def run():
    dict_files = get_files()
    for sw, files in dict_files.items():
        for fn in files:
            for i, path in enumerate(Config.paths):
                fp = path + sw + '/' + Config.file_prefix[i] + '_' + fn
                add_fs(get_fs(fp), Config.labels[i], Config.colors[i])
            save_fs(sw, fn)


def single():
    for i, path in enumerate(Config.paths):
        fp = path + 'Y2Y/ANGQTS_EWFA_train_2010(2010 Q1).csv'
        add_fs(get_fs(fp), Config.labels[i], Config.colors[i])
    save_fs('Y2Y', 'ANGQTS_EWFA_train_2010(2010 Q1).csv')


def total_testing_period():
    dict_files = get_files()
    for sw, files in dict_files.items():
        for fn in files:
            for i, path in enumerate(Config.paths):
                fp = path + sw + '/' + Config.file_prefix[i] + '_' + sw + '_' + fn
                fs = []
                first_to_last = []
                with open(fp) as reader:
                    for row in reader:
                        if re.match(r'FS\(\d+\)', row):
                            result = row.split(',')
                            fs.append(float(result[1]))
                            first_to_last.append(float(result[3]))
                add_fs(fs, Config.labels[i], Config.colors[i], first_to_last)
            save_fs(sw, fn)


def study_case():
    files = ['study case 1 - H2Q - 2011 Q1-Q2.csv', 'study case 2 - Y2Y - 2011.csv', 'study case 3 - M2M 2013 09.csv']
    titles = ['H2Q $-$ 2011 Q1-Q2', 'Y2Y $-$ 2011', 'M2M $-$ 2013 Sep']
    for k, file in enumerate(files):
        with open('C:/Users/Lab114/Desktop/DJI30 convergence/result/' + file) as reader:
            data = []
            for i, row in enumerate(reader):
                if i == 0:
                    cols = row.split(',')
                    for j in range(len(cols)):
                        cols[j] = str(cols[j]).replace('\n', '')
                        data.append([])
                    continue
                for j, val in enumerate(row.split(',')):
                    data[j].append(float(str(val).replace('\n', '')))

            for i in range(len(data) - 1):
                plt.plot(data[i], label=cols[i], alpha=.5)
            plt.plot(data[-1], label='Portfolio', alpha=.8, color='tab:red')
            plt.plot(get_trend_line(data[-1]), label='Portfolio TL.', alpha=.5, color='tab:red', linestyle='dashed')
            plt.xlabel('Day', fontsize=14)
            plt.ylabel('Funds standardization', fontsize=14)
            plt.title(titles[k], fontsize=14)
            plt.legend(fontsize=10, ncol=1)
            extension = '.svg'
            plt.savefig('./py_output/' + titles[k].replace('$', '') + extension)
            extension = '.pdf'
            plt.savefig('./py_output/' + titles[k].replace('$', '') + extension)
            plt.show()


if __name__ == '__main__':
    plt.figure(figsize=(7.2, 4.8))
    # run()
    # single()
    # if Config.mode == 'total':
    #     total_testing_period()
    study_case()
