#include <gtest/gtest.h>
#include <iostream>
#include "db.h"

TEST(DbTest, Print)
{
    db::table_t tableA = {
        {0, {"lean"}},
        {1, {"sweater"}},
        {2, {"frank"}},
        {3, {"violation"}},
        {4, {"quality"}},
        {5, {"precision"}}
    };

    std::ostringstream oss;
    db::printTable(tableA, oss);
    EXPECT_TRUE(oss.str() == "0,lean\n"
                             "1,sweater\n"
                             "2,frank\n"
                             "3,violation\n"
                             "4,quality\n"
                             "5,precision\n");
}

TEST(DbTest, Insert)
{
    db::table_t tableA = {
        {0, {"lean"}},
        {1, {"sweater"}},
        {2, {"frank"}},
        {3, {"violation"}},
        {4, {"quality"}},
        {5, {"precision"}}
    };

    EXPECT_FALSE(db::query::insert(tableA, 0, {"new value"}));
    EXPECT_TRUE(db::query::insert(tableA, 6, {"new value"}));

    std::ostringstream oss;
    db::printTable(tableA, oss);
    EXPECT_TRUE(oss.str() == "0,lean\n"
                             "1,sweater\n"
                             "2,frank\n"
                             "3,violation\n"
                             "4,quality\n"
                             "5,precision\n"
                             "6,new value\n");
}

TEST(DbTest, Truncate)
{
    db::table_t tableA = {
        {0, {"lean"}},
        {1, {"sweater"}},
        {2, {"frank"}},
        {3, {"violation"}},
        {4, {"quality"}},
        {5, {"precision"}}
    };
    EXPECT_TRUE(db::query::truncate(tableA));

    std::ostringstream oss;
    db::printTable(tableA, oss);
    EXPECT_TRUE(oss.str() == "");
}

TEST(DbTest, Intersection)
{
    db::table_t tableA = {
        {0, {"lean"}},
        {1, {"sweater"}},
        {2, {"frank"}},
        {3, {"violation"}},
        {4, {"quality"}},
        {5, {"precision"}}
    };

    db::table_t tableB = {
        {3, {"proposal"}},
        {4, {"example"}},
        {5, {"lake"}},
        {6, {"flour"}},
        {7, {"wonder"}},
        {8, {"selection"}}
    };

    auto AB = db::query::intersection(tableA, tableB);
//    auto A_B = db::query::symdifference(A, B);

    std::ostringstream oss;
    db::printTable(AB, oss);
    EXPECT_TRUE(oss.str() == "3,violation,proposal\n"
                             "4,quality,example\n"
                             "5,precision,lake\n");
}

TEST(DbTest, SymDifference)
{
    db::table_t tableA = {
        {0, {"lean"}},
        {1, {"sweater"}},
        {2, {"frank"}},
        {3, {"violation"}},
        {4, {"quality"}},
        {5, {"precision"}}
    };

    db::table_t tableB = {
        {3, {"proposal"}},
        {4, {"example"}},
        {5, {"lake"}},
        {6, {"flour"}},
        {7, {"wonder"}},
        {8, {"selection"}}
    };

    auto AB = db::query::symdifference(tableA, tableB);

    std::ostringstream oss;
    db::printTable(AB, oss);
    EXPECT_TRUE(oss.str() == "0,lean,\n"
                             "1,sweater,\n"
                             "2,frank,\n"
                             "6,,flour\n"
                             "7,,wonder\n"
                             "8,,selection\n");
}
