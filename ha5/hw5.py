import sys


def read_data():
    # file = open("input.txt", "r")
    file = sys.stdin

    def get_generator():
        while True:
            yield file.readline()

    generator = get_generator()

    def next_line():
        return next(generator)

    t = float(next_line())
    n = int(next_line())
    concentrations = []
    formulas = {}

    for i in range(n):
        concentrations.append(float(next_line()))
        formulas[i] = []

    m = int(next_line())

    for i in range(m):
        left, right = next_line().split(' > ')
        left = left.split()
        right = right.split()[:-1]
        right = [x for x in right if x != '+']

        # process left part
        k = float(right[0])
        element = int(left[1])
        if len(left) == 2:
            coeff = int(left[0])
            if coeff == 1:
                conc = [element]
                formulas[element].append(tuple([-coeff * k, conc]))
            else:
                conc = [element, element]
                formulas[element].append(tuple([-coeff * k, conc]))

        else:
            el1 = int(left[1])
            el2 = int(left[4])
            conc = [el1, el2]
            formulas[el1].append(tuple([-k, conc]))
            formulas[el2].append(tuple([-k, conc]))

        # process right part
        for j in range(1, len(right) - 1, 2):
            n_j = float(right[j])
            b_j = int(right[j + 1])
            formulas[b_j].append(tuple([k * n_j, conc]))

    return t, concentrations, formulas


def calculate_derivatives(equations, concentr):
    derivatives = [0] * N

    for i in range(N):
        res = 0
        for coeff, conc in equations[i]:
            part = coeff
            for el in conc:
                part *= concentr[el]
            res += part

        derivatives[i] = res

    return derivatives


def vec_add(*args):
    res = [0] * len(args[0])
    for vec in args:
        for i, v in enumerate(vec):
            res[i] += v

    return res


def vec_mul(value, vec):
    return [value * v for v in vec]


t, concentrations, formulas = read_data()
N = len(concentrations)

steps_count = 2000
h = t / steps_count


def derivative_step(concentrations):
    return vec_mul(h, calculate_derivatives(formulas, concentrations))


for i in range(steps_count):
    k1 = derivative_step(concentrations)
    k2 = derivative_step(vec_add(concentrations, vec_mul(1 / 2, k1)))
    k3 = derivative_step(vec_add(concentrations, vec_mul(1 / 2, k2)))
    k4 = derivative_step(vec_add(concentrations, k3))

    approx_step = vec_mul(1 / 6, vec_add(k1, k2, k2, k3, k3, k4))
    concentrations = vec_add(concentrations, approx_step)

print(' '.join(map(str, concentrations)))
