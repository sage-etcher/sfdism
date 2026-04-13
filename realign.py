#!/usr/bin/env python3

import sys

if len(sys.argv) < 2:
    print('Usage: realign.py infile')

infile = sys.argv[1]
with open(infile, 'r') as f:
    content = [ line.split('\t\t', 1) for line in f.read().split('\n') ]

    mnemonic_max_len = 0
    for line in content:
        if len(line) <= 1:
            continue

        mnemonic = line[0].strip()
        mnemonic_len = len(mnemonic)
        if mnemonic_max_len < mnemonic_len:
            mnemonic_max_len = mnemonic_len

    for line in content:
        if len(line) == 0:
            print('')

        elif len(line) == 1:
            print(line[0])

        else:
            print(f'\t{line[0].strip():<{mnemonic_max_len}}', end = '')

            if len(line) > 1:
                print(f'\t\t{line[1]}', end = '')

            print('')   # end of line

# vim: ts=4 sts=4 sw=4 et
# end of file
