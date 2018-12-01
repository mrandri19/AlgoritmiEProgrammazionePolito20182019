#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int start;
    int finish;
} att;

void check(int* sol, int len, int* max_duration, int* max_sol, att* durations)
{
    // Check if there are overlappings
    for (int i = 0; i < len; ++i) {
        // Dont' perform the check if it wasnt selected
        if (!sol[i]) {
            continue;
        }

        att a = durations[i];

        for (int j = 0; j < len; ++j) {
            // Dont' perform the check if it wasnt selected
            if (!sol[j]) {
                continue;
            }
            // Don't check if it overlaps with itself
            if (i == j) {
                continue;
            }
            att b = durations[j];

            // Check for overlappings

            // A ******
            // B     *************
            // overlapping

            // A           *******
            // B ******
            // non overlapping because of first condition

            // A ******
            // B          ********
            // non overlapping because of second condition

            if (a.start < b.finish && b.start < a.finish) {
                return;
            }
        }
    }

    // Sum durations
    int duration = 0;
    for (int i = 0; i < len; ++i) {
        if (sol[i]) {
            duration += (durations[i].finish - durations[i].start);
        }
    }
    if (*max_duration < duration) {
        *max_duration = duration;

        for (int i = 0; i < len; i++) {
            max_sol[i] = sol[i];
        }
    }
}

void powerset(int pos, int len, int* sol, int* max_duration, int* max_sol, att* durations)
{
    if (pos == len) {
        check(sol, len, max_duration, max_sol, durations);
        return;
    }

    sol[pos] = 0;
    powerset(pos + 1, len, sol, max_duration, max_sol, durations);
    sol[pos] = 1;
    powerset(pos + 1, len, sol, max_duration, max_sol, durations);
    return;
}

void activSel(int N, att* v)
{
    int* sol = calloc(N, sizeof(int));

    int max_duration = 0;
    int* max_sol = calloc(N, sizeof(int));

    powerset(0, N, sol, &max_duration, max_sol, v);

    printf("%d\n", max_duration);
    // Print all chosen activites
    for (int i = 0; i < N; i++) {
        if (max_sol[i]) {
            printf("{%d, %d} ", v[i].start, v[i].finish);
        }
    }
    printf("\n");
}

int main()
{
    att S[] = { { 1, 2 }, { 2, 4 }, { 2, 5 }, { 3, 5 }, { 5, 7 }, { 6, 8 } };
    int len = sizeof(S) / sizeof(S[0]);

    // maximize duration
    // Generate powerset
    // TODO: can this be done with dynamic programming?
    activSel(len, S);

    return 0;
}
