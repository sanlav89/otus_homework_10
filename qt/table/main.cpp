#include <iostream>
#include "db.h"

int main()
{
    std::cout << "Hello World!" << std::endl;

    db::table_t A = {
        {0, {"lean"}},
        {1, {"sweater"}},
        {2, {"frank"}},
        {3, {"violation"}},
        {4, {"quality"}},
        {5, {"precision"}}
    };

    db::table_t B = {
        {3, {"proposal"}},
        {4, {"example"}},
        {5, {"lake"}},
        {6, {"flour"}},
        {7, {"wonder"}},
        {8, {"selection"}}
    };

    auto print = [](const db::table_t &table) {
        for (const auto &pair : table) {
            std::cout << pair.first;
            for (const auto &name : pair.second) {
                std::cout << "," << name;
            }
            std::cout << std::endl;
        }
    };

    auto AB = db::query::intersection(A, B);
    auto A_B = db::query::symdifference(A, B);

    print(AB);
    print(A_B);

    return 0;
}
