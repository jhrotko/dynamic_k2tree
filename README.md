# Dynamic k2tree

The Dynamic k2tree is a C++ Graph Library which implements the dynamic k2tree data structure, implemented with static 
bit vectors provided by the SDSL library (https://github.com/joo95h/sdsl-lite). Besides, we implemented based on the SDK implementation (https://github.com/aplf/sdk2tree). 

Our data structure presents compression ratios between 3-4 bits per link. Moreover, it allows query directly over the compressed data 
structure without having to uncompress it. 

## Documentation
Our data structure provides the following methods:

- ```add_edge(x,y)``` - Inserts the edge ```(x,y)``` in the graph.
- ```del_edge(x,y)``` - Deletes the edge ```(x,y)``` from the graph.
- ```contains(x,y)``` - Checks if edge ```(x,y)``` is present in the graph
- ```list_neighbors(x)``` - returns a ```vector<uint>``` with the neighborhood of node ```x```.
- ```serialize(filename)``` - Serializes the graph in folder ```filename```. It will create some auxiliary files to store the k2-tree in the graph which by default are saved in the current directory.
- ```load(filename)``` - Loads the graph from folder ```filename```. By default it will load the k2-trees from the currentdirectory. This directory should be the same where you decided to serialize. 
Besides, it cleans these files by the end of the process. You can set the clean flag to false in order to not delete the k2-tree serialized files.

It is also possible to iterate over the edges, nodes and neighbors of a node by evoking the following:

- ```edge_begin()``` - Returns the first edge iterator.
- ```edge_end()``` - Returns the past-end edge iterator.
- ```node_begin()``` - Returns the first node iterator.
- ```node_end()``` - Returns the past-end node iterator.
- ```neighbor_begin(x)``` - Returns the first neighbor iterator for node  ```x```.
- ```neighbor_end()``` - Returns the past-end neighbor iterator.

The iterators usage should be similar to the following example:
```cpp
dynamic_ktree::DKtree <2> graph(50); //initialize dynamic k2tree with k=2.

for(auto it = graph.edge_begin(); graph.edge_end(); ++it) {
    std::cout << it.x() << std::endl; 
}
```

We also offer a small range of Algorithms to operate over the graph.
- Breadth-first Search
- Depth-first Search
- Counting Triangles
- Clustering Coefficient
- PageRank

```cpp
dynamic_ktree::DKtree <2> graph;
graph.load("serialized_folder"); 
vector<uint> path = Algorithm<dynamic_ktree::DKtree<2>>::bfs(graph, 1); // start BFS at root node 1.
```
### Running tests
We also present some examples of how to work with our data structure in our tests. You can run the unit tests:
- ```make tests_dktree``` - Run the dynamic k2tree tests
- ```make tests_algorithm``` - Test the Algorithms

## Installation
1. First you will need to install gtest and cmake
1. Install the sdsl-lite fork (https://github.com/joo95h/sdsl-lite): \
 ```cd sdsl-lite```\
 ```sudo ./install.sh /usr/local/```


[Bibliography]: https://github.com/joo95h/dynamic_k2tree/wiki "Bibliography"
