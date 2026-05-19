#include <stdio.h>
#include <string.h>

int main() {
    int c;
    scanf("%d", &c); // number of test cases

    double dp[105]; // max n is 100, a little extra buffer

    for (int t = 0; t < c; t++) {
        int n, p;
        scanf("%d %d", &n, &p);

        // Reset dp array
        for (int i = 0; i <= n; i++) dp[i] = 0.0;
        dp[0] = 1.0;

        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= p; j++) {
                if (i - j >= 0) {
                    dp[i] += (1.0 / p) * dp[i - j];
                }
            }
        }

        printf("%.9f\n", dp[n]);
    }

    return 0;
}
