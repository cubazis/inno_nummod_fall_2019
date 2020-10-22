import sys

class Description:
    def __init__(self, reaction):
        input_data, output_data = reaction.split(">")
        self.rate, output = float(output_data.split()[0]), output_data.split()[1:]
        self.create_outputs(output)
        self.create_inputs(input_data.split('+'))
        

    def get_rate(self):
        return self.rate


    def get_inputs(self):
        return self.inputs


    def get_outputs(self):
        return self.outputs

    
    def create_inputs(self, inputs):
        self.inputs = []
        for i in range(len(inputs)):
            s0, s1 = inputs[i].split()
            for _ in range(int(s0)):
                self.inputs.append(int(s1))
        

    def create_outputs(self, output):
        self.outputs = []
        i = 0
        while output[i] != "#":
            if output[i] == '+':
                i += 1
            self.outputs.append((int(output[i]), int(output[i + 1])))
            i += 2


def f(x, descriptions):
    res = [0] * len(x)
    for r in descriptions:
        a = r.get_rate()
        for i in r.get_inputs():
            a *= x[i]
        for i in r.get_inputs():
            res[i] -= a
        for first_output, second_output in r.get_outputs():
            res[second_output] += first_output * a
    return res


#input_file = open("input.txt")
input_file = sys.stdin
params = input_file.readlines()
time = float(params[0])
reactants_number = int(params[1])

concentrations = []
for i in range(reactants_number):
    val = float(params[2 + i])
    concentrations.append(val)

reactions_number = int(params[2 + reactants_number])

descriptions = []
for i in range(reactions_number):
    val = params[3 + reactants_number + i]
    descriptions.append(Description(val))


# apply algorithm
n = 1500
h = time / n
reactants_number_range = range(reactants_number)
for i in range(n):
    # runge kutta
    k1 = f(concentrations, descriptions)
    k2 = f([concentrations[j] + 0.5 * h * k1[j] for j in reactants_number_range], descriptions)
    k3 = f([concentrations[j] + 0.5 * h * k2[j] for j in reactants_number_range], descriptions)
    k4 = f([concentrations[j] + h * k3[j] for j in reactants_number_range], descriptions)
    concentrations = [concentrations[j] + h / 6 * (k1[j] + 2 * k2[j] + 2 * k3[j] + k4[j]) for j in reactants_number_range]


# print result output
result = []
for i in concentrations:
    result.append(str(i))
print(" ".join(result))
