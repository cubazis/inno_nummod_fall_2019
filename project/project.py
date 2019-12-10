import matplotlib.pyplot as plt
import mpl_toolkits.axes_grid1 as axes_grid1
import numpy as np
import scipy.sparse


def is_in_house(x, y):
    box_houses = [
        (69, 111),
        (69, 264),
        (138, 12),
        (138, 165),
        (192, 111),
        (192, 264),
        (261, 12),
        (261, 165)
    ]

    bottom_corner_houses = [
        (69, 12),
        (69, 165),
        (192, 12),
        (192, 165)
    ]

    top_corner_houses = [
        (138, 51),
        (138, 204),
        (261, 51),
        (261, 204)
    ]

    for house in box_houses:
        if house[0] < x < house[0] + 18 and house[1] < y < house[1] + 18:
            return True

    for house in bottom_corner_houses:
        if (house[0] < x < house[0] + 18 and house[1] < y < house[1] + 78) or \
                (house[0] + 12 < x < house[0] + 24 and house[1] < y < house[1] + 18):
            return True

    for house in top_corner_houses:
        if (house[0] < x < house[0] + 18 and house[1] < y < house[1] + 78) or \
                (house[0] - 12 < x < house[0] + 1 and house[1] + 60 < y < house[1] + 78):
            return True
    return False


def run(max_number_of_steps=50000, eps=1e-10):
    n = 300
    N = n * n
    ans = np.zeros((N, 1))

    for i in range(0, N, 300):
        ans[i, 0] = 1

    a = np.zeros(N)
    d = [(1, 0), (-1, 0), (0, 1), (0, -1)]
    k, lambda1, lambda2 = 0.5, 1, 0
    h = 1 / n
    tau = h ** 2 / (4 * k)
    cfs = [
        1 - 4 * (tau * k) / (h ** 2),
        tau * (k / (h ** 2) - lambda1 / (2 * h)),
        tau * (k / (h ** 2) + lambda1 / (2 * h)),
        tau * (k / (h ** 2) - lambda2 / (2 * h)),
        tau * (k / (h ** 2) + lambda2 / (2 * h))
    ]

    A = scipy.sparse.lil_matrix((N, N))

    for i in range(0, N):
        x_i = int(i / 300)
        y_i = i % 300
        A[i, i] = cfs[0]
        for j in range(0, 4):
            cf = cfs[j + 1]
            x1 = x_i + d[j][0]
            if x1 == 0:
                a[i] -= cf
                continue

            y1 = y_i + d[j][1]
            if not is_in_house(x1, y1) and x1 <= 299 and 0 < y1 <= 299:
                A[i, x1 * 300 + y1] += cf
            else:
                A[i, i] += cf

    A = scipy.sparse.csr_matrix(A)
    a = a.reshape(N, -1)

    for i in range(max_number_of_steps):
        next_ans = A * ans + a
        if np.max(np.abs(next_ans - ans)/max(1, len(ans))) < eps:
            print('Amount of iterations: ' + str(i))
            break

        ans = next_ans


    fig = plt.figure()
    grid = axes_grid1.AxesGrid(
        fig, 111, nrows_ncols=(1, 1), axes_pad = 0.5, cbar_location = "right",
        cbar_mode="each", cbar_size="15%", cbar_pad="5%",)
    im = grid[0].imshow(-np.rot90(ans.reshape((300, 300))), cmap="hot")
    grid.cbar_axes[0].colorbar(im)
    plt.show()


run(max_number_of_steps=50000, eps=1e-10)
