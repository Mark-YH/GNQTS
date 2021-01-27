# coding: UTF-8
# Created by Mark Hsu on 2021/1/23
#
"""
This file ...
"""
import matplotlib.pyplot as plt
import numpy as np
import os

objective_number = 3


def read_training_period():
    p = 'C:/Users/Lab114/Desktop/DJI30 convergence/result/training period_tr_sr.csv'
    ANGQTS_EWFA = {'return': [], 'risk': [], 'tr': []}
    ANGQTS_FA = {'return': [], 'risk': [], 'tr': []}
    ANGQTS_SR = {'return': [], 'risk': [], 'tr': []}
    with open(p, 'r', encoding='utf-8') as reader:
        for i, row in enumerate(reader):
            if i < 2:
                continue
            for j, value in enumerate(str(row).split(',')):
                if np.floor(j / (objective_number + 1)) == 0:
                    t = 'return'
                elif np.floor(j / (objective_number + 1)) == 1:
                    t = 'risk'
                elif np.floor(j / (objective_number + 1)) == 2:
                    t = 'tr'
                if j % (objective_number + 1) == 1:
                    ANGQTS_EWFA[t].append(float(value))
                elif j % (objective_number + 1) == 2:
                    ANGQTS_FA[t].append(float(value))
                elif j % (objective_number + 1) == 3:
                    ANGQTS_SR[t].append(float(value))
        t = 'return'
        data = [ANGQTS_EWFA[t], ANGQTS_FA[t], ANGQTS_SR[t]]
        plot('Training Period', 'Expected return', 'Training period/Expected return', data, sort_descend=True)
        t = 'risk'
        data = [ANGQTS_EWFA[t], ANGQTS_FA[t], ANGQTS_SR[t]]
        plot('Training Period', 'Risk', 'Training period/Risk', data, sort_descend=False)
        t = 'tr'
        data = [ANGQTS_EWFA[t], ANGQTS_FA[t], ANGQTS_SR[t]]
        plot('Training Period', 'Trend ratio', 'Training period/Trend ratio', data, sort_descend=True)


def read_testing_period():
    p = 'C:/Users/Lab114/Desktop/DJI30 convergence/result/testing period_tr_sr.csv'
    ANGQTS_EWFA = {'profit': [], 'fluc': [], 'ei': [], 'return': [], 'risk': [], 'tr': []}
    ANGQTS_FA = {'profit': [], 'fluc': [], 'ei': [], 'return': [], 'risk': [], 'tr': []}
    ANGQTS_SR = {'profit': [], 'fluc': [], 'ei': [], 'return': [], 'risk': [], 'tr': []}
    DJIA = {'profit': [], 'fluc': [], 'ei': [], 'return': [], 'risk': [], 'tr': []}

    with open(p, 'r') as reader:
        for i, row in enumerate(reader):
            if i < 2:
                continue
            for j, value in enumerate(str(row).split(',')):
                if np.floor(j / (objective_number + 2)) == 0:
                    t = 'profit'
                elif np.floor(j / (objective_number + 2)) == 1:
                    t = 'fluc'
                elif np.floor(j / (objective_number + 2)) == 2:
                    t = 'ei'
                elif np.floor(j / (objective_number + 2)) == 3:
                    t = 'return'
                elif np.floor(j / (objective_number + 2)) == 4:
                    t = 'risk'
                elif np.floor(j / (objective_number + 2)) == 5:
                    t = 'tr'
                if j % (objective_number + 2) == 1:
                    ANGQTS_EWFA[t].append(float(value))
                elif j % (objective_number + 2) == 2:
                    ANGQTS_FA[t].append(float(value))
                elif j % (objective_number + 2) == 3:
                    ANGQTS_SR[t].append(float(value))
                elif j % (objective_number + 2) == 4:
                    DJIA[t].append(float(value))
        t = 'profit'
        data = [ANGQTS_EWFA[t], ANGQTS_FA[t], ANGQTS_SR[t], DJIA[t]]
        plot('Overall Testing Period', 'Profit', 'Testing period/Profit', data, sort_descend=True)
        t = 'fluc'
        data = [ANGQTS_EWFA[t], ANGQTS_FA[t], ANGQTS_SR[t], DJIA[t]]
        plot('Overall Testing Period', 'Fluctuation', 'Testing period/Fluctuation', data, sort_descend=False)
        t = 'ei'
        data = [ANGQTS_EWFA[t], ANGQTS_FA[t], ANGQTS_SR[t], DJIA[t]]
        plot('Overall Testing Period', 'Emotion index', 'Testing period/Emotion index', data, sort_descend=True)
        t = 'return'
        data = [ANGQTS_EWFA[t], ANGQTS_FA[t], ANGQTS_SR[t], DJIA[t]]
        plot('Overall Testing Period', 'Expected return', 'Testing period/Expected return', data, sort_descend=True)
        t = 'risk'
        data = [ANGQTS_EWFA[t], ANGQTS_FA[t], ANGQTS_SR[t], DJIA[t]]
        plot('Overall Testing Period', 'Risk', 'Testing period/Risk', data, sort_descend=False)
        t = 'tr'
        data = [ANGQTS_EWFA[t], ANGQTS_FA[t], ANGQTS_SR[t], DJIA[t]]
        plot('Overall Testing Period', 'Trend ratio', 'Testing period/Trend ratio', data, sort_descend=True)


def sort(data, sorted_indices, sort_descend):
    sorted_data = []
    for i in sorted_indices:
        sorted_data.append(data[i])
    if sort_descend:
        return reversed(sorted_data)
    return sorted_data


def plot(title, y_label, filename, data, sort_descend=None):
    col_count = 14
    index = np.arange(0, col_count, 1)
    ANGQTS_EWFA = data[0]
    ANGQTS_FA = data[1]
    ANGQTS_SR = data[2]
    if len(data) == objective_number + 1:
        DJIA = data[objective_number]
    xlabels = ['Y2Y', 'Y2H', 'Y2Q', 'Y2M', 'H2H', 'H2Q', 'H2M', 'H*', 'Q2Q', 'Q2M', 'Q*', 'M2M', 'M*', 'Mean']

    if sort_descend is not None:
        sorted_indices = np.argsort(ANGQTS_FA[0:-1])
        ANGQTS_FA[0:-1] = sort(ANGQTS_FA, sorted_indices, sort_descend)
        ANGQTS_EWFA[0:-1] = sort(ANGQTS_EWFA, sorted_indices, sort_descend)
        ANGQTS_SR[0:-1] = sort(ANGQTS_SR, sorted_indices, sort_descend)
        xlabels[0:-1] = sort(xlabels, sorted_indices, sort_descend)

    plt.figure(figsize=(7.2, 4.8))
    plt.plot(ANGQTS_EWFA, alpha=.8, label="ANGQTS-EWFA", marker='x')
    plt.plot(ANGQTS_FA, alpha=.8, label="ANGQTS-FA", marker='o')
    plt.plot(ANGQTS_SR, alpha=.8, label="ANGQTS-SR", marker='+')
    if len(data) == objective_number + 1:
        plt.plot(DJIA, alpha=.6, label="DJIA")
    plt.ylabel(y_label, fontsize=14)
    plt.xlabel("Sliding windows", fontsize=14)
    plt.xticks(index + .3 / 2, xlabels)
    plt.ticklabel_format(axis="y", style="sci", scilimits=(0, 0))
    plt.legend(fontsize=12, ncol=1)
    plt.title(title, fontsize=14)
    plt.tight_layout()
    file_path = './py_output/'
    extension = '.svg'
    os.makedirs(file_path + filename.split('/')[0], exist_ok=True)
    file_path += filename
    plt.savefig(file_path + extension)
    extension = '.pdf'
    plt.savefig(file_path + extension)
    # plt.show()
    plt.close()


def run():
    read_testing_period()
    read_training_period()


if __name__ == '__main__':
    run()
