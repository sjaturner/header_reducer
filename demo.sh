#!/bin/bash
rm -rf tree-sitter-c
(git clone https://github.com/tree-sitter/tree-sitter-c && cd tree-sitter-c && git checkout 2a265d6)
gcc -Wall -g -o parse parse.c tree-sitter-c/src/parser.c -I tree-sitter-c/src -ltree-sitter

gcc -E -fdirectives-only hello.c > hello_directive_only.c
wc -l hello_directive_only.c

cat hello_directive_only.c | sed -n '1,/split_marker/p' | sed '$d' > head.c
cp head.c head.reference.c
cat hello_directive_only.c | sed -n '/split_marker/,$p' > body.c

set -e

cat head.c body.c > test.c
if gcc -c test.c 2> err ; then
    echo compiles for a start
else
    echo does not even compile
    exit
fi

total=$(./parse head.c | wc -l)
iteration=0;
start_time=$(date +%s)
./parse head.c | cut -f2- | sort -n -r | while read a b ; do 
    cp head.c head.save.c
    cat head.save.c | python3 splat_spaces.py $a $b > head.c
    cat head.c body.c > test.c

    if gcc -c test.c 2> err ; then
        echo $(($(date +%s) - start_time)) $total $iteration $a $b removed
    else                                          
        echo $(($(date +%s) - start_time)) $total $iteration $a $b kept
        cp head.save.c head.c 
    fi
    iteration=$((iteration + 1))
done
