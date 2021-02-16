import random
for j in range(5):
    with open(f'infile{j + 1}.txt', 'w') as f:
        for i in range(5000):
            f.write('%d ' % random.randint(0, 2147483647))