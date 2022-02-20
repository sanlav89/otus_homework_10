#include <gtest/gtest.h>
#include <iostream>
#include "db.h"

TEST(DbTest, Insert)
{
    db::DataBase db;
    EXPECT_FALSE(db.insert("A", 0, {"new value"}) == "OK\n");
    EXPECT_TRUE(db.insert("A", 6, {"new value"}) == "OK\n");
}

TEST(DbTest, Truncate)
{
    db::DataBase db;
    EXPECT_TRUE(db.truncate("A") == "OK\n");
    EXPECT_FALSE(db.truncate("C") == "OK\n");
}

TEST(DbTest, Intersection)
{
    db::DataBase db;
    auto result = db.intersection();
    EXPECT_TRUE(result == "3,violation,proposal\n"
                          "4,quality,example\n"
                          "5,precision,lake\n"
                          "OK\n");
}

TEST(DbTest, SymDifference)
{
    db::DataBase db;
    auto result = db.symdifference();
    EXPECT_TRUE(result == "0,lean,\n"
                          "1,sweater,\n"
                          "2,frank,\n"
                          "6,,flour\n"
                          "7,,wonder\n"
                          "8,,selection\n"
                          "OK\n");
}
