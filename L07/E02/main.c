#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define SAPPHIRE_VALUE 5
#define EMERALD_VALUE 10
#define RUBY_VALUE 15
#define TOPAZ_VALUE 20

#define MAX_RIP 4

typedef enum {
    sapphire,
    emerald,
    ruby,
    topaz
} gems;

static gems* best_sol = NULL;
static int best_sol_value = 0;
static int best_sol_len = 0;

void print_solution(gems* solution, int solution_len)
{
    if (solution_len == 0) {
        printf("{}\n");
        return;
    }

    printf("{");
    for (int i = 0; i < solution_len; i++) {
        printf("%d, ", solution[i]);
    }
    printf("\b\b}\n");
}

bool check(gems* solution, int solution_len)
{
    int sapphires = 0, emeralds = 0, rubies = 0, topazes = 0;
    for (int i = 0; i < solution_len - 1; i++) {
        int next = solution[i + 1];

        switch (solution[i]) {
        case sapphire:
            if (next == sapphire || next == ruby) {
            } else {
                return false;
            }
            sapphires++;
            break;
        case emerald:
            if (next == emerald || next == topaz) {
            } else {
                return false;
            }
            emeralds++;
            break;
        case ruby:
            if (next == emerald || next == topaz) {
            } else {
                return false;
            }
            rubies++;
            break;
        case topaz:
            if (next == sapphire || next == ruby) {
            } else {
                return false;
            }
            topazes++;
            break;
        }
    }

    // Handle last element
    switch (solution[solution_len - 1]) {
    case sapphire:
        sapphires++;
        break;
    case emerald:
        emeralds++;
        break;
    case ruby:
        rubies++;
        break;
    case topaz:
        topazes++;
        break;
    }

    if (sapphires > MAX_RIP || emeralds > MAX_RIP || rubies > MAX_RIP || topazes > MAX_RIP) {
        return false;
    }

    if (sapphires > emeralds)
        return false;

    return true;
}

int value(gems* solution, int solution_len)
{
    int value = 0;

    for (int i = 0; i < solution_len; i++) {
        switch (solution[i]) {
        case sapphire:
            value += SAPPHIRE_VALUE;
            break;
        case emerald:
            value += EMERALD_VALUE;
            break;
        case ruby:
            value += RUBY_VALUE;
            break;
        case topaz:
            value += TOPAZ_VALUE;
            break;
        }
    }
    return value;
}

void solve(int pos, gems* sol, int termination_len, int s, int e, int r, int t)
{
    if (pos == termination_len) {
        if (check(sol, termination_len)) {
            int sol_value = value(sol, termination_len);
            if (sol_value > best_sol_value) {
                if (best_sol != NULL) {
                    free(best_sol);
                }
                best_sol = calloc(termination_len, sizeof(gems));

                for (int i = 0; i < termination_len; i++) {
                    best_sol[i] = sol[i];
                }
                best_sol_value = sol_value;
                best_sol_len = termination_len;
            }
        }
        return;
    }

    // If we can do pruning based on the past gem
    if (pos > 0) {
        int past = sol[pos - 1];
        if (s > 0 && (past == sapphire || past == topaz)) {
            sol[pos] = sapphire;
            solve(pos + 1, sol, termination_len, s - 1, e, r, t);
        }
        if (e > 0 && (past == emerald || past == ruby)) {
            sol[pos] = emerald;
            solve(pos + 1, sol, termination_len, s, e - 1, r, t);
        }
        if (r > 0 && (past == sapphire || past == topaz)) {
            sol[pos] = ruby;
            solve(pos + 1, sol, termination_len, s, e, r - 1, t);
        }
        if (t > 0 && (past == ruby || past == emerald)) {
            sol[pos] = topaz;
            solve(pos + 1, sol, termination_len, s, e, r, t - 1);
        }
    } else {
        if (s > 0) {
            sol[pos] = sapphire;
            solve(pos + 1, sol, termination_len, s - 1, e, r, t);
        }
        if (e > 0) {
            sol[pos] = emerald;
            solve(pos + 1, sol, termination_len, s, e - 1, r, t);
        }
        if (r > 0) {
            sol[pos] = ruby;
            solve(pos + 1, sol, termination_len, s, e, r - 1, t);
        }
        if (t > 0) {
            sol[pos] = topaz;
            solve(pos + 1, sol, termination_len, s, e, r, t - 1);
        }
    }

    return;
}

int main(int argc, char const* argv[])
{
    if (argc != 5) {
        printf("Usage: %s sapphires emeralds rubies topazes\n", argv[0]);
        return 1;
    }
    int sapphires = atoi(argv[1]);
    int emeralds = atoi(argv[2]);
    int rubies = atoi(argv[3]);
    int topazes = atoi(argv[4]);

    // Find necklace with max lenght which respects the current constraints
    // - sapphire must be followed by sapphire or ruby
    // - emerald must be followed by emerald or topaz
    // - ruby must be followed by emerald or topaz
    // - topaz must be followed by sapphire or ruby
    // - 1 <= length(necklace) <= s+e+r+t

    int total = sapphires + emeralds + rubies + topazes;
    gems* solution = calloc(total, sizeof(gems));

    // TODO: add for to iterate necklace lenghts
    // TODO: add maximum search

    for (int k = 1; k <= total; k++) {
        solve(0, solution, k, sapphires, emeralds, rubies, topazes);
    }

    print_solution(best_sol, best_sol_len);
    printf("%d\n", value(best_sol, best_sol_len));

    free(solution);
    return 0;
}
