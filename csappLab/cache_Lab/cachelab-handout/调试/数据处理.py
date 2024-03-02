import re
t = 0

# 16转2
def hex_to_bin(hex_str):
    return bin(int(hex_str, 16))[2:].zfill(len(hex_str)*4)

# 2转10
def bin_to_dec(bin_str):
    return str(int(bin_str, 2))


with open('调试/64x64.txt', 'r') as infile, open('分块与循环展开A_64x64.txt', 'w') as outfileA, open('分块与循环展开B_64x64.txt', 'w') as outfileB:
    for line in infile:
        t += 1
        op = True #B组line
        if not line.startswith('L'):
            op = False # A组line
        line = hex_to_bin(line[4:8:1]) + ' ' + line[10:]
        line =  bin_to_dec(line[4:6:1])+  ' '+ bin_to_dec(line[6:11:1]) +' ' +  line[12:]
        if (t % 8 == 0):
            line = line + '\n'

        if op:
            outfileA.write(line)
        else:
            outfileB.write(line)
