#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tree_sitter/api.h>

// Forward declaration from tree-sitter-c grammar
extern const TSLanguage *tree_sitter_c();

void print_node(TSNode node, const char *source_code, const char *label) {
    uint32_t start = ts_node_start_byte(node);
    uint32_t end = ts_node_end_byte(node);
    printf("%s\t%d\t%d\n", label, start, end);
}

#include <string.h>  // for strncmp, strlen, memmem


int has_descendant_of_type(TSNode node, const char *target_type) {
    if (strcmp(ts_node_type(node), target_type) == 0) return 1;

    uint32_t count = ts_node_named_child_count(node);
    for (uint32_t i = 0; i < count; i++) {
        TSNode child = ts_node_named_child(node, i);
        if (has_descendant_of_type(child, target_type)) return 1;
    }
    return 0;
}

void walk_top_level(TSNode root, const char *source_code) {
    uint32_t n_children = ts_node_named_child_count(root);

    for (uint32_t i = 0; i < n_children; i++) {
        TSNode node = ts_node_named_child(root, i); 
        const char *type = ts_node_type(node);

        if (strcmp(type, "type_definition") == 0) {
            print_node(node, source_code, "typedef");
        } else if (strcmp(type, "struct_specifier") == 0) {
            print_node(node, source_code, "struct");
        } else if (strcmp(type, "union_specifier") == 0) {
            print_node(node, source_code, "union");
        } else if (strcmp(type, "enum_specifier") == 0) {
            print_node(node, source_code, "enum");
        } else if (strcmp(type, "preproc_def") == 0 || strcmp(type, "preproc_function_def") == 0) {
            print_node(node, source_code, "macro");
        } else if (strcmp(type, "declaration") == 0) {
            if (has_descendant_of_type(node, "function_declarator")) {
                print_node(node, source_code, "function_prototype");
            } else {
                // still check for extern storage_class_specifier
                uint32_t child_count = ts_node_named_child_count(node);
                int printed = 0;
                for (uint32_t j = 0; j < child_count && !printed; j++) {
                    TSNode child = ts_node_named_child(node, j); 
                    if (!ts_node_is_null(child)) {
                        const char *child_type = ts_node_type(child);
                        if (strcmp(child_type, "storage_class_specifier") == 0) {
                            uint32_t start = ts_node_start_byte(child);
                            if (start + 6 <= strlen(source_code) &&
                                strncmp(&source_code[start], "extern", 6) == 0) {
                                print_node(node, source_code, "extern");
                                printed = 1;
                            }   
                        }   
                    }   
                }
                if (!printed) {
                    print_node(node, source_code, "declaration");
                }
            }
        } else if (strcmp(type, "function_definition") == 0) {
            // Check for 'inline' keyword
            uint32_t start = ts_node_start_byte(node);
            uint32_t end = ts_node_end_byte(node);
            size_t len = end - start;
            const char *snippet = source_code + start;

            if (memmem(snippet, len, "inline", 6)) {
                print_node(node, source_code, "inline_function");
            } else {
                print_node(node, source_code, "function_definition");
            }
        }
    }
}
char *read_file(const char *path, size_t *out_len) {
    FILE *f = fopen(path, "rb");
    if (!f) return NULL;
    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    rewind(f);

    if (len <= 0) {
        fclose(f);
        return NULL;
    }

    char *buf = malloc(len + 1);
    if (!buf) {
        fclose(f);
        return NULL;
    }

    fread(buf, 1, len, f);
    buf[len] = '\0';
    fclose(f);
    *out_len = len;
    return buf;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s file.c\n", argv[0]);
        return 1;
    }

    size_t len = 0;
    char *source_code = read_file(argv[1], &len);
    if (!source_code || len == 0) {
        fprintf(stderr, "Failed to read source file.\n");
        return 1;
    }

    TSParser *parser = ts_parser_new();
    if (!ts_parser_set_language(parser, tree_sitter_c())) {
        fprintf(stderr, "Failed to set C language for parser.\n");
        ts_parser_delete(parser);
        free(source_code);
        return 1;
    }

    TSTree *tree = ts_parser_parse_string(parser, NULL, source_code, len);
    if (!tree) {
        fprintf(stderr, "Parse failed.\n");
        ts_parser_delete(parser);
        free(source_code);
        return 1;
    }

    TSNode root = ts_tree_root_node(tree);
    walk_top_level(root, source_code);

    ts_tree_delete(tree);
    ts_parser_delete(parser);
    free(source_code);
    return 0;
}
