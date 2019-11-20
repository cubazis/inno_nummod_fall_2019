import sys


def inputs_to_list(l):
    out = list()
    for i in range(0, len(l), 3):
        out += [int(l[i + 1])] * int(l[i])
    return out


def outputs_to_dict(l):
    return {int(l[i + 1]): int(l[i]) for i in range(0, len(l), 3)}


class Reaction:
    def __init__(self, description):
        process = description.split(" ")
        index = process.index('>')

        self.inputs = inputs_to_list(process[:index])
        self.rate = float(process[index + 1])
        self.outputs = outputs_to_dict(process[index + 2:-1])


class Params:
    def __init__(self, source):
        self.time = float(source.readline())
        self.n_reactants = int(source.readline())
        self.concentrations = [float(source.readline()) for _ in range(self.n_reactants)]
        self.n_reactions = int(source.readline())
        self.descriptions = [Reaction(source.readline()) for _ in range(self.n_reactions)]

def iteration(state, p):
    diff = [0] * len(state)
    for reaction in p.descriptions:
        inputs = reaction.inputs
        rate = reaction.rate
        outputs = reaction.outputs
        if len(inputs) == 1:
            i = inputs[0]
            diff[i] -= rate * state[i]
            for o, c in outputs.items():
                diff[o] += c * rate * state[i]
        if len(inputs) == 2:
            a, b = inputs[0], inputs[1]
            interaction = state[a] * state[b]
            diff[a] -= rate * interaction
            diff[b] -= rate * interaction
            for o, c in outputs.items():
                diff[o] += c * rate * interaction
    return diff


p = Params(sys.stdin)
steps = 7500

step = p.time / steps
state = p.concentrations

for i in range(steps):
  k1 = iteration(state, p)
  k2 = iteration([a + step / 2 * b for a, b in zip(state, k1)], p)
  k3 = iteration([a + step / 2 * b for a, b in zip(state, k2)], p)
  k4 = iteration([a + step * b for a, b in zip(state, k3)], p)
  state = [s + step / 6 * (ka + 2 * kb + 2 * kc + kd) for s, ka, kb, kc, kd in zip(state, k1, k2, k3, k4)]

print(" ".join([str(i) for i in state]))
