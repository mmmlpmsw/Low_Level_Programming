#include <stdio.h>

int is_prime(unsigned long n) {
    size_t d = 2;
    while (d * d <= n && n % d != 0)
        d ++;
    if (d * d > n)
        return 1;
    else
        return 0;
}

int main() {
    unsigned long n = 0;
    printf("Input unsigned long : ");
    scanf("%lu", &n);
    printf("(%lu) Is prime : ", n);
    printf(is_prime(n) ? "Yes\n" : "No\n");
    return 0;
}

