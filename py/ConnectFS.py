#
# Created by Mark Hsu on 2020/06/19.
#
"""
This program is used to generate a continuous funds standardization in training period of a particular sliding window.
"""
import csv
from os import listdir

ver_dir = ["分配", "平分"]
sliding_windows = ["Y2Y", "Y2H", "Y2Q", "Y2M", "H2H", "H2Q", "H2M", "H#", "Q2Q", "Q2M", "Q#", "M2M", "M#"]
fs = 1


def increment():
    global fs
    fs += 1


def read():
    # path = "./data/"
    for version in ver_dir:
        for sw in sliding_windows:
            path = "C:/Users/Lab114/Desktop/DJI30 測試期無複利/" + version + "/" + sw + "/"
            global fs
            fs = 1
            print(path)
            files = listdir(path)
            for file in files:
                if file.__contains__("output_train"):
                    p = path + file
                    write(p, path)
                    print(file + " done.")


def write(p, write_path):
    with open(p) as csvfile:
        rows = csv.reader(csvfile)
        for row in rows:
            if row != [] and row[0].__contains__('FS('):
                with open(write_path + 'funds_standardization.csv', 'a+', newline='') as csvfile:
                    writer = csv.writer(csvfile, delimiter=',')
                    writer.writerow(["FS(" + str(fs) + ")", row[len(row) - 2]])
                    increment()


if __name__ == '__main__':
    read()
