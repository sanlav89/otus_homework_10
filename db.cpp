#include "db.h"

namespace db {

void printTable(const table_t &table, std::ostream &os)
{
    for (const auto &pair : table) {
        os << pair.first;
        for (const auto &name : pair.second) {
            os << "," << name;
        }
        os << std::endl;
    }
}

namespace query {

bool insert(table_t &table, const id_t &id, const name_t &name)
{
    if (table.find(id) != table.end()) {
        return false;
    }
    table[id] = name;
    return true;
}

bool truncate(table_t &table)
{
    table.clear();
    return true;
}

table_t intersection(const table_t &table1, const table_t &table2)
{
    table_t result;
    auto it1 = table1.begin();
    auto it2 = table2.begin();

    name_t tmp;
    tmp.reserve(it1->second.size() + it2->second.size());

    while (true) {
        if (it1->first < it2->first) {
            it1++;
            if (it1 == table1.end()) {
                break;
            }
        } else if (it1->first > it2->first) {
            it2++;
            if (it2 == table2.end()) {
                break;
            }
        } else {
            tmp.insert(tmp.begin(), it1->second.begin(), it1->second.end());
            tmp.insert(tmp.end(), it2->second.begin(), it2->second.end());
            result[it1->first] = tmp;
            tmp.clear();
            it1++;
            it2++;
            if (it1 == table1.end() || it2 == table2.end()) {
                break;
            }
        }
    }
    return result;
}

table_t symdifference(const table_t &table1, const table_t &table2)
{
    table_t result;
    auto it1 = table1.begin();
    auto it2 = table2.begin();

    name_t tmp;
    tmp.reserve(it1->second.size() + it2->second.size());

    while (true) {
        if (it1->first < it2->first) {
            tmp.insert(tmp.begin(), it1->second.begin(), it1->second.end());
            tmp.insert(tmp.end(), {""});
            result[it1->first] = tmp;
            tmp.clear();
            it1++;
            if (it1 == table1.end()) {
                break;
            }
        } else if (it1->first > it2->first) {
            tmp.insert(tmp.begin(), {""});
            tmp.insert(tmp.end(), it2->second.begin(), it2->second.end());
            result[it2->first] = tmp;
            tmp.clear();
            it2++;
            if (it2 == table2.end()) {
                break;
            }
        } else {
            it1++;
            it2++;
            if (it1 == table1.end() || it2 == table2.end()) {
                break;
            }
        }
    }

    while (it1 != table1.end()) {
        tmp.insert(tmp.begin(), it1->second.begin(), it1->second.end());
        tmp.insert(tmp.end(), {""});
        result[it1->first] = tmp;
        tmp.clear();
        it1++;
    }

    while (it2 != table2.end()) {
        tmp.insert(tmp.begin(), {""});
        tmp.insert(tmp.end(), it2->second.begin(), it2->second.end());
        result[it2->first] = tmp;
        tmp.clear();
        it2++;
    }

    return result;
}

}

}
