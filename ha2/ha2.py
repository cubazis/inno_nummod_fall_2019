import numpy as np

def cubic_spline(x, Y):
    """
    Calculate the coefficients (a_{k,i}) for the cubic spline.
    
    :param x: numpy array of grid values
    :param Y: numpy matrix of values of different functions in given points
    :return: (Y.shape[0], 4, x.shape[0] - 1) numpy matrix with the calculated coefficients
    """
    dlt_x = x[1:] - x[:-1]
    dlt_Y = Y[:, 1:] - Y[:, :-1]
    
    alphas = dlt_x[:-1].copy()
    betas  = 2 * (dlt_x[:-1] + dlt_x[1:])
    gammas = dlt_x[1:].copy()
    deltas = 6 * (dlt_Y[:, 1:] / dlt_x[1:] - dlt_Y[:, :-1] / dlt_x[:-1])
    
    N = deltas.shape[1]
    
    # Direct pass
    P = np.zeros((Y.shape[0], N), dtype=np.float64)
    Q = np.zeros((Y.shape[0], N), dtype=np.float64)
    P[:, 0] = - gammas[0] / betas[0]
    Q[:, 0] = deltas[:, 0] / betas[0]
    for i in range(1, N):
        common_part = betas[i] + alphas[i] * P[:, i - 1]
        Q[:, i] = ((deltas[:, i] - alphas[i] * Q[:, i - 1]) / common_part)
        P[:, i] = - gammas[i] / common_part
    
    # Reverse pass
    m = np.zeros((Y.shape[0], x.shape[0]))
    m[:, -2] = Q[:, -1]
    for i in range(N - 2, -1, -1):
        m[:, i + 1] = P[:, i] * m[:, i + 2] + Q[:, i]
    
    # Calculate final coefficients
    a = np.zeros((Y.shape[0], 4, x.shape[0] - 1), dtype=np.float64)
    
    common_part = (m[:, 1:] - m[:, :-1]) / 6
    a[:, 0, :] = Y[:, :-1]
    a[:, 1, :] = dlt_Y / dlt_x - dlt_x * m[:, :-1] / 2 - dlt_x * common_part    
    a[:, 2, :] = m[:, :-1] / 2
    a[:, 3, :] = common_part / dlt_x
        
    return a

def interpolate(P, x, A):
    """
    Inrerpolate the cubic spline in given point.
    
    :param P: numpy array of x positions to interpolate
    :param x: grid values
    :param A: numpy array of parameters for different cubic splines
    :return: Interpolation result
    """ 
    # check if all the points are inside the covered region.
    # if any(P > x[-1]) or any(P < x[0]):
        # raise ValueError('point outside the defined region')
    # find the needed spline
    p_index = 0
    res = np.zeros((P.shape[0], A.shape[0]), dtype=np.float64)
    for i, pi in enumerate(P):
        while x[p_index + 1] < pi: p_index += 1
        dlt_x = pi - x[p_index]
        res[i, :] = ( A[:, 0, p_index]
                    + A[:, 1, p_index] * dlt_x
                    + A[:, 2, p_index] * dlt_x ** 2
                    + A[:, 3, p_index] * dlt_x ** 3)
    return res
            
def str_to_floats(s, sep=None):
    """
    Parse string of floats to numpy array of floats.
    
    :param str: input string
    :param sep: separator between float values in input string
    :return: numpy array of floats
    """
    splitted = s.strip().split(sep)
    res = np.zeros(len(splitted), dtype=np.float64)
    for i, si in enumerate(splitted):
        res[i] = si
    return res
#     return np.fromstring(str, dtype=float, sep=sep)
#     return np.array(list(float(x) for x in str.split(sep)))


if __name__ == "__main__":
    # -------------------- For standart IO --------------------
    N = int(input())
    x = str_to_floats(input(), sep=' ')
    
    M = int(input())
    Y = np.zeros(shape=(M, N))
    for i in range(M):
        Y[i] = str_to_floats(input(), sep=' ')
    
    K = int(input())
    points = str_to_floats(input(), sep=' ')
    
    # # Check the amount of arguments
    # if x.shape[0] != N or Y.shape[1] != N or points.shape[0] != K:
        # raise ValueError('Wrong amount of arguments')
    
    # I'm doing this with a step, just because of the memory limitation
    step = 250
    for i in range(0, Y.shape[0], step):
        Ai = cubic_spline(x, Y[i:i+step])
        print('\n'.join(map(lambda x: ' '.join(map(str, x)) # lambda x: '{:.16}'.format(x)
                           , interpolate(points, x, Ai).T
                           )
                       ))
    pass
    
    # -------------------- For 'input.txt'/'output.txt' I/O --------------------
    # with open('input.txt') as file:
        # N = int(file.readline())
        # x = str_to_floats(file.readline(), sep=' ')
        
        # M = int(file.readline())
        # Y = np.zeros(shape=(M, N))
        # for i in range(M):
            # Y[i] = str_to_floats(file.readline(), sep=' ')
        
        # K = int(file.readline())
        # points = str_to_floats(file.readline(), sep=' ')
    
    # # # Check the amount of arguments
    # # if x.shape[0] != N or Y.shape[1] != N or points.shape[0] != K:
        # # raise ValueError('Wrong amount of arguments')
    
    # with open('output.txt', 'w') as file:
        # # I'm doing this with a step, just because of the memory limitation
        # step = 250
        # for i in range(0, Y.shape[0], step):
            # Ai = cubic_spline(x, Y[i:i+step])
            # file.write('\n'.join(map(lambda x: ' '.join(map(str, x)) # lambda x: '{:.16}'.format(x)
                                     # , interpolate(points, x, Ai).T
                                     # )
                                 # ))
            # if i < Y.shape[0]: file.write('\n')
    # pass
