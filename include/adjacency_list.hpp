#ifndef __ADJACENCY_LIST_H__
#define __ADJACENCY_LIST_H__

#include <vector>
#include "utils.hpp"
#include <iostream>

class item
{
public:
    int x, next;
    item() {}
    item(int x, int next) : x(x), next(next) {}
};

class adjacency_list
{
private:
    std::vector<item> table;
    uint n_elements;

public:
    adjacency_list() : n_elements(0) {}
    adjacency_list(size_t size) {
        n_elements = 0;
        table.resize(size);
        table.assign(table.size(), item(-1, -1));
    }

    adjacency_list(std::vector<item> elements)
    {
        n_elements = 0;
        table.resize(elements.size());
        table.assign(table.size(), item(-1, -1));
        for (size_t i = 0; i < elements.size(); i++)
            insert(elements[i]);
    }

    item &operator[](size_t idx)
    {
        return table[idx];
    }

    void insert(item i)
    {
        insert(i.x, i.next);
    }

    void insert(uint x, uint next)
    {
        item new_item(x, next);
        if (n_elements > (table.size() >> 1))
        {
            std::vector<item> table_old = table;
            table.resize(table.size() << 1);
            table.assign(table.size(), item(-1, -1));
            n_elements = 0;

            uint k;
            for (k = 0; k < (table.size() >> 1); k++)
                if (table_old[k].x != -1)
                    insert(table_old[k].x, table_old[k].next);
        }

        uint i = hash32shift(x) % (table.size());
        while (table[i].x != -1)
            i = (i + 1) % (table.size());

        table[i].x = x;
        table[i].next = next;
        n_elements++;
    }

    int find(int x)
    {
        uint32_t i = hash32shift(x) % (table.size());

        while (table[i].x != -1)
            if (x == table[i].x)
                return i;
            else
                i = (i + 1) % (table.size());

        return -1;
    }

    uint size()
    {
        return n_elements;
    }

    void clear() {
        table.assign(table.size(), item(-1,-1));
        n_elements = 0;
    }
};

#endif
