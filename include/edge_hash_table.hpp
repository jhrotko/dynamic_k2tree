#ifndef __HASH_H__
#define __HASH_H__

#include <vector>
#include <stdexcept>
#include <iostream>
#include <algorithm>

using namespace std;

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
    edge_hash_table(vector<edge> new_elements)
    {
        table.resize(new_elements.size() * 2);
        table.assign(table.size(), EMPTY);

        this->elements.assign(new_elements.begin(), new_elements.end());
        number_of_elements = 0;

        for (size_t i = 0; i < new_elements.size(); i++)
        {
            insert(i);
        }
    }

    edge_hash_table(size_t size, vector<edge> new_elements)
    {
        table.resize(size);
        table.assign(table.size(), EMPTY);

        this->elements.assign(new_elements.begin(), new_elements.end());
        number_of_elements = new_elements.size();

        for (size_t i = 0; i < number_of_elements; i++)
            insert(i);
    }

    void insert(uint32 value, vector<int> *ht = NULL)
    {
        if (ht == NULL)
            ht = &table;

        if (number_of_elements > ht->size() / 2)
            resize_table();

        uint32 key = edge_to_uint64(elements[value]);
        uint32 i = hash_function(key);
        while ((*ht)[i] != EMPTY)
            i = (i + 1) % table.size();

        (*ht)[i] = value;
        number_of_elements++;
    }

    int find(edge e) 
    {
        uint32 i = edge_to_uint64(e);
        while(table[i] != EMPTY) {
            if(e.x == elements[table[i]].x && e.y == elements[table[i]].y)
                return table[i];
            else
                i = (i + 1) % table.size();
        }   

        return -1;
    }

    void add_element(edge e) 
    {
        elements.push_back(e);
        insert(number_of_elements);
    }

    int get_key(int key) // maybe delete me later
    {
        std::vector<int>::iterator it = std::find(table.begin(), table.end(), key);
        if (it != table.end())
            return *it;
        else
            return -1;
    }

    vector<int> get_table()
    {
        return table;
    }

    vector<edge> get_elements()
    {
        return elements;
    }

private:

    /* Resizes the current table with the double size of the previous one*/
    void resize_table()
    {
        vector<int> new_table;

        new_table.resize(table.size() * 2);
        new_table.assign(new_table.size(), EMPTY);
        number_of_elements = 0;

        for (size_t i = 0; i < table.size(); i++)
            if (table[i] != EMPTY)
                insert(table[i], &new_table);
        this->table = new_table;
    }

    /* concatenates the values of x and y into a 64 bit (32*2)*/
    uint64 edge_to_uint64(edge e)
    {
        uint64 concat_edge = e.x;
        concat_edge <<= 32;
        concat_edge |= e.y;

        return concat_edge;
    }

    /* Converts edge x,y (key) into a uint 32 */
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

    vector<int> table;
    vector<edge> elements;
    size_t number_of_elements;
};

#endif