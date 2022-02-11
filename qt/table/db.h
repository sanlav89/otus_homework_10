#pragma once

#include <map>
#include <vector>
#include <string>

namespace db {

using id_t = int;
using name_t = std::vector<std::string>;
using table_t = std::map<id_t, name_t>;

namespace query {

/**
 * @brief insert
 * Complexity: O(log(N)), N = table.size()
 */
bool insert(table_t &table, const id_t &id, const name_t &name);

/**
 * @brief truncate
 * Complexity: O(N), N = table.size()
 */
bool truncate(table_t &table);

/**
 * @brief intersection
 * Complexity: O(min{N1, N2}), N1 = table1.size(), N2 = table2.size()
 */
table_t intersection(const table_t &table1, const table_t &table2);

/**
 * @brief symdifference
 * Complexity: O(max{N1, N2}), N1 = table1.size(), N2 = table2.size()
 */
table_t symdifference(const table_t &table1, const table_t &table2);

}

}
