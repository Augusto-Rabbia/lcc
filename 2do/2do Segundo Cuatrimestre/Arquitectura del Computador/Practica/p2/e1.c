// a)
/*int main() {
    int a = -1;
    return a;
}*/

// b)
int main() {
    int a = 0xFE;
    int b = -1;
    a += b;
    b++;
    return a;
}