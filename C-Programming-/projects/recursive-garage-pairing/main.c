#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define MAX_GARAGES 16 // Maximum of 2n garages, n <= 8

typedef struct {
    int x, y;
    char name[21];
} Garage;

Garage garages[MAX_GARAGES];
int used[MAX_GARAGES] = {0};
int n;
double minDistance = 1e9;
int bestPairs[MAX_GARAGES];
int pairings[MAX_GARAGES][2]; // To store the best pairs

double distance(int i, int j) {
    return sqrt((garages[i].x - garages[j].x) * (garages[i].x - garages[j].x) +
                (garages[i].y - garages[j].y) * (garages[i].y - garages[j].y));
}

void findPairs(int pairCount, double currentDistance) {
    if (pairCount == n) {
        if (currentDistance < minDistance) {
            minDistance = currentDistance;
            for (int i = 0; i < n; i++) {
                pairings[i][0] = bestPairs[2 * i];
                pairings[i][1] = bestPairs[2 * i + 1];
            }
        }
        return;
    }

    for (int i = 0; i < 2 * n; i++) {
        if (!used[i]) {
            for (int j = i + 1; j < 2 * n; j++) {
                if (!used[j]) {
                    // Pair garages i and j
                    used[i] = used[j] = 1;
                    bestPairs[pairCount * 2] = i;
                    bestPairs[pairCount * 2 + 1] = j;

                    findPairs(pairCount + 1, currentDistance + distance(i, j));

                    // Backtrack
                    used[i] = used[j] = 0;
                }
            }
            return; // No need to continue after pairing i
        }
    }
}

int main() {
    scanf("%d", &n);
    for (int i = 0; i < 2 * n; i++) {
        scanf("%d %d %s", &garages[i].x, &garages[i].y, garages[i].name);
    }

    findPairs(0, 0.0);

    printf("%.3f\n", minDistance);
    for (int i = 0; i < n; i++) {
        printf("(%s, %s, %.3f)\n", garages[pairings[i][0]].name, garages[pairings[i][1]].name, distance(pairings[i][0], pairings[i][1]));
    }
    return 0;
}
