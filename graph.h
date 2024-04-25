#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <utility>
using std::vector;
#include <iostream>

namespace graphalgo
{
    struct vtx
    {
        int s,t;
        int p;
    };

    class node
    {
        public:
            node(int n);

            //getters
            int n() const;
            node& next_m() const;
            node& next_s() const;
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

        friend class graph;
    };

    /**
     * @brief The graph class a dynamique structur of graph node
     */
    class graph
    {
        public:
            /**
             * @brief graph default constructor
             */
            graph();
            /**
             * @brief construct a list of n node
             * @param n: the number of node to create
             * @param oriented: the graph is oriented or not
             * @pre n > 1
             */
            graph(unsigned int n, bool oriented = true);
            /**
             * @brief graph create a graph using fs & aps;
             * @param fs
             * @param aps
             */
            graph(const vector<int>& fs, const vector<int>& aps);
            /**
             * @brief create a graph using an adjacency matrix;
             * @param mat_adj: the adjacency matrix
             */
            graph(const vector<vector<int>>& mat_adj, bool oriented = true);
            /**
             * @brief destructor;
             */
            ~graph();

            // Getteurs
            /**
             * @brief Méthode publique retournant le contenu de d_n
             * @return
             */
            int n() const;

            // methods

            bool oriented() const;
            void oriented(bool oriented);

            /**
             * @brief find a node
             * @param s: the node to find
             * @return the pointer to the node
             * @pre s > 1
             */
            node* find(int s) const;
            /**
             * @brief add_node; add a node to the graph
             */
            void add_node();
            /**
             * @brief add a successor to a node
             * @param s: the node itself
             * @param ss: the node that should be connected to the node s
             * @param cost: the cost of node
             * @pre cost in ]-INT_MAX, INT_MAX[
             * @pre s > 0
             * @pre ss > 0
             */
            void add_successor(int s, int ss, int cost = __INT_MAX__);
            /**
             * @brief checks if there is a vertex between two node of a not oriented graph
             * @param s
             * @param ss
             * @param ct: a pointer to the cost of the vertex
             * @return the vertex head if there is, nullptr if there isn't
             */
            node* vertex(int s, int ss, node*& ct) const;
            /**
             * @brief checks if there is a vertex between two node of a oriented graph
             * @param s
             * @param ss
             * @return the cost of the vertex if there is, nullptr if there isn't
             */
            node* vertex(int s, int ss) const;
            /**
             * @brief get the cost of a vertex
             * @param s
             * @param ss
             * @return INT_MAX if there a cost was not set
             */
            int cost(int s, int ss) const;
            /**
             * @brief set the cost of a vertex
             * @param s
             * @param ss
             * @param cost
             */
            void cost(int s, int ss, int cost);
            /**
             * @brief transform the graph to a queue of successors
             * @param fs
             * @param aps
             */
            void fs_aps(vector<int>& fs, vector<int>& aps);
            /**
             * @brief retrun the adjacency matrix of the graph
             * @retrun vector<vector<int>>
             */
            vector<vector<int>> mat_adj();

            /**
             * @brief get all the vertex of a graph
             * @return a vector of vt
             */
            vector<vtx> vertexes() const;

            vector<vector<int>> cost_matrice() const;
            void save(std::ostream& ost) const;

        private:
            // nb of node
            int d_n;
            // the graph is oriented or not
            bool d_oriented;
            // entry point if the list
            node* d_tete;

            // @brief create an adjacency matrix for a not oriented graph
            void mat_adj_not_oriented(vector<vector<int>>& mat_adj);
            // @brief create an adjacency matrix for an oriented graph
            void mat_adj_oriented(vector<vector<int>>& mat_adj);

    };
}

#endif // GRAPH_H
