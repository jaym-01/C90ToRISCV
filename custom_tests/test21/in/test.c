struct test1 {
    int x;
    int y;
};

int test() {
    int test1;
    // struct test1 {
    //     int x;
    //     int y;
    // };

    struct test1 t = {
        10, 20
    };

    return t.x + t.y;
}
