class Reaction:
    def __init__(self, description):
        input, output = description.split('>')
        input_reactants = input.split('+')
        rate = str(output.split()[0])
        temp = output[len(rate) + 1:].split('+')
        if len(temp) > 1:
            temp[-1] = temp[-1][:-2]
            output_reactants = temp
        else:
            output_reactants = [temp[0][:-2]]
        self.input, self.output, self.rate = [], [], float(rate)
        for reactant in input_reactants:
            counts, id = reactant.split()
            self.input.extend([int(id) for _ in range(int(counts))])
        for reactant in output_reactants:
            out, id = reactant.split()
            self.output.append((int(out), int(id)))


def f(x):
    delta = [0 for _ in range(len(x))]
    for reaction in reactions:
        k = reaction.rate
        for i in reaction.input:
            k *= x[i]
        for i in reaction.input:
            delta[i] -= k
        for out, id in reaction.output:
            delta[id] += out * k
    return delta


time = float(input())
n_reactants = int(input())
concentrations = [float(input()) for _ in range(n_reactants)]
n_reactions = int(input())
reactions= [Reaction(input()) for _ in range(n_reactions)]

n = 8000
h = time / n
for i in range(n):
    k1 = f(concentrations)
    k2 = f([(concentrations[j] + 0.5 * h * k1[j]) for j in range(n_reactants)])
    k3 = f([concentrations[j] + 0.5 * h * k2[j] for j in range(n_reactants)])
    k4 = f([concentrations[j] + h * k3[j] for j in range(n_reactants)])
    concentrations = [concentrations[j] + h / 6 * (k1[j] + 2 * k2[j] + 2 * k3[j] + k4[j]) for j in
                      range(n_reactants)]

result = ''
for i in concentrations:
    result += str(i) + ' '
print(result[:-1])
