# F
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


def parse_reaction_description(reaction):
    input, output = reaction.split(">")
    input_1 = input.split('+')
    rate, output = float(output.split()[0]), output.split()[1:]
    dict = {}
    dict["input"] = []

    for i in range(len(input_1)):
        s = input_1[i].split()
        for j in range(int(s[0])):
            dict["input"].append(int(s[1]))

    dict["rate"] = rate
    dict["output"] = []
    i = 0
    while output[i] != "#":
        if output[i] == '+':
            i += 1

        dict["output"].append((int(output[i]), int(output[i + 1])))
        i += 2
    return dict


### READ INPUT
# Read time
T = float(input())
# Read number of reactants
N = int(input())
# Read initial concentration of reactants
C = []
for i in range(N):
    C.append(float(input()))
# Read number of reactions
M = int(input())
# Read descriptions of reactions
D = []
for i in range(M):
    D.append(parse_reaction_description(input()))

# Fourth Order Runge-Kutta Method
n = 1116
h = T / n
conc = C
for i in range(n):
    k1 = f(conc)

    k2 = []
    for j in range(N):
        k2.append(conc[j] + 0.5 * h * k1[j])
    k2 = f(k2)

    k3 = []
    for j in range(N):
        k3.append(conc[j] + 0.5 * h * k2[j])
    k3 = f(k3)

    k4 = []
    for j in range(N):
        k4.append(conc[j] + h * k3[j])
    k4 = f(k4)

    for j in range(N):
        conc[j] = conc[j] + h / 6 * (k1[j] + 2 * k2[j] + 2 * k3[j] + k4[j])

res = ""
for i in conc:
    if len(res) > 0:
        res += " "
    res += str(i)
print(res)
