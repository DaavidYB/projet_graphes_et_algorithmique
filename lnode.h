#ifndef LNODE_H
#define LNODE_H

#include <vector>
using std::vector;

namespace graphalgo{
    class node
    {
        public:
            node(int n);
        private:
            /**
             * @brief d_n: The value of the node or the cost
             */
            int d_n;
            /**
             * @brief d_next_m: The pointer to a node in the main list.
             * This is either a consecutive successor (1 -> 2)
             * or its successor in the graph (1 <---> 3).
             */
            node* d_next_m;
            /**
             * @brief d_next_s: The pointer to a node in the secondary list.
             * It's either a cost or the next cost of the current cost.
             */
            node* d_next_s;

        friend class lnode;
    };


    /**
     * @brief The lnode class a dynamique structur of graph node
     */
    class lnode
    {
        public:
            /**
             * @brief lnode default constructor
             */
            lnode();
            /**
             * @brief lnode construct a list of n node
             * @param n: the nulber of node to create
             * @pre n > 1
             */
            lnode(int n);
            /**
             * @brief lnode create a graph using fs & aps;
             * @param fs
             * @param aps
             */
            lnode(const vector<int>& fs, const vector<int>& aps);
            /**
             * @brief lnode create a graph using an adjacency matrix;
             * @param mat_adj: the adjacency matrix
             */
            lnode(const vector<vector<int>>& mat_adj);
            /**
             * @brief destructor;
             */
            ~lnode();

            // methods

            /**
             * @brief find: find a node
             * @param s: the node to find
             * @return the pointer to the node
             * @pre s > 1
             */
            node* find(int s);
            /**
             * @brief add_node; add a node to the graph
             */
            void add_node();
            /**
             * @brief add_successor: add a successor to a node
             * @param s: the node itself
             * @param ss: the node that should be connected to the node s
             * @param cost: the cost of node
             * @pre cost in [-INT_MAX, INT_MAX[
             * @pre s > 0
             * @pre ss > 0
             */
            void add_successor(int s, int ss, int cost = INT_MAX);
            /**
             * @brief fs_aps transform the graph to a queue of successors
             * @param fs
             * @param aps
             */
            void fs_aps(vector<int>& fs, vector<int>& aps);
            /**
             * @brief mat_adj transform the graph to an adjacency matrix
             * @param mat_adj
             */
            void mat_adj(vector<vector<int>>& mat_adj);
        private:
            // nb of node
            int d_n;
            // entry point if the list
            node* d_tete;
    };
}

#endif // LNODE_H
