

int test1() {
    int x = 2;
    int y = 2;

    if (x < 3) {
        int x = 5;
        return x;
    } else if (y < 3) {
        x = 4;
    } else {
        x = 8;
    }

    return x;
}
