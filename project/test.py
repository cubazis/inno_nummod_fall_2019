import numpy as np
import matplotlib.pyplot as plt
from scipy.sparse import csr_matrix, lil_matrix
import matplotlib.cm as cm

def is_small_house(x,y):
  smalls = [(69,111),(192,111),
            (138,12),(261,12),
            (69,264),(192,264),
            (138,165), (261,165)]
  for sm in smalls:
    xl = sm[0]
    yb = sm[1]
    if (xl < x and x < xl+18 and yb < y and y < yb+18):
        return True
  return False

def is_T_house(x,y):
  T_houses = [(138,51),(138,204),
            (261,51),(261,204)]
  for h in T_houses:
    xl = h[0]
    yb = h[1]
    if (xl < x and x < xl+18 and yb < y and y < yb+78):
      return True
    xl = xl-12
    yb = yb+60
    if (xl < x and x < xl+13 and yb < y and y < yb+18):
      return True
  return False

def is_L_house(x,y):
  L_houses = [(69,12),(69,165),
              (192,12),(192,165)]
  for h in L_houses:
    xl = h[0]
    yb = h[1]
    if (xl < x and x < xl+18 and yb < y and y < yb+78):
      return True
    xl = xl+17
    if (xl < x and x < xl+12 and yb < y and y < yb+18):
      return True
  return False

def is_house(x,y):
  if is_small_house(x,y):
    return True
  if is_T_house(x,y):
    return True
  if is_L_house(x,y):
    return True
  return False

def is_smelless(x,y):
  if is_house(x,y):
    return True
  if x>299 or y>299 or y == 0:
    return True
  return False


def findSoln(k=0.5, l1=1.0, l2=0.0, steps=2000, M=300, eps=1e-8):
    N = M*M
    h=1/M
    tau = (h * h) / (4 * k)

    x = np.zeros((N, 1))
    for i in range(M):
        x[M * i, 0] = 1

    A = lil_matrix((N, N))
    b = np.zeros(N)

    dx = [1, -1, 0, 0]
    dy = [0, 0, 1, -1]

    u0_cfs = 1 - 4*(tau * k)/(h ** 2)

    cfs = [tau * (k/(h ** 2) - l1/(2 * h)),
              tau * (k/(h ** 2) + l1/(2 * h)),
              tau * (k/(h ** 2) - l2/(2 * h)),
              tau * (k/(h ** 2) + l2/(2 * h))]

    for i in range(0, M):
        for e in range(0, M):
            ix = i*M + e
            A[ix, ix] = u0_cfs
            for j in range(0, 4):
                cf = cfs[j]
                x_coord = i + dx[j]
                y_coord = e + dy[j]
                if x_coord == 0:
                    b[ix] += cf
                elif is_smelless(x_coord, y_coord):
                    A[ix, ix] += cf
                else:
                    A[ix, x_coord*M + y_coord] = cf

    A = csr_matrix(A)
    b = b.reshape(N, -1)

    c_steps = -1
    for i in range(steps):
        x_new = (A * x + b)
        error = np.max(np.abs(x_new - x))
        if error < eps:
            c_steps=i
            break
        x = x_new
    x = x.reshape((M, M))
    return x, c_steps

def print_mat(x_res):
    for i in range(x_res.shape[0]):
        line = ""
        for j in range(x_res.shape[1]):
            line += str(x_res[i][j]) + " "
        print(line[:-1])

def main():
    eps = float(input())
    M=300
    x_res, c_steps = findSoln(k=0.5, steps=90000, eps=eps, M=M)

    print(M)
    print_mat(x_res.T)

    for i in range(x_res.shape[0]):
        for j in range(x_res.shape[1]):
            if is_house(i,j):
                x_res[i][j]=-1

    fig, ax = plt.subplots()
    ax.set_xticks([0, 69, 138, 192, 261, 299])
    ax.set_yticks([0, 12, 111, 165, 264, 299])
    my_cmap = cm.get_cmap("YlOrBr")
    my_cmap.set_under('k')
    im = ax.imshow(x_res.T, cmap=my_cmap, clim=(0,1))
    fig.gca().invert_yaxis()
    fig.colorbar(im, ax=ax, orientation='vertical')
    plt.title("Converged in {}".format(c_steps))
    plt.show()


if __name__ == "__main__":
    main()