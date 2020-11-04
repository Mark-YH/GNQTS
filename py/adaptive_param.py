import matplotlib.pyplot as plt
import numpy as np

X = []
Y = []
Y2 = []
upper = 0.0008
lower = 0.0002


def print_data():
    for x, y, y2 in zip(X, Y, Y2):
        print('x = %.3f' % x, 'Y = %.8f' % y, 'linear = %.8f' % y2)


def f(x):
    return np.power(2, -x * 10) * (upper - lower) + lower


def linear(x):
    return upper - 2 * x * (upper - lower)


if __name__ == '__main__':
    for _x in range(0, 501):
        X.append(_x / 1000)

    for _x in X:
        Y.append(f(_x))
        Y2.append(linear(_x))

    print_data()
    plt.plot(X, Y, label='exponential')
    plt.plot(X, Y2, label='linear')
    plt.legend()
    plt.xlabel(r'$x$')
    plt.ylabel(r"$\theta$")
    plt.show()
