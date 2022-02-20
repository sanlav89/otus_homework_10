#pragma once

#include <map>
#include <list>
#include <array>
#include <string>
#include <cassert>
#include <shared_mutex>

namespace db {

using id_t = int;
using name_t = std::string;
using tablename_t = std::string;

/**
 *  Класс Table - одна таблица из базы данных
 *  Для простоты предположил, что для хранения БД есть некий ограниченный
 *  объем памяти, поэтому воспользовался std::array для моделирования этой
 *  памяти. Не стал заморачиваться с хранением на жестком диске и т.п.
 *  Выбор такой структуры данных обусловлен тем, что будут минимизированы
 *  блокировки: блокируется только конкретная строка из таблицы (не вся таблица).
 *  Для более быстрого доступа к элементам  array организована вспомогательная
 *  структура данных на std::map, которая хранит соответсвие {id, индекс}
 */
template<std::size_t MaxSize = 100>
class Table
{
    using storage_t = std::array<name_t, MaxSize>;
    using map_t = std::map<id_t, std::size_t>;
    using mapiter_t = map_t::iterator;

public:

    class TableIterator
    {
    public:
        TableIterator() = default;
        TableIterator(const mapiter_t &it, const storage_t &data)
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
            assert(m_it->second < MaxSize);
            return m_data[m_it->second];
        }

    private:
        mapiter_t m_it;
        const storage_t &m_data;
    };


    Table(const tablename_t &name)
        : m_name(name)
        , m_currentSize(0)
    {
    }

    Table(const Table &) = delete;
    Table(Table &&) = delete;

    bool insert(const id_t &id, const name_t &name)
    {
        if (m_map.find(id) != m_map.end()) {
            return false;
        }

        if (m_currentSize < MaxSize) {
            m_mutex.lock();
            m_data[m_currentSize] = name;
            m_map[id] = m_currentSize++;
            m_mutex.unlock();
            return true;
        } else {
            return false;
        }

    }

    void truncate()
    {
        m_map.clear();
        m_currentSize = 0;
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
    std::size_t m_currentSize;
    std::mutex m_mutex;

};

}

