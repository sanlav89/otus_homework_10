#include "db.h"
#include <iostream>

namespace db {

DataBase::DataBase()
    : m_tableA(Table<4>("A"))
    , m_tableB(Table<4>("B"))
{
    loadInitData();
}

std::string DataBase::insert(const tablename_t &tableName, const id_t &id, const name_t &name)
{
    if (tableName == m_tableA.name()) {
        if (m_tableA.insert(id, name)) {
            return "OK\n";
        } else {
            return "ERR duplicate " + std::to_string(id) + "\n";
        }
    } else if (tableName == m_tableB.name()) {
        if (m_tableB.insert(id, name)) {
            return "OK\n";
        } else {
            return "ERR duplicate " + std::to_string(id) + "\n";
        }
    } else {
        return "ERR database doesn't contain table " + tableName;
    }
}

std::string DataBase::truncate(const tablename_t &tableName)
{
    if (tableName == m_tableA.name()) {
        m_tableA.truncate();
        return "OK\n";
    } else if (tableName == m_tableB.name()) {
        m_tableB.truncate();
        return "OK\n";
    } else {
        return "ERR database doesn't contain table " + tableName + "\n";
    }
}

std::string DataBase::intersection()
{
    std::string result;

    auto it1 = m_tableA.begin();
    auto it2 = m_tableB.begin();

    while (true) {
        if (it1.first() < it2.first()) {
            ++it1;
            if (it1 == m_tableA.end()) {
                break;
            }
        } else if (it1.first() > it2.first()) {
            ++it2;
            if (it2 == m_tableB.end()) {
                break;
            }
        } else {

            result.append(std::to_string(it1.first()));
            result.append(",");
            result.append(it1.second());
            result.append(",");
            result.append(it2.second());
            result.append("\n");

            ++it1;
            ++it2;
            if (it1 == m_tableA.end() || it2 == m_tableB.end()) {
                break;
            }
        }
    }

    result.append("OK\n");

    return result;
}

std::string DataBase::symdifference()
{
    std::string result;
    auto it1 = m_tableA.begin();
    auto it2 = m_tableB.begin();

    while (true) {
        if (it1.first() < it2.first()) {

            result.append(std::to_string(it1.first()));
            result.append(",");
            result.append(it1.second());
            result.append(",\n");

            ++it1;
            if (it1 == m_tableA.end()) {
                break;
            }

        } else if (it1.first() > it2.first()) {

            result.append(std::to_string(it2.first()));
            result.append(",,");
            result.append(it2.second());
            result.append("\n");

            ++it2;
            if (it2 == m_tableB.end()) {
                break;
            }

        } else {

            ++it1;
            ++it2;
            if (it1 == m_tableA.end() || it2 == m_tableB.end()) {
                break;
            }

        }
    }

    while (it1 != m_tableA.end()) {
        result.append(std::to_string(it1.first()));
        result.append(",");
        result.append(it1.second());
        result.append(",\n");
        ++it1;
    }

    while (it2 != m_tableB.end()) {
        result.append(std::to_string(it2.first()));
        result.append(",,");
        result.append(it2.second());
        result.append("\n");
        ++it2;
    }

    result.append("OK\n");

    return result;
}

void DataBase::loadInitData()
{
    m_tableA.insert(0, "lean");
    m_tableA.insert(1, "sweater");
    m_tableA.insert(2, "frank");
    m_tableA.insert(3, "violation");
    m_tableA.insert(4, "quality");
    m_tableA.insert(5, "precision");

    m_tableB.insert(3, "proposal");
    m_tableB.insert(4, "example");
    m_tableB.insert(5, "lake");
    m_tableB.insert(6, "flour");
    m_tableB.insert(7, "wonder");
    m_tableB.insert(8, "selection");
}

}
