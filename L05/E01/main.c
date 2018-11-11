#include <stdio.h>
#include <stdlib.h>

#define MAX_SONG_LEN 255 + 1

static int count = 0;

void playlists(char*** friends_songs, int* friends_songs_len, int friends_depth, int friends_len, char** solution)
{
    // Termination condition
    if (friends_depth == friends_len) {
        printf("%d\n", ++count);
        for (int i = 0; i < friends_len; i++) {
            printf("%s\n", solution[i]);
        }

        return;
    }

    // Foreach song of this friend
    for (int i = 0; i < friends_songs_len[friends_depth]; i++) {
        // Put in the solution a song from this friend's choiche
        solution[friends_depth] = friends_songs[friends_depth][i];

        // Recursive call increasing depth
        playlists(friends_songs, friends_songs_len, friends_depth + 1, friends_len, solution);
    }

    return;
}

int main()
{
    int friends_len = 0;
    FILE* fp = fopen("brani.txt", "r");
    if (fp == NULL) {
        perror("Couldn't open file");
        return 1;
    }

    // Parse input file
    fscanf(fp, "%d ", &friends_len);

    char*** friends_songs = (char***)calloc(friends_len, sizeof(char***));
    int* friends_songs_len = (int*)calloc(friends_len, sizeof(int));

    for (int i = 0; i < friends_len; i++) {
        int songs_len = 0;
        fscanf(fp, "%d ", &songs_len);
        friends_songs_len[i] = songs_len;

        friends_songs[i] = (char**)calloc(songs_len, sizeof(char**));

        for (int j = 0; j < songs_len; j++) {
            friends_songs[i][j] = (char*)calloc(MAX_SONG_LEN, sizeof(char));
            fscanf(fp, "%s", friends_songs[i][j]);
        }
    }

    // Initialize solution array
    char** solution = (char**)calloc(friends_len, MAX_SONG_LEN * sizeof(char));

    // Perform recursive solution
    playlists(friends_songs, friends_songs_len, 0, friends_len, solution);

    // Cleanup
    free(solution);
    for (int i = 0; i < friends_len; i++) {
        for (int j = 0; j < friends_songs_len[i]; j++) {
            free(friends_songs[i][j]);
        }
        free(friends_songs[i]);
    }
    free(friends_songs);
    fclose(fp);

    return 0;
}
