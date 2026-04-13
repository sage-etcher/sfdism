#!/usr/bin/env bash
infile=$1

printf '#!/usr/bin/env sh\n'
printf 'cat "$1" \\\n'
while IFS= read -r line; do
    input="$line"
    output=$(( -(0x10000 - 0x0$line) ))
    printf "\t|sed 's/0%sh[^ ]* /& ;%s/' \\\\\n" "$input" "$output"

done <<<`grep '0f...h' "$infile" |sed 's/^.*0\(....\)h.*/\1/' `

# end of file
