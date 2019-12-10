import sys
def parse_equation(inp):

    vals = inp.split(" ")
    arrow_seen = vals.index(">")
    main_reactants = vals[:arrow_seen]
    main_reactants_inds = []
    for i in range(0, len(main_reactants), 3):
        main_reactants_inds.append(int(main_reactants[i + 1]))
        if int(main_reactants[i]) == 2:
            main_reactants_inds.append(int(main_reactants[i+1]))

    k = float(vals[arrow_seen+1])
    b_reactants = vals[arrow_seen+2:-1]

    b_reactants_data = []
    for i in range(0, len(b_reactants), 3):
        b_reactants_data.append((int(b_reactants[i+1]), int(b_reactants[i])))

    return [main_reactants_inds, k, b_reactants_data]




def conc_update(r_conc, eqs):
    r_conc_new = [0]*len(r_conc)
    for elem in eqs:
        primary = elem[0]
        secondary = elem[2]
        k = elem[1]
        if len(primary) == 1:
            i = primary[0]
            r_conc_new[i] -= k * r_conc[i]
            for elemb in secondary:
                o, c = elemb
                r_conc_new[o] += c*k*r_conc[i]
        if len(primary) == 2:
            a, c = primary[0], primary[1]
            mult_ac = r_conc[a] * r_conc[c]
            r_conc_new[a] -= k * mult_ac
            r_conc_new[c] -= k * mult_ac
            for elemb in secondary:
                o, c = elemb
                r_conc_new[o] += c*k*mult_ac
    return r_conc_new




def main():
    # t, num_reactants, num_equations, reactants_conc, equations = parse_input()

    src = sys.stdin
    t = float(src.readline())
    num_reactants = int(src.readline())
    reactants_conc = []
    for _ in range(num_reactants):
        reactants_conc.append(float(src.readline()))

    num_equations = int(src.readline())
    equations = []

    for _ in range(num_equations):
        eq = parse_equation(src.readline())
        equations.append(eq)

    iters = 4000
    h = t/iters

    for i in range(iters):
        k1 = conc_update(reactants_conc, equations)
        k2 = []

        for i in range(len(reactants_conc)):
            k2.append(reactants_conc[i]+h/2*k1[i])
        k2 = conc_update(k2, equations)

        k3 = []

        for i in range(len(reactants_conc)):
            k3.append(reactants_conc[i] + h/2*k2[i])
        k3 = conc_update(k3, equations)

        k4 = []
        for i in range(len(reactants_conc)):
            k4.append(reactants_conc[i] + h*k3[i])
        k4 = conc_update(k4, equations)

        reactants_conc = [s + h/6 * (k11+2*k22+2*k33+k44) for s, k11, k22, k33, k44 in zip(reactants_conc, k1, k2, k3, k4)]

    print(" ".join([str(i) for i in reactants_conc]))

if __name__ == "__main__":
    main()