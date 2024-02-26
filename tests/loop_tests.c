// Test while looop
int test1() {
    int x = 3;
    while (x < 10) {
        x++;
    }
    return x;
}

// Test do while
int test2() {
    int x = 3;
    do
    {
        x++;
    } while (x < 14);
    return x;
}

// Test loops with break and continue
int test3() {
    int x = 3;


    int i;
    for (i = 0; i < 10; i++)
    {
        x = i;

        if (i == 0)
        {
            continue;
        }

        while (1)
        {
            x++;
            if (x == 2)
            {
                continue;
            }
            if (x == 3)
            {
                break;
            }
        }

        break;
    }

    return x;
}

// Test break continue with do while
int test4() {
    int x = 3;

    do
    {
        break;
        x++;
        continue;

        x--;
    } while (x < 4);

    return x;
}
