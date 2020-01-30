def calculate_main():
    step_num = 2000
    concentrations = Cs_arr.copy()

    def calculate_coef(concentrations):
        d = []
        for a in range(len(concentrations)):
            d.append(0)

        for r_desc in desc_arr:
            input = r_desc['input']
            rate = r_desc['rate']
            output = r_desc['output']
            if len(input) == 1:
                a = input[0]
                d[a] -= rate * concentrations[a]
                for i1, i2 in output.items():
                    d[i1] += i2 * rate * concentrations[a]
            if len(input) == 2:
                a, b = input[0], input[1]
                mult = concentrations[a] * concentrations[b]
                d[a] -= rate * mult
                d[b] -= rate * mult
                for i1, i2 in output.items():
                    d[i1] += i2 * rate * mult
        return d

    for i in range(step_num):
        k1 = calculate_coef(concentrations)
        arr = []
        for j in range(len(concentrations)):
            arr.append(concentrations[j] + T / step_num * 0.5 * k1[j])
        k2 = calculate_coef(arr)
        arr = []
        for j in range(len(concentrations)):
            arr.append(concentrations[j] + T / step_num * 0.5 * k2[j])
        k3 = calculate_coef(arr)
        arr = []
        for j in range(len(concentrations)):
            arr.append(concentrations[j] + T / step_num * k3[j])
        k4 = calculate_coef(arr)
        arr = []
        for j in range(len(concentrations)):
            arr.append(concentrations[j] + T / step_num / 6 * (k1[j] + 2 * k2[j] + 2 * k3[j] + k4[j]))
        concentrations = arr
    return concentrations

# Reading input data
T = 0
N = 0
M = 0
Cs_arr = []
desc_arr = []
# with open("data/input.txt", "r") as f:
T = float(input())
N = int(input())
for i in range(N):
    Cs_arr.append(float(input()))
M = int(input())
for i in range(M):
    desc = {}
    line = input()
    input_arr = []
    split_io = line.split('>')
    split_input = split_io[0].split('+')
    for item in split_input:
        r_item = item.split()
        for j in range(int(r_item[0])):
            input_arr.append(int(r_item[1]))
    desc['input'] = input_arr
    split_ro = split_io[1].split()
    desc['rate'] = float(split_ro[0])
    split_ro = split_ro[1:-1]
    split_ro = list(filter(lambda x: x != '+', split_ro))
    output_dict = {}
    for j in range(0, len(split_ro), 2):
        output_dict[int(split_ro[j + 1])] = int(split_ro[j])
    desc['output'] = output_dict
    desc_arr.append(desc)

concentrations = calculate_main()

output = ""
for i in range(len(concentrations)):
    output += str(concentrations[i])
    if i is not len(concentrations) - 1:
        output += " "
print(output)
