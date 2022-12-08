// ----------------------------------------------------------------------------
// Advent of Code 2022 - Day 7
// Dale Whinham
//
// $ gcc day7.c -Wall -Wextra -Wpedantic -Werror -o day7
// $ ./day7
// ----------------------------------------------------------------------------

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_FILE "input/day7.txt"

#define TOTAL_DISK_SPACE 70000000
#define FREE_SPACE_REQUIRED 30000000
#define LARGE_DIR_THRESHOLD 100000

typedef enum
{
    TYPE_FILE,
    TYPE_DIRECTORY,
} dir_type_t;

typedef struct dir_node_t
{
    dir_type_t type;
    char name[64];
    size_t size;
    struct dir_node_t* parent;
    struct dir_node_t* children;
    size_t num_children;
} dir_node_t;

static dir_node_t root =
{
    .type = TYPE_DIRECTORY,
    .name = "/",
    .size = 0,
    .parent = NULL,
    .children = NULL,
    .num_children = 0,
};

static dir_node_t* find_subdir(const dir_node_t* node, const char* name)
{
    for (size_t i = 0; i < node->num_children; ++i)
        if (strcmp(node->children[i].name, name) == 0)
            return &node->children[i];

    return NULL;
}

static void add_node(dir_node_t* parent, dir_type_t type, const char* name, size_t size)
{
    ++parent->num_children;
    parent->children = realloc(parent->children, parent->num_children * sizeof(dir_node_t));

    dir_node_t* node = &parent->children[parent->num_children - 1];
    node->type = type;
    strcpy(node->name, name);
    node->size = size;
    node->parent = parent;
    node->children = NULL;
    node->num_children = 0;

    if (type == TYPE_FILE)
        do parent->size += size; while ((parent = parent->parent));
}

static void print_directory_tree(dir_node_t* node, size_t depth)
{
    if (node->type == TYPE_FILE)
        printf("%*s- %s (file, size=%zu)\n", (int)depth * 4, "", node->name, node->size);
    else
    {
        printf("%*s- %s (dir)\n", (int)depth * 4, "", node->name);
        for (size_t i = 0; i < node->num_children; ++i)
            print_directory_tree(&node->children[i], depth + 1);
    }
}

static size_t find_large_dirs(dir_node_t* node)
{
    if (node->type == TYPE_FILE)
        return 0;

    size_t size = 0;
    if (node->size <= LARGE_DIR_THRESHOLD)
        size += node->size;

    for (size_t i = 0; i < node->num_children; ++i)
        size += find_large_dirs(&node->children[i]);

    return size;
}

static dir_node_t* find_deletion_candidate(dir_node_t* node, size_t target_size)
{
    if (node->type == TYPE_FILE)
        return NULL;

    if (node->size < target_size)
        return NULL;

    dir_node_t* candidate_node = node;
    for (size_t i = 0; i < node->num_children; ++i)
    {
        dir_node_t* current_node = find_deletion_candidate(&node->children[i], target_size);
        if (current_node && current_node->size < candidate_node->size)
            candidate_node = current_node;
    }

    return candidate_node;
}

static void free_directory_tree(dir_node_t* node)
{
    if (node->type == TYPE_FILE)
        return;

    for (size_t i = 0; i < node->num_children; ++i)
        free_directory_tree(&node->children[i]);

    free(node->children);
    node->children = NULL;
    node->num_children = 0;
}

int main()
{
    FILE* f = fopen(INPUT_FILE, "r");

    dir_node_t* current_dir = &root;
    char line_buffer[512];
    char name_buffer[64];
    size_t file_size;

    while (fgets(line_buffer, sizeof(line_buffer), f))
    {
        if (sscanf(line_buffer, "$ cd %s\n", name_buffer) == 1)
        {
            if (name_buffer[0] == '/')
                current_dir = &root;
            else if (name_buffer[0] == '.')
                current_dir = current_dir->parent;
            else
                current_dir = find_subdir(current_dir, name_buffer);
        }
        else if (sscanf(line_buffer, "%zu %s\n", &file_size, name_buffer) == 2)
            add_node(current_dir, TYPE_FILE, name_buffer, file_size);
        else if (sscanf(line_buffer, "dir %s\n", name_buffer) == 1)
            add_node(current_dir, TYPE_DIRECTORY, name_buffer, 0);
    }

    print_directory_tree(&root, 0);

    size_t sum_large_dir_sizes = find_large_dirs(&root);
    size_t free_space = TOTAL_DISK_SPACE - root.size;
    size_t additional_space = FREE_SPACE_REQUIRED - free_space;
    dir_node_t* deletion_candidate = find_deletion_candidate(&root, additional_space);

    printf
    (
        "Free space:                   %zu\n"
        "Sum of large directory sizes: %zu\n"
        "Additional space required:    %zu\n"
        "Deletion candidate size:      %zu\n",
        free_space,
        sum_large_dir_sizes,
        additional_space,
        deletion_candidate->size
    );

    free_directory_tree(&root);
    fclose(f);
    return EXIT_SUCCESS;
}
