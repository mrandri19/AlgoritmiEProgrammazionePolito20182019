#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum {
    sapphire,
    emerald,
    ruby,
    topaz
} gems;

static gems* best_sol = NULL;
static int best_sol_len = 0;

void print_solution(gems* solution, int solution_len)
{
    printf("Collana di massima lunghezza %d\n", solution_len);
    // if (solution_len == 0) {
    //     printf("{}\n");
    //     return;
    // }

    // printf("{");
    // for (int i = 0; i < solution_len; i++) {
    //     printf("%d, ", solution[i]);
    // }
    // printf("\b\b}\n");
}

bool check(gems* solution, int solution_len)
{
    for (int i = 0; i < solution_len - 1; i++) {
        int next = solution[i + 1];
        switch (solution[i]) {
        case sapphire:
            if (next == sapphire || next == ruby) {
                continue;
            } else {
                return false;
            }
        case emerald:
            if (next == emerald || next == topaz) {
                continue;
            } else {
                return false;
            }
        case ruby:
            if (next == emerald || next == topaz) {
                continue;
            } else {
                return false;
            }
        case topaz:
            if (next == sapphire || next == ruby) {
                continue;
            } else {
                return false;
            }
        }
    }
    return true;
}

void solve(int pos, gems* sol, int termination_len, int s, int e, int r, int t)
{

    if (pos == termination_len) {
        if (check(sol, termination_len)) {
            if (termination_len > best_sol_len) {
                // if (best_sol != NULL) {
                //     free(best_sol);
                // }
                // best_sol = calloc(termination_len, sizeof(gems));

                // for (int i = 0; i < termination_len; i++) {
                //     best_sol[i] = sol[i];
                // }
                best_sol_len = termination_len;
            }
        }
        return;
    }

    // If we can do pruning based on the past gem
    if (pos > 0) {
        gems past = sol[pos - 1];
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

int main()
{
    int tests_len;
    fscanf(stdin, "%d", &tests_len);

    for (int i = 0; i < tests_len; i++) {
        int sapphires, emeralds, rubies, topazes;
        fscanf(stdin, "%d %d %d %d ", &sapphires, &rubies, &topazes, &emeralds);

        int total = sapphires + emeralds + rubies + topazes;
        gems* solution = calloc(total, sizeof(gems));

        printf("TEST #%d\n", i + 1);
        printf("zaffiro = %d, rubino = %d, topazio = %d, smeraldo = %d, TOT = %d\n", sapphires, rubies, topazes, emeralds, total);

        for (int k = 1; k <= total; k++) {
            solve(0, solution, k, sapphires, emeralds, rubies, topazes);
        }

        print_solution(best_sol, best_sol_len);

        free(best_sol);
        best_sol = NULL;
        best_sol_len = 0;
        free(solution);
    }

    return 0;
}
