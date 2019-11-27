from functools import reduce

STEPS = 1120


class Reaction:
    def __init__(self, string):
        tokens = string.split(" ")
        input_end_index = tokens.index(">")

        self.rate = float(tokens[input_end_index + 1])

        self.input = []
        for i in range(0, input_end_index, 3):
            self.input.extend(int(tokens[i]) * [int(tokens[i + 1])])

        self.output = [(int(tokens[i]), int(tokens[i + 1])) for i in range(input_end_index + 2, len(tokens), 3)]


class Input:
    def __init__(self):
        self.T = float(input())
        self.N = int(input())
        self.concentrations = [float(input()) for _ in range(self.N)]
        self.M = int(input())
        self.reactions = [Reaction(input()) for _ in range(self.M)]


def step(state, inp: Input):
    diff = [0.0] * len(state)

    for reaction in inp.reactions:
        interaction = reduce((lambda x, y: x * y), [state[i] for i in reaction.input])
        for i in reaction.input:
            diff[i] -= reaction.rate * interaction
        for k, i in reaction.output:
            diff[i] += k * reaction.rate * interaction

    return diff


def main():
    inp = Input()
    step_size = inp.T / STEPS

    state = inp.concentrations
    for step_n in range(STEPS):
        k1 = step(state, inp)
        k2 = step([state[i] + step_size / 2 * k1[i] for i in range(inp.N)], inp)
        k3 = step([state[i] + step_size / 2 * k2[i] for i in range(inp.N)], inp)
        k4 = step([state[i] + step_size * k3[i] for i in range(inp.N)], inp)

        for i in range(inp.N):
            state[i] += step_size / 6 * (k1[i] + 2 * k2[i] + 2 * k3[i] + k4[i])

    print(" ".join([str(i) for i in state]))


if __name__ == "__main__":
    main()
