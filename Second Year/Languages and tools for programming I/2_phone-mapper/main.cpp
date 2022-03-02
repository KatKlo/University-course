#include <iostream>
#include "maptel.h"

int main() {
    auto id{jnp1::maptel_create()};
    char t1[] = "123456";
    char t3[3];
    for (int i = 0; i < 3; i++) {
        t3[i] = t1[i];
    }
    jnp1::maptel_insert(id, t1, t3);

}

