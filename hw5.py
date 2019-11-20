def f(x, descriptions):
    d = [0] * len(x)
    for r in descriptions:
        a = r["rate"]
        for i in r['input']:
            a *= x[i]
        for i in r['input']:
            d[i] -= a
        for o1, o2 in r["output"]:
            d[o2] += o1 * a
    return d

def f_k1(x, descriptions):
    return f(x, descriptions)

def f_k2(x, h, k1, i):
    return x[i] + 0.5 * h * k1[i]

def f_k3(x, h, k2, i):
    return x[i] + 0.5 * h * k2[i]

def f_k4(x, h, k3, i):
    return x[i] + h * k3[i]

def read_description():
    line = input()
    split1 = line.split(">")
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


def runge_kutta():
    n = 1116
    h = T / n
    x = C
    for i in range(n):
        k1 = f_k1(x, descriptions)
        k2 = []
        for i in range(N):
            k2.append(f_k2(x, h, k1, i))
        k2 = f(k2, descriptions)
        k3 = []
        for i in range(N):
            k3.append(f_k3(x, h, k2, i))
        k3 = f(k3, descriptions)
        k4 = []
        for i in range(N):
            k4.append(f_k4(x, h, k3, i))
        k4 = f(k4, descriptions)
        for i in range(N):
            x[i] = x[i] + h / 6 * (k1[i] + 2 * k2[i] + 2 * k3[i] + k4[i])
    return x


T = float(input())
N = int(input())
C = []
for i in range(N):
    C.append(float(input()))
M = int(input())
descriptions = []
for i in range(M):
    descriptions.append(read_description())
x = runge_kutta()
res = ""
for item in x:
    if len(res) > 0:
        res += " "
    res += str(item)
print(res)
