import z3
import re
import numpy as np

total_sum = 0

with open('input.txt', 'r') as file:
    # Columns are buttons
    for line in file:
        matches: list[str] = re.findall(r"[\(\{]((?:[0-9]+,)*[0-9]+)[\)\}]", line)
        wanted_result = list(map(int, matches[-1].split(',')))
        buttons = [np.fromstring(m, sep=',', dtype=int) for m in matches[:-1]]
        # buttons = list(map(
        #     lambda m: list(map(int, m.split(','))),
        #     matches[:-1],
        # ))

        buttons = np.array([np.bincount(b, minlength=len(wanted_result)) for b in buttons])

        print(wanted_result)
        print(buttons)
        A = buttons.transpose()
        b = wanted_result

        x = z3.IntVector("x", len(A[0]))

        opt = z3.Optimize()

        for i in range(len(A)):
            opt.add(sum(A[i][j] * x[j] for j in range(len(A[0]))) == b[i])

        for i in range(len(A[0])):
            opt.add(x[i] >= 0)

        x_sum = sum(x[i] for i in range(len(A[0])))

        opt.minimize(x_sum)

        print(A)
        print(b)

        if opt.check() == z3.sat:
            model = opt.model()
            print(model)
            x_values = [model[xi].as_long() for xi in x]
            for xi in x_values:
                total_sum += xi

    print(f"Total presses: {total_sum}")
