#include <stdio.h>
#include <limits.h>

#define INFINITY INT_MAX

int main()
{
    int graph[4][4] = {
        {0, INFINITY, 50, INFINITY},
        {40, 0, 10, INFINITY},
        {INFINITY, 30, 0, 20},
        {90, 20, INFINITY, 0}};

    for (int k = 0; k < 4; k++)
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                if (graph[i][k] != INFINITY && graph[k][j] != INFINITY && graph[i][k] + graph[k][j] < graph[i][j])
                {
                    graph[i][j] = graph[i][k] + graph[k][j];
                }
            }
        }
    }

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (graph[i][j] == INFINITY)
            {
                printf("INFINITY ");
            }
            else
            {
                printf("%d ", graph[i][j]);
            }
        }
        printf("\n");
    }

    return 0;
}
