#pragma once

#include <iostream>
#include <memory>
#include "table.h"

namespace db {

class IDataBase
{
public:
    virtual std::string insert(const tablename_t &tableName, const id_t &id, const name_t &name) = 0;
    virtual std::string truncate(const tablename_t &tableName) = 0;
    virtual std::string intersection() = 0;
    virtual std::string symdifference() = 0;
};

class DataBase : public IDataBase
{
public:
    DataBase();
    ~DataBase() = default;

    /**
     * @brief insert
     * Complexity: O(log(N)), N = table.size()
     */
    std::string insert(const tablename_t &tableName, const id_t &id, const name_t &name) override;

    /**
     * @brief truncate
     * Complexity: O(N), N = table.size()
     */
    std::string truncate(const tablename_t &tableName) override;

    /**
     * @brief intersection
     * Complexity: O(min{N1, N2}), N1 = table1.size(), N2 = table2.size()
     */
    std::string intersection() override;

    /**
     * @brief symdifference
     * Complexity: O(max{N1, N2}), N1 = table1.size(), N2 = table2.size()
     */
    std::string symdifference() override;

private:

    std::unique_ptr<Table<>> m_tableA;
    std::unique_ptr<Table<>> m_tableB;

    void loadInitData();

};

}
