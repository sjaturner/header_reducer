#pragma once
int remove_function_prototype(int a);
int keep_function_prototype(int a);
enum
{
    REMOVE_ENUM = 2,
};
/* Comment. */
enum
{
    KEEP_ENUM = 2,
};
extern int remove_extern_int;
extern int keep_extern_int;
static inline int remove_inline_function(int a)
{
    return 2;
}
static inline int keep_inline_function(int a)
{
    return 2;
}
struct remove_struct
{
    int a;
};
struct keep_struct
{
    int a;
};
typedef struct
{
    int a;
} remove_typedef_struct;
typedef struct
{
    int a;
} keep_typedef_struct;
#define REMOVE_MACRO(A) 1
#define KEEP_MACRO(A) 1
#define REMOVE_MACRO_CONST 1
#define KEEP_MACRO_CONST 1
union remove_union
{
    int a;
    int b;
};
union keep_union
{
    int a;
    int b;
};
typedef union
{
    int a;
    int b;
} remove_typedef_union;
typedef union
{
    int a;
    int b;
} keep_typedef_union;
