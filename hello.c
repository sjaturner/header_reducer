#include <stdio.h>
#include "header.h"

volatile int split_marker;

int main(int argc, char *argv)
{
    keep_function_prototype(1);
    int a[KEEP_ENUM] = { };
    ++keep_extern_int;
    keep_inline_function(3);
    struct keep_struct keep_struct = { };
    keep_typedef_struct keep_typedef_struct = { };
    printf("hello\n");
    keep_extern_int = KEEP_MACRO(1);
    keep_extern_int = KEEP_MACRO_CONST;
    union keep_union keep_union = { };
    keep_typedef_union keep_typedef_union = { };
}
