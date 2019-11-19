def parse_reaction(reaction):
    split1 = reaction.split(">")
    split1_0 = split1[0].split('+')
    split1_1 = split1[1].split()[1:]
    dict = {}
    dict["input"] = []

    for i in range(len(split1_0)):
        s = split1_0[i].split()
        for j in range(int(s[0])):
            dict["input"].append(int(s[1]))

    dict["rate"] = float(split1[1].split()[0])
    dict["output"] = []
    i = 0
    while split1_1[i] != "#":
        if split1_1[i] == '+':
            i += 1

        dict["output"].append((int(split1_1[i]), int(split1_1[i + 1])))
        i += 2
    return dict


def f(x):
    d = [0] * len(x)
    for r in D:
        a = r["rate"]
        for i in r['input']:
            a *= x[i]
        for i in r['input']:
            d[i] -= a
        for o1, o2 in r["output"]:
            d[o2] += o1 * a
    return d


# Read input
T = float(input())  # time
N = int(input())  # number of reactants
C = []  # initial concentration of reactants
for i in range(N):
    C.append(float(input()))
M = int(input())  # number of reactions
D = []  # descriptions of reactions
for i in range(M):
    D.append(parse_reaction(input()))

# Fourth Order Runge-Kutta Method
n = 1116
h = T / n
x = C
for i in range(n):
    k1 = f(x)

    k2 = []
    for i in range(N):
        k2.append(x[i] + 0.5 * h * k1[i])
    k2 = f(k2)

    k3 = []
    for i in range(N):
        k3.append(x[i] + 0.5 * h * k2[i])
    k3 = f(k3)

    k4 = []
    for i in range(N):
        k4.append(x[i] + h * k3[i])
    k4 = f(k4)

    for i in range(N):
        x[i] = x[i] + h / 6 * (k1[i] + 2 * k2[i] + 2 * k3[i] + k4[i])

res = ""
for i in x:
    if len(res) > 0:
        res += " "
    res += str(i)
print(res)
