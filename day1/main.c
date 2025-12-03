#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_DIAL 100

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "usage: %s input\n", argv[0]);
        return 1;
    }

    FILE *f = fopen(argv[1], "r");
    char line[256];

    int pointing_to = 50;

    int times_at_0 = 0;

    while(fgets(line, sizeof(line), f))
    {
        int direction = 0;
        if (line[0] == 'R') direction = 1;
        else if (line[0] == 'L') direction = -1;
        else assert(0 && "Invalid direction character");

        long how_much = strtol(line+1, NULL, 10);
        assert(how_much > 0);

        for (int i = 0; i < how_much; i++)
        {
            pointing_to += direction;
            pointing_to = ((pointing_to % MAX_DIAL) + MAX_DIAL) % MAX_DIAL;

            if (pointing_to == 0) times_at_0++;
        }
    }

    printf("Times at 0: %d\n", times_at_0);

    return 0;
}
