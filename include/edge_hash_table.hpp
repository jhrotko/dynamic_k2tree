#ifndef __HASH_H__
#define __HASH_H__

#include <vector>
#include <stdexcept>
#include <iostream>

typedef unsigned int uint32;
typedef unsigned long uint64;

#define EMPTY -1

class edge
{
public:
    uint32 x, y;
    edge(uint32 x, uint32 y) : x(x), y(y) {}
};

class edge_hash_table
{
public:
    edge_hash_table(std::vector<edge> new_elements)
    {
        table.resize(new_elements.size() * 2);
        table.assign(table.size(), EMPTY);

        this->elements.assign(new_elements.begin(), new_elements.end());
        number_of_elements = new_elements.size();

        for (size_t i = 0; i < number_of_elements; i++)
            insert(i);
    }

    edge_hash_table(size_t size, std::vector<edge> new_elements)
    {
        table.resize(size);
        table.assign(table.size(), EMPTY);

        this->elements.assign(new_elements.begin(), new_elements.end());
        number_of_elements = new_elements.size();

        for (size_t i = 0; i < number_of_elements; i++)
            insert(i);
    }

    std::vector<int> get_table()
    {
        return table;
    }

    std::vector<edge> get_elements()
    {
        return elements;
    }

    void insert(uint32 value, std::vector<int> ht = {})
    {
        if (ht.size() == 0)
            ht = table;

        if (number_of_elements > ht.size() / 2)
            resize_table();

        uint32 key = convert_edge_to_uint64(elements[value]);
        uint32 i = hash_function(key);
        while (ht[i] != EMPTY)
            i = (i + 1) % table.size();

        ht[i] = value;
        number_of_elements++;
    }

    size_t find() {}
    void erase() {}

private:
    void resize_table()
    {
        std::vector<int> new_table;

        new_table.resize(table.size() * 2);
        new_table.assign(new_table.size(), EMPTY);

        for (size_t i = 0; i < table.size() / 2; i++)
            if (table[i] != EMPTY)
                insert(table[i], new_table);
        this->table = new_table;
    }

    uint64 convert_edge_to_uint64(edge e)
    {
        uint64 concat_edge = e.x;
        concat_edge <<= 32;
        concat_edge |= e.y;

        return concat_edge;
    }

    /*Converts edge x,y (key) into a uint 32 */
    uint32 hash_function(uint64 key)
    {
        key = (~key) + (key << 18);
        key = key ^ (key >> 31);
        key = (key + (key << 2)) + (key << 4);
        key = key ^ (key >> 11);
        key = key + (key << 6);
        key = key ^ (key >> 22);

        return (uint32)key % table.size();
    }

    std::vector<int> table;
    std::vector<edge> elements;
    size_t number_of_elements;
};

#endif