"""Script solving this day's challenge, read from input.txt"""
import re


def do_part_one():
    sum_tot = 0
    with open("input.txt") as file:
        for line in file:
            for m in re.finditer("mul\((\d+),(\d+)\)", line.strip()):
                num1, num2 = m.group(1, 2)
                num1, num2 = int(num1), int(num2)
                sum_tot += num1 * num2
    return sum_tot

#
# There are 2 cases:
#   1. There exists a `mul(,)` before any `do()` or `don't()`
#   2. There does not exist such a `mul(,)`
def do_part_two():
    sum_tot = 0
    with open("input.txt") as file:
        for line in file:
            for m in re.finditer("mul\((\d+),(\d+)\)", line.strip()):
                num1, num2 = m.group(1, 2)
                num1, num2 = int(num1), int(num2)
                sum_tot += num1 * num2
    return sum_tot

if __name__ == "__main__":
    sum_tot = do_part_one()
    print(f"Calculated a sum of {sum_tot}")
