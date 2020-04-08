#ifndef __HASH_H__
#define __HASH_H__

#include <vector>
#include <tuple>
#include <stdexcept>
#include <algorithm>

#include <unordered_map>

#include <iostream>

using namespace std;

typedef unsigned int uint32;
typedef unsigned long uint64;

#define EMPTY -1

class edge
{
public:
    uint32 x, y, next, prev;
    edge() {}
    edge(uint32 x, uint32 y) : x(x), y(y), next(-1), prev(-1) {}

    edge operator() () const
    {
        return *this;
    }
};

class edge_hash_table
{
private:
    class Hash {
    public:
        uint32  operator() (edge const& e) const
        {
            uint64 key = edge_to_uint64(e);
            key = (~key) + (key << 18);
            key = key ^ (key >> 31);
            key = (key + (key << 2)) + (key << 4);
            key = key ^ (key >> 11);
            key = key + (key << 6);
            key = key ^ (key >> 22);

            return (uint32)key;
        }

        uint64 edge_to_uint64(edge const& e) const
        {
            uint64 concat_edge = e.x;
            concat_edge <<= 32;
            concat_edge |= e.y;

            return concat_edge;
        }
    };

    class Comparator {
    public:
        bool operator() (edge const& e1, edge const& e2) const
        {
            return e1.x == e2.x && e1.y == e2.y;
        }
    };

    typedef unordered_map<edge, int, Hash, Comparator> h_table;

    h_table ht;
public:

    edge_hash_table() = default;
    edge_hash_table(const size_t size)
    {
/*        table.resize(size);
        table.assign(table.size(), EMPTY);
        n_elements = 0;
        elements = vector<edge>(size);
        elements.assign(elements.size(), edge());*/

        ht.reserve(size);
    }

    edge_hash_table(vector<edge> new_elements)
    {
        for (int i = 0; i < new_elements.size(); ++i)
            ht[new_elements[i]] = i;
    }

    void print() {
        for (auto e : ht) {
            cout << "[" << e.first.x << ", "
                 << e.first.y
                 << "] = > " << e.second << '\n';
        }
    }

    void insert(int x, int y, int index)
    {
        ht[edge(x,y)] = index;
    }

    // Returns the index of the index where the edge is.
    // Returns -1 in case it cannot find
    int find(int x, int y)
    {
        h_table::const_iterator iterator = ht.find(edge(x,y));
        if(iterator == ht.end())
            return -1;
        return iterator->second;
    }



//    int& operator[](int index)
//    {
//        if (index >= size) {
//            cout << "Array index out of bound, exiting";
//            exit(0);
//        }
//        return ptr[index];
//    }

//    void change_element(uint index, uint x, uint y)
//    {
//        elements.insert(elements.begin() + index, edge(x, y));
//    }
//
//    void erase(uint32 index)
//    {
//        if (index >= elements.size())
//            throw logic_error("The index must be within elements range");
//
//        elements.erase(elements.begin() + index);
//
//        uint32 key = edge_to_uint64(elements[index].x, elements[index].y);
//        uint32 i = hash_function(key);
//
//        while (index != table[i])
//            i = (i + 1) % table.size();
//
//        table[i] = EMPTY;
//        i = (i + 1) % table.size();
//
//        while (table[i] != EMPTY)
//        {
//            index = table[i];
//            table[i] = EMPTY;
//            insert(index);
//            i = (i + 1) % table.size();
//        }
//
//        n_elements--;
//    }
//
//    vector<int> get_table()
//    {
//        return table;
//    }
//
    size_t size()
    {
        return ht.size();
    }
//
//    size_t n_edges()
//    {
//        return n_elements;
//    }
//
//    edge &operator[](size_t idx)
//    {
//        return elements[idx];
//    }
//
//    void clear()
//    {
//        table.assign(table.size(), EMPTY);
//        elements.clear();
//        n_elements = 0;
//    }
//
//    void resize(uint new_size)
//    {
//        table.resize(new_size);
//    }

/*    void print()
    {
        for (size_t i = 0; i < elements.size(); i++)
            std::cout << "x: " << elements[i].x << "   y: " << elements[i].y << std::endl;
        if (elements.size() == 0)
            std::cout << "empty" << std::endl;
    }*/

private:
    /* Resizes the current table with the double size of the previous one*/
//    void resize_table()
//    {
//        vector<int> new_table;
//
//        new_table.resize(table.size() * 2);
//        new_table.assign(new_table.size(), EMPTY);
//        n_elements = 0;
//
//        for (size_t i = 0; i < table.size(); i++)
//            if (table[i] != EMPTY)
//                insert(table[i], &new_table);
//        this->table = new_table;
//    }
//
//    /* concatenates the values of x and y into a 64 bit (32*2)*/
//    uint64 edge_to_uint64(int x, int y)
//    {
//        uint64 concat_edge = x;
//        concat_edge <<= 32;
//        concat_edge |= y;
//
//        return concat_edge;
//    }
//
//    /* Converts edge x,y (key) into a uint 32 */
//    uint32 hash_function(uint64 key)
//    {
//        key = (~key) + (key << 18);
//        key = key ^ (key >> 31);
//        key = (key + (key << 2)) + (key << 4);
//        key = key ^ (key >> 11);
//        key = key + (key << 6);
//        key = key ^ (key >> 22);
//
//        return (uint32)key % table.size();
//    }
//
//    vector<int> table;
//    vector<edge> elements;
//    size_t n_elements;


};

#endif