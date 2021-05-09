# coding: UTF-8
# Created by Mark Hsu on 2021/5/9
#
"""
This file ...
"""
from plot_fs import *
import matplotlib.pyplot as plt


def twin_plot(FA, SR, title, output_path):
    plt.rcParams['font.family'] = "Times New Roman"
    # plt.rcParams['font.size'] = 14
    fig, ax = plt.subplots(1, 1)
    fig.set_size_inches(7.2, 4.8)
    ax2 = ax.twinx()
    ln = ax.plot(FA, label='ANGQTS-FA', c=(1, 0.5, 0.2))
    ln2 = ax.plot(get_first_last_line(FA), label='ANGQTS-FA FL.', linestyle='dashed', c=(1, 0.5, 0.2))
    ln3 = ax2.plot(SR, label='ANGQTS-SR', c=(0.2, 0.6, 0.4))
    ln4 = ax2.plot(get_first_last_line(SR), label='ANGQTS-SR FL.', linestyle='dashed', c=(0.2, 0.6, 0.4))
    ax.set_ylabel('Funds Standardization (ANGQTS-FA)', fontsize=14)
    ax2.set_ylabel('Funds Standardization (ANGQTS-SR)', fontsize=14)

    # ax.set_ylim([0.8e7, 2.0e7])
    # ax2.set_ylim([3.43e7, 4.5e7])
    lns = ln + ln2 + ln3 + ln4

    labels = [l.get_label() for l in lns]
    ax.legend(lns, labels, loc='lower right', fontsize=14)
    ax.set_xlabel('Day', fontsize=14)
    plt.title(title, fontsize=14)
    plt.tight_layout()
    ext = '.pdf'
    plt.savefig(output_path + ext)
    ext = '.svg'
    plt.savefig(output_path + ext)
    plt.clf()
    plt.close(fig)


if __name__ == '__main__':
    dict_files = get_files()
    for sw, files in dict_files.items():
        if sw != 'M#':
            continue
        for fn in files:
            fp1 = Config.paths[0] + sw + '/' + Config.file_prefix[0] + '_' + fn
            fp2 = Config.paths[1] + sw + '/' + Config.file_prefix[1] + '_' + fn
            FA = get_fs(fp1)
            SR = get_fs(fp2)
            p = './img/test/' + sw + '/'
            os.makedirs(p, exist_ok=True)
            twin_plot(FA, SR, get_title(sw, fn), p + fn.replace('.csv', ''))
