import sys


def meow_to_list(l):
    out = list()
    for i in range(0, len(l), 3):
        out += [int(l[i + 1])] * int(l[i])
    return out


def meow_to_dict(l):
    return {int(l[i + 1]): int(l[i]) for i in range(0, len(l), 3)}


class Reaction:
    def __init__(self, description):
        l = description.split(" ")
        self.inputs = meow_to_list(l[:l.index(">")])
        self.rate = float(l[l.index(">") + 1])
        self.outputs = meow_to_dict(l[l.index(">") + 2:-1])

    def __repr__(self):
        return "{}->{}->{}".format(self.inputs, self.rate, self.outputs)


params_repr = """\
T: {}
State: {}
Reactions: 
{}"""


class Params:
    def __init__(self, source):
        self.time = float(source.readline())
        self.n_reactants = int(source.readline())
        self.concentrations = [float(source.readline()) for l in range(self.n_reactants)]
        self.n_reactions = int(source.readline())
        self.descriptions = [Reaction(source.readline()) for l in range(self.n_reactions)]

    def __repr__(self):
        return params_repr.format(self.time, self.concentrations, "\n".join([str(i) for i in self.descriptions]))


def iteration(state, p):
    diff = [0 for i in range(len(state))]
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


def solve(p):
    n = p.n_reactants
    # 8K for R-K
    # 50K for Euler
    steps = 8000
    step = p.time / steps
    state = p.concentrations

    for i in range(steps):
        # Runge-Kutta
        k1 = iteration(state, p)
        k2 = iteration([a + step / 2 * b for a, b in zip(state, k1)], p)
        k3 = iteration([a + step / 2 * b for a, b in zip(state, k2)], p)
        k4 = iteration([a + step * b for a, b in zip(state, k3)], p)
        state = [s + step / 6 * (ka + 2 * kb + 2 * kc + kd) for s, ka, kb, kc, kd in zip(state, k1, k2, k3, k4)]
        # Euler
        # diff = iteration(state, p)
        # state = [a + b * step for a, b in zip(state, diff)]
    return state


def main():
    # p = Params(open("data/input.txt"))
    p = Params(sys.stdin)
    state = solve(p)
    print(" ".join([str(i) for i in state]))


if __name__ == "__main__":
    main()
