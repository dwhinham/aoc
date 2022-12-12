// ----------------------------------------------------------------------------
// Advent of Code 2022 - Day 11
// Dale Whinham
//
// $ gcc day11.c -Wall -Wextra -Wpedantic -Werror -o day11
// $ ./day11
// ----------------------------------------------------------------------------

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define PART_TWO

#ifdef PART_TWO
#define NUM_ROUNDS 10000
#else
#define NUM_ROUNDS 20
#endif

#define INPUT_FILE "input/day11.txt"

typedef struct
{
    char operator;
    int32_t operand;
} operation_t;

typedef struct
{
    int32_t divisible_by;
    size_t monkey_true;
    size_t monkey_false;
} test_t;

typedef struct
{
    int64_t* items;
    size_t num_items;
    operation_t operation;
    test_t test;
    uint64_t inspected;
} monkey_t;

static monkey_t* monkeys = NULL;
static size_t num_monkeys = 0;
#ifdef PART_TWO
static int32_t common_modulo = 0;
#endif

static monkey_t* new_monkey()
{
    monkeys = realloc(monkeys, (num_monkeys + 1) * sizeof(*monkeys));
    monkey_t* monkey = &monkeys[num_monkeys++];
    *monkey = (monkey_t)
    {
        .items = NULL,
        .num_items = 0,
        .operation = { .operator = '\0', .operand = 0, },
        .test = { .divisible_by = 0, .monkey_true = 0, .monkey_false = 0, },
        .inspected = 0,
    };
    return monkey;
}

static void add_item(monkey_t* monkey, int64_t item)
{
    monkey->items = realloc(monkey->items, (monkey->num_items + 1) * sizeof(*monkey->items));
    monkey->items[monkey->num_items++] = item;
}

#include <assert.h>

static void remove_item(monkey_t* monkey, size_t index)
{
    assert(monkey->num_items > 0);
    if (--monkey->num_items > index)
        memmove(&monkey->items[index], &monkey->items[index + 1], (monkey->num_items - index) * sizeof(*monkey->items));

    monkey->items = realloc(monkey->items, monkey->num_items * sizeof(*monkey->items));
}

// static void print_monkey(monkey_t* monkey)
// {
//     printf("Starting items: ");
//     for (size_t i = 0; i < monkey->num_items; ++i)
//         printf("%ld%s", monkey->items[i], i < monkey->num_items - 1 ? ", " : "\n");
    
//     printf
//     (
//         "Operation: new = old %c %d\n"
//         "Test: divisible by %d\n"
//         "  If true: throw to monkey %ld\n"
//         "  If false: throw to monkey %ld\n",
//         monkey->operation.operator,
//         monkey->operation.operand,
//         monkey->test.divisible_by,
//         monkey->test.monkey_true,
//         monkey->test.monkey_false
//     );
// }

static void print_monkey_items(size_t index)
{
    printf("Monkey %zu items: ", index);
    monkey_t* monkey = &monkeys[index];
    for (size_t i = 0; i < monkey->num_items; ++i)
        printf("%ld%s", monkey->items[i], i < monkey->num_items - 1 ? ", " : "");
    putchar('\n');
}

#include <assert.h>

static int64_t operator(char operator, int64_t operand_a, int64_t operand_b)
{
    switch (operator)
    {
        case '+': return operand_a + operand_b;
        case '-': return operand_a - operand_b;
        case '*': return operand_a * operand_b;
        case '/': return operand_a / operand_b;
        default:  return operand_a * operand_a;
    }
}

static void do_round()
{
    for (size_t i = 0; i < num_monkeys; ++i)
    {
        monkey_t* monkey = &monkeys[i];
        while (monkey->num_items)
        {
            int64_t item = monkey->items[0];
            item = operator(monkey->operation.operator, item, monkey->operation.operand);
#ifdef PART_TWO
            item %= common_modulo;
#else
            item /= 3;
#endif

            remove_item(monkey, 0);
            if (item % monkey->test.divisible_by == 0)
                add_item(&monkeys[monkey->test.monkey_true], item);
            else
                add_item(&monkeys[monkey->test.monkey_false], item);

            ++monkey->inspected;
        }
    }
}

// static int comparator(const void* a, const void* b) 
// {
//     monkey_t* m_a = (monkey_t*)a;
//     monkey_t* m_b = (monkey_t*)b;
//     return m_a->inspected - m_b->inspected;
//     //m_a->inspected == m_b->inspected ? 0 : m_a->inspected < m_b->inspected ? 1 : -1;
// }

int main()
{
    FILE* f = fopen(INPUT_FILE, "r");
    if (f == NULL)
        return EXIT_FAILURE;

    char op;
    int value;
    monkey_t* monkey;

    char line_buffer[512];
    while (fgets(line_buffer, sizeof(line_buffer), f))
    {
        if (sscanf(line_buffer, "Monkey %d\n", &value) == 1)
        {
            printf("new monkey %d\n", value);
            monkey = new_monkey();
        }
        else if (strstr(line_buffer, "  Starting items:") != NULL)
        {
            char* token = strtok(line_buffer + 18, ", \n");
            do
                add_item(monkey, atoi(token));
            while ((token = strtok(NULL, ", \n")));
        }
        else if (sscanf(line_buffer, "  Operation: new = old %c %d\n", &op, &value) == 2)
            monkey->operation = (operation_t) { .operator = op, .operand = value, };
        else if (sscanf(line_buffer, "  Test: divisible by %d\n", &value) == 1)
        {
            monkey->test.divisible_by = value;
#ifdef PART_TWO
            if (common_modulo == 0)
                common_modulo = value;
            else
                common_modulo *= value;
#endif
        }
        else if (sscanf(line_buffer, "    If true: throw to monkey %d\n", &value) == 1)
            monkey->test.monkey_true = value;
        else if (sscanf(line_buffer, "    If false: throw to monkey %d\n", &value) == 1)
            monkey->test.monkey_false = value;
    }

    for (uint32_t i = 0; i < NUM_ROUNDS; ++i)
    {
        printf("Round %d\n", i + 1);
        do_round();
        for (size_t j = 0; j < num_monkeys; ++j)
            print_monkey_items(j);
        puts("-------------------");
    }

    for (size_t i = 0; i < num_monkeys; ++i)
        printf("Monkey %zu: %lu items inspected\n", i, monkeys[i].inspected);

    // monkey_t* most_active[num_monkeys];
    // for (size_t i = 0; i < num_monkeys; ++i)
    //     most_active[i] = &monkeys[i];
    // qsort(most_active, sizeof(most_active) / sizeof(*most_active), sizeof(*most_active), comparator);

    // uint64_t monkey_business = most_active[0]->inspected * most_active[1]->inspected;

    // printf
    // (
    //     "Most active:\n"
    //     "  Monkey %zu: %lu items inspected\n"
    //     "  Monkey %zu: %lu items inspected\n"
    //     "Total monkey business: %lu\n",
    //     0L, most_active[0]->inspected,
    //     1L, most_active[1]->inspected,
    //     monkey_business
    // );

    fclose(f);
    return EXIT_SUCCESS;
}
