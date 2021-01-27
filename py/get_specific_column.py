# coding: UTF-8
# Created by Mark Hsu on 2020/11/12
#
"""
This file ...
"""
from os import path
import matplotlib.pyplot as plt
import numpy as np

paths = ['C:/Users/Lab114/Desktop/DJI30 convergence/result/DJI30 ANGQTS',
         'C:/Users/Lab114/Desktop/DJI30 convergence/result/DJI30 GNQTS']
ver = ['EWFA', 'FA']
alias = ['ANGQTS_', 'GNQTS_']
sliding_windows = ["Y2Y", "Y2H", "Y2Q", "Y2M", "H2H", "H2Q", "H2M", "H#", "Q2Q", "Q2M", "Q#", "M2M", "M#"]
specific_columns = ['at round', 'at generation', 'Found best count']


def read():
    ANGQTS_EWFA = {'round': [], 'gen': [], 'count': []}
    ANGQTS_FA = {'round': [], 'gen': [], 'count': []}
    GNQTS_EWFA = {'round': [], 'gen': [], 'count': []}
    GNQTS_FA = {'round': [], 'gen': [], 'count': []}
    for v in ver:
        for i_alias, p in enumerate(paths):
            for sw in sliding_windows:
                file_path = path.join(path.realpath(p), v, sw, alias[i_alias] + v + '_' + sw + '_final_result.csv')
                with open(file_path) as csvfile:
                    i_round = 0
                    avg_round = 0
                    i_gen = 0
                    avg_gen = 0
                    i_best_count = 0
                    avg_best_count = 0
                    period_count = 0
                    for i, row in enumerate(csvfile):
                        if i == 0:
                            for ic, c in enumerate(str(row).split(',')):
                                if c.replace('\n', '') == specific_columns[0]:
                                    i_round = ic
                                elif c.replace('\n', '') == specific_columns[1]:
                                    i_gen = ic
                                elif c.replace('\n', '') == specific_columns[2]:
                                    i_best_count = ic
                        else:
                            try:
                                gen = int(str(row).split(',')[i_gen])
                                bc = int(str(row).split(',')[i_best_count])
                                round = int(str(row).split(',')[i_round])
                                if int(str(row).split(',')[2]) == 0:
                                    continue
                                avg_gen += gen
                                avg_best_count += bc
                                avg_round += round
                                period_count += 1
                            except Exception as e:
                                # print(e, ':', str(row).replace('\n', '\\n'))
                                pass
                    avg_gen /= period_count
                    avg_best_count /= period_count
                    avg_round /= period_count
                    print(
                        '{}, Periods count: {}, avg round: {:.2f}, avg generation: {:.2f}, avg best count: {:.2f}'.format(
                            file_path, period_count, avg_round, avg_gen, avg_best_count))
                    if v == 'FA' and i_alias == 0:
                        ANGQTS_FA['round'].append(avg_round)
                        ANGQTS_FA['gen'].append(avg_gen)
                        ANGQTS_FA['count'].append(avg_best_count)
                    elif v == 'FA' and i_alias == 1:
                        GNQTS_FA['round'].append(avg_round)
                        GNQTS_FA['gen'].append(avg_gen)
                        GNQTS_FA['count'].append(avg_best_count)
                    elif v == 'EWFA' and i_alias == 0:
                        ANGQTS_EWFA['round'].append(avg_round)
                        ANGQTS_EWFA['gen'].append(avg_gen)
                        ANGQTS_EWFA['count'].append(avg_best_count)
                    elif v == 'EWFA' and i_alias == 1:
                        GNQTS_EWFA['round'].append(avg_round)
                        GNQTS_EWFA['gen'].append(avg_gen)
                        GNQTS_EWFA['count'].append(avg_best_count)
    write([ANGQTS_EWFA, GNQTS_EWFA, ANGQTS_FA, GNQTS_FA])


def write(data):
    sliding_windows = ['Y2Y', 'Y2H', 'Y2Q', 'Y2M', 'H2H', 'H2Q', 'H2M', 'H#', 'Q2Q', 'Q2M', 'Q#', 'M2M', 'M#']
    with open('./py_output/results.csv', 'w') as writer:
        for v in ver:
            for i_alias, p in enumerate(paths):
                writer.write(',' + alias[i_alias] + v + '_avg. round,')
                writer.write(alias[i_alias] + v + '_avg. generation,')
                writer.write(alias[i_alias] + v + '_avg. count')
        writer.write('\n')
        for i, sw in enumerate(sliding_windows):
            writer.write(sw + ',')
            for d in data:
                writer.write(str(d['round'][i]))
                writer.write(',')
                writer.write(str(d['gen'][i]))
                writer.write(',')
                writer.write(str(d['count'][i]))
                writer.write(',')
            writer.write('\n')
    plot(data)


def sort(data, sorted_indices):
    sorted_data = []
    for i in sorted_indices:
        sorted_data.append(data[i])
    return reversed(sorted_data)


def plot(data):
    index = np.arange(0, 14)

    for i in range(len(data)):
        data[i]['round'].append(np.mean(data[i]['round']))
        data[i]['gen'].append(np.mean(data[i]['gen']))
        data[i]['count'].append(np.mean(data[i]['count']))
    xlabels = ['Y2Y', 'Y2H', 'Y2Q', 'Y2M', 'H2H', 'H2Q', 'H2M', 'H*', 'Q2Q', 'Q2M', 'Q*', 'M2M', 'M*', 'Mean']
    sorted_indices = np.argsort(data[0]['round'][0:-1])
    data[0]['round'][0:-1] = sort(data[0]['round'], sorted_indices)
    data[1]['round'][0:-1] = sort(data[1]['round'], sorted_indices)
    xlabels[0:-1] = sort(xlabels, sorted_indices)
    plt.figure(figsize=(7.2, 4.8))
    plt.plot(data[0]['round'], marker='+', alpha=.8, label='ANGQTS-EWFA', markersize=10)
    plt.plot(data[1]['round'], marker='x', alpha=.8, label='GNQTS-EWFA', markersize=10)
    plt.legend(fontsize=14)
    plt.title('Average round', fontsize=14)
    plt.ylabel("Round", fontsize=14)
    plt.xlabel("Sliding windows", fontsize=14)
    plt.xticks(index + .3 / 2, xlabels)
    plt.tight_layout()
    p = './py_output/'
    extension = '.svg'
    plt.savefig(p + 'Round-EWFA' + extension)
    extension = '.pdf'
    plt.savefig(p + 'Round-EWFA' + extension)
    # plt.show()
    plt.close()

    xlabels = ['Y2Y', 'Y2H', 'Y2Q', 'Y2M', 'H2H', 'H2Q', 'H2M', 'H*', 'Q2Q', 'Q2M', 'Q*', 'M2M', 'M*', 'Mean']
    sorted_indices = np.argsort(data[2]['round'][0:-1])
    data[2]['round'][0:-1] = sort(data[2]['round'], sorted_indices)
    data[3]['round'][0:-1] = sort(data[3]['round'], sorted_indices)
    xlabels[0:-1] = sort(xlabels, sorted_indices)
    plt.figure(figsize=(7.2, 4.8))
    plt.plot(data[2]['round'], marker='+', alpha=.8, label='ANGQTS-FA', markersize=10)
    plt.plot(data[3]['round'], marker='x', alpha=.8, label='GNQTS-FA', markersize=10)
    plt.legend(fontsize=14)
    plt.title('Average round', fontsize=14)
    plt.ylabel("Round", fontsize=14)
    plt.xlabel("Sliding windows", fontsize=14)
    plt.xticks(index + .3 / 2, xlabels)
    plt.tight_layout()
    extension = '.svg'
    plt.savefig(p + 'Round-FA' + extension)
    extension = '.pdf'
    plt.savefig(p + 'Round-FA' + extension)
    # plt.show()
    plt.close()

    xlabels = ['Y2Y', 'Y2H', 'Y2Q', 'Y2M', 'H2H', 'H2Q', 'H2M', 'H*', 'Q2Q', 'Q2M', 'Q*', 'M2M', 'M*', 'Mean']
    sorted_indices = np.argsort(data[0]['gen'][0:-1])
    data[0]['gen'][0:-1] = sort(data[0]['gen'], sorted_indices)
    data[1]['gen'][0:-1] = sort(data[1]['gen'], sorted_indices)
    xlabels[0:-1] = sort(xlabels, sorted_indices)
    plt.figure(figsize=(7.2, 4.8))
    plt.plot(data[0]['gen'], marker='+', alpha=.8, label='ANGQTS-EWFA', markersize=10)
    plt.plot(data[1]['gen'], marker='x', alpha=.8, label='GNQTS-EWFA', markersize=10)
    plt.legend(fontsize=14)
    plt.title('Average generation', fontsize=14)
    plt.ylabel("Generation", fontsize=14)
    plt.xlabel("Sliding windows", fontsize=14)
    plt.xticks(index + .3 / 2, xlabels)
    plt.tight_layout()
    p = './py_output/'
    extension = '.svg'
    plt.savefig(p + 'Generation-EWFA' + extension)
    extension = '.pdf'
    plt.savefig(p + 'Generation-EWFA' + extension)
    # plt.show()
    plt.close()

    xlabels = ['Y2Y', 'Y2H', 'Y2Q', 'Y2M', 'H2H', 'H2Q', 'H2M', 'H*', 'Q2Q', 'Q2M', 'Q*', 'M2M', 'M*', 'Mean']
    sorted_indices = np.argsort(data[2]['gen'][0:-1])
    data[2]['gen'][0:-1] = sort(data[2]['gen'], sorted_indices)
    data[3]['gen'][0:-1] = sort(data[3]['gen'], sorted_indices)
    xlabels[0:-1] = sort(xlabels, sorted_indices)
    plt.figure(figsize=(7.2, 4.8))
    plt.plot(data[2]['gen'], marker='+', alpha=.8, label='ANGQTS-FA', markersize=10)
    plt.plot(data[3]['gen'], marker='x', alpha=.8, label='GNQTS-FA', markersize=10)
    plt.legend(fontsize=14)
    plt.title('Average generation', fontsize=14)
    plt.ylabel("Generation", fontsize=14)
    plt.xlabel("Sliding windows", fontsize=14)
    plt.xticks(index + .3 / 2, xlabels)
    plt.tight_layout()
    extension = '.svg'
    plt.savefig(p + 'Generation-FA' + extension)
    extension = '.pdf'
    plt.savefig(p + 'Generation-FA' + extension)
    # plt.show()
    plt.close()

    xlabels = ['Y2Y', 'Y2H', 'Y2Q', 'Y2M', 'H2H', 'H2Q', 'H2M', 'H*', 'Q2Q', 'Q2M', 'Q*', 'M2M', 'M*', 'Mean']
    sorted_indices = np.argsort(data[0]['count'][0:-1])
    data[0]['count'][0:-1] = sort(data[0]['count'], sorted_indices)
    data[1]['count'][0:-1] = sort(data[1]['count'], sorted_indices)
    xlabels[0:-1] = sort(xlabels, sorted_indices)
    plt.figure(figsize=(7.2, 4.8))
    plt.plot(data[0]['count'], marker='+', alpha=.8, label='ANGQTS-EWFA', markersize=10)
    plt.plot(data[1]['count'], marker='x', alpha=.8, label='GNQTS-EWFA', markersize=10)
    plt.legend(fontsize=14)
    plt.title('Average count', fontsize=14)
    plt.ylabel("Count", fontsize=14)
    plt.xlabel("Sliding windows", fontsize=14)
    plt.xticks(index + .3 / 2, xlabels)
    plt.tight_layout()
    p = './py_output/'
    extension = '.svg'
    plt.savefig(p + 'Count-EWFA' + extension)
    extension = '.pdf'
    plt.savefig(p + 'Count-EWFA' + extension)
    # plt.show()
    plt.close()

    xlabels = ['Y2Y', 'Y2H', 'Y2Q', 'Y2M', 'H2H', 'H2Q', 'H2M', 'H*', 'Q2Q', 'Q2M', 'Q*', 'M2M', 'M*', 'Mean']
    sorted_indices = np.argsort(data[2]['count'][0:-1])
    data[2]['count'][0:-1] = sort(data[2]['count'], sorted_indices)
    data[3]['count'][0:-1] = sort(data[3]['count'], sorted_indices)
    xlabels[0:-1] = sort(xlabels, sorted_indices)
    plt.figure(figsize=(7.2, 4.8))
    plt.plot(data[2]['count'], marker='+', alpha=.8, label='ANGQTS-FA', markersize=10)
    plt.plot(data[3]['count'], marker='x', alpha=.8, label='GNQTS-FA', markersize=10)
    plt.legend(fontsize=14)
    plt.title('Average count', fontsize=14)
    plt.ylabel("Count", fontsize=14)
    plt.xlabel("Sliding windows", fontsize=14)
    plt.xticks(index + .3 / 2, xlabels)
    plt.tight_layout()
    extension = '.svg'
    plt.savefig(p + 'Count-FA' + extension)
    extension = '.pdf'
    plt.savefig(p + 'Count-FA' + extension)
    # plt.show()
    plt.close()


if __name__ == '__main__':
    read()
