"""Script solving this day's challenge, read from input.txt"""
#
#   Note:
#       - Initial implementation of Part 2 was incorrect since I erroneously
#   assumed that the condition of whether or not we sum `mul()` operations 
#   resets each line when, in actuality, the `don't()` instruction carries
#   on between lines.
#


import io
import re
import typing
from os import path

#
#   Returns the sum of all `mul` operations on the passed in line.
#   Appends to `output_me.txt` the list of all `mul(\d,\d)` matches,
#   one per line.
#
def get_mul_sum_on_line(line: str, write_muls = False) -> int:
    write_path = 'output_me.txt'
    write_fstream = io.StringIO()
    if write_muls:
        write_fstream = open(write_path, 'a')
    line_sum = 0
    for m in re.finditer("mul\((\d+),(\d+)\)", line.strip()):
        if write_muls:
            write_fstream.write(m.group(0) + '\n')
        num1, num2 = m.group(1, 2)
        num1, num2 = int(num1), int(num2)
        line_sum += num1 * num2

    write_fstream.close()
    return line_sum

#
#   Iterates over the file, parses each `mul(\d, \d)` operation,
#   and sums their values.
#
def do_part_one(file_name = 'input.txt'):
    sum_tot = 0
    with open(file_name) as file:
        for line in file:
            sum_tot += get_mul_sum_on_line(line)
    return sum_tot

#
#   1. Split the line along "do()" and "don't()" instructions. Iterate
#   over these matches
#       i. If split_match is "do()" then set `mul_active` to true
#       ii. Else if split_match is "don't()" then set `mul_active` to false
#       iii. else, we're matching a line between these two
#           1. If `mul_active` is false, continue
#           2. Add the summed mul operations on this line to a run_sum
#   2. return run_sum
#
def do_part_two(file_name = 'input.txt', write_muls = False) -> int:
    run_sum = 0
    with open(file_name) as file:
        mul_active = True
        for line in file:
            # Split the line along "do()" and "don't()"
            for split_match in re.split("(do\(\)|don't\(\))", line):
                if split_match == "do()":
                    mul_active = True
                elif split_match == "don't()":
                    mul_active = False
                elif mul_active:
                    run_sum += get_mul_sum_on_line(split_match, write_muls)
    return run_sum

if __name__ == "__main__":
    sum_tot_one = do_part_one('input.txt')
    print(f"Calculated a sum of {sum_tot_one} for part one")
    sum_tot_two = do_part_two("input.txt")
    print(f"Calculated a sum of {sum_tot_two} for part two")
