// If else
int test1() {
    int x = 3;
    int y = 2;

    if (x < 3) {
        x = 3;
    }
    else if (y < 3) {
        x = 4;
    }
    else {
        x = 8;
    }
    return x;
}

// Nested if else
int test2()
{
    int x = 5;
    int y = 8;

    if (x < 3) {
        x = 3;
    }
    else if (y < 3) {
        if (x < 5) {
            x = 4;
        }
        else {
            x = 8;
        }
    }
    else {
        x = 13;
    }

    return x;
}
