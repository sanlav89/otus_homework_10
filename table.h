#pragma once

#include <map>
#include <vector>
#include <array>
#include <string>
#include <cassert>

namespace db {

using id_t = int;
using name_t = std::string;
using tablename_t = std::string;
using map_t = std::map<id_t, std::pair<std::size_t, std::size_t>>;

/**
 *  Класс Table - одна таблица из базы данных
 *  Постарался соблюсти некоторые принципы проектирования БД. А именно:
 *  - данные хранятся блоками размером ChunkSize;
 *  - доступ к каждому "кортежу" (строке) не блокирует всю таблицу,
 *    блокирует только конкретную строку;
 *  - для быстрого доступа к элементам есть общий словарь, который хранит
 *    индексы;
 *  -
 *
 */
template<std::size_t ChunkSize = 10>
class Table
{
    using storage_t = std::vector<std::array<name_t, ChunkSize>>;

public:

    class TableIterator
    {
    public:
        TableIterator() = default;
        TableIterator(const map_t::iterator &it, const storage_t &data)
            : m_it(it)
            , m_data(data)
        {
        }

        TableIterator(const TableIterator &other)
            : m_it(other.m_it)
            , m_data(other.m_data)
        {
        }

        bool operator==(const TableIterator &other) const
        {
            return m_it == other.m_it;
        }

        bool operator!=(const TableIterator &other) const
        {
            return !(*this == other);
        }

        std::pair<id_t, name_t> operator*() const
        {
            return {first(), second()};
        }

        TableIterator &operator++()
        {
            ++m_it;
            return *this;
        }

        id_t first() const
        {
            return m_it->first;
        }

        name_t second() const
        {
            assert(m_it->second.first < m_data.size());
            assert(m_it->second.second < m_data[m_it->second.first].size());
            return m_data[m_it->second.first][m_it->second.second];
        }

    private:
        map_t::iterator m_it;
        const storage_t &m_data;
    };


    Table(const tablename_t &name)
        : m_name(name)
        , m_currentChunk(0)
        , m_currentNode(0)
    {
        m_data.reserve(10);
    }

    bool insert(const id_t &id, const name_t &name)
    {
        if (m_map.find(id) != m_map.end()) {
            return false;
        }

        if (m_data.empty()) {
            m_data.push_back(std::array<name_t, ChunkSize>());
        }

        if (m_currentNode == ChunkSize) {
            m_currentNode = 0;
            m_data.push_back(std::array<name_t, ChunkSize>());
            m_currentChunk++;
        }

        m_data[m_currentChunk][m_currentNode] = name;
        m_map[id] = {m_currentChunk, m_currentNode};
        m_currentNode++;

        return true;
    }

    void truncate()
    {
        m_map.clear();
        m_data.clear();
        m_currentChunk = 0;
        m_currentNode = 0;
    }

    TableIterator begin()
    {
        return TableIterator(m_map.begin(), m_data);
    }

    TableIterator end()
    {
        return TableIterator(m_map.end(), m_data);
    }

    tablename_t name() const
    {
        return m_name;
    }

private:
    tablename_t m_name;
    map_t m_map;
    storage_t m_data;
    std::size_t m_currentChunk;
    std::size_t m_currentNode;

};

}

