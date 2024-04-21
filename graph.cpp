#include "graph.h"

namespace graphalgo
{
    node::node(int n):d_n{n}, d_next_m{nullptr}, d_next_s{nullptr}
    {}

    int node::n() const { return d_n; }
    node& node::next_m() const { return *d_next_m; }
    node& node::next_s() const { return *d_next_s; }

    graph::graph(): d_n{0}, d_oriented{true}, d_tete{nullptr}
    {}

    graph::graph(unsigned int n, bool oriented): d_n{static_cast<int>(n)}, d_oriented{oriented}
    {
        d_tete = new node(1);
        node *crt = d_tete;
        for(int i = 1; i <= d_n; ++i)
        {
            crt->d_next_m = new node(i);
            crt = crt->d_next_m;
        }
    }

    graph::graph(const vector<int>& fs, const vector<int>& aps, bool oriented): d_n{aps[0]}, d_oriented{oriented}
    {
        d_tete = new node(1);
        node *crt = d_tete;
        for(int i = 1; i <= d_n; ++i)
        {
            crt->d_next_m = new node(i);
            crt = crt->d_next_m;
        }

        int s = 1;
        for(int i = 1; i <= fs[0]; ++i)
        {
            node* ct;
            if(fs[i] != 0 && !vertex(s, fs[i], ct))
                add_successor(s, fs[i]);
            else
                ++s;
        }
    }

    graph::graph(const vector<vector<int>>& mat_adj, bool oriented): d_n{mat_adj[0][0]}, d_oriented{oriented}
    {
        d_tete = new node(1);
        node *crt = d_tete;
        for(int i = 1; i <= d_n; ++i)
        {
            crt->d_next_m = new node(i);
            crt = crt->d_next_m;
        }

        for(int i = 1; i <= d_n; ++i)
        {
            for(int j = 1; j <= d_n; ++j)
            {
                node* ct;
                if(mat_adj[i][j] == 1 && !vertex(i, j, ct))
                    add_successor(i, j);
            }
        }
    }

    graph::~graph()
    {
        node* crt = d_tete;
        node* as;

        while(crt)
        {
            node* crt_ct = crt->d_next_s;
            // detroying all the cost of the current node
            while(crt_ct)
            {
                as = crt_ct;
                crt_ct = crt_ct->d_next_s;
                delete as;
            }

            // delete the node itself and move to the next node;
            as = crt;
            crt = crt->d_next_m;
            delete as;
        }
    }

    bool graph::oriented() const { return d_oriented; }

    void graph::oriented(bool oriented) { d_oriented = oriented; }

    node* graph::find(int s) const
    {
        if(!d_tete)
            return nullptr;
        else if(d_tete && d_tete->d_n == s)
            return d_tete;

        node* crt = d_tete;

        while(crt && crt->d_n != s)
            crt = crt->d_next_m;

        return crt;
    }

    void graph::add_node()
    {
        if(!d_tete)
            d_tete = new node(1);
        else {
            node* crt = d_tete;

            while(crt->d_next_m != nullptr)
                crt = crt->d_next_m;

            crt->d_next_m = new node(++d_n);
        }
    }

    void graph::add_successor(int s, int ss, int cost)
    {
        node* ns = find(s);
        node* nss = find(ss);

        // return if s or ss could not be found break execution
        if(!ns || !nss)
            return;

        // create the node with the cost @cost and connect it to @ss
        node* new_c = new node(cost);
        new_c->d_next_m = nss;

        // if the list of cost of the node is null then add the top else add to the bottom
        if(!ns->d_next_s)
            ns->d_next_s = new_c;
        else
        {
            node* crt_c = ns->d_next_s;
            while(crt_c->d_next_s)
                crt_c = crt_c->d_next_s;
            crt_c->d_next_s = new_c;
        }
    }

    node* graph::vertex(int s, int ss, node*& ct) const
    {
        node* ns = find(s);
        node* nss = find(ss);

        if(!ns || !nss)
            return nullptr;

        node* crt_vertex_1 = ns->d_next_s;
        node* crt_vertex_2 = nss->d_next_s;

        // d_n in crt_ct is the cost and d_next_m->d_n is the successor
        while((crt_vertex_1 && crt_vertex_2) &&
            (crt_vertex_1->d_next_m->d_n != ss && crt_vertex_2->d_next_m->d_n != s))
        {
            crt_vertex_1 = crt_vertex_1->d_next_s;
            crt_vertex_2 = crt_vertex_2->d_next_s;
        }

        if(crt_vertex_1 && crt_vertex_2)
        {
            if (crt_vertex_1->d_next_m->d_n == ss)
            {
                ct = crt_vertex_1;
                return ns;
            }
            else
            {
                ct = crt_vertex_2;
                return nss;
            }
        }
        else{
            if(crt_vertex_1)
            {
                while(crt_vertex_1 && crt_vertex_1->d_next_m->d_n != ss)
                    crt_vertex_1 = crt_vertex_1->d_next_s;

                if(crt_vertex_1 && crt_vertex_1->d_next_m->d_n == ss)
                {
                    ct = crt_vertex_1;
                    return ns;
                }
            }
            else
            {
                while(crt_vertex_2 && crt_vertex_2->d_next_m->d_n != s)
                    crt_vertex_2 = crt_vertex_2->d_next_s;

                if(crt_vertex_2 && crt_vertex_2->d_next_m->d_n == s)
                {
                    ct = crt_vertex_2;
                    return nss;
                }
            }
        }
        return nullptr;
    }

    int graph::cost(int s, int ss) const
    {
        node* crt_ct;

        if(vertex(s, ss, crt_ct))
            return crt_ct->d_n;
        else
            return -__INT_MAX__;
    }

    void graph::cost(int s, int ss, int cost)
    {
        node* crt_ct;

        if(vertex(s, ss, crt_ct))
            crt_ct->d_n = cost;
    }

    void graph::fs_aps(vector<int>& fs, vector<int>& aps)
    {
        if(!d_tete)
            return;

        //putting a default size in fs[0]
        fs.clear();
        fs.push_back(-1);
        aps.resize(d_n + 1);
        aps[0] = d_n;
        //current node id;
        int i = 1;

        node *crt = d_tete;
        while(crt)
        {
            // add the fs index of the current node
            aps[i] = fs.size();

            node* crt_ct = crt->d_next_s;
            // going to all the successors of the current node
            while(crt_ct)
            {
                // d_n in crt_ct is the cost and d_next_m->d_n is the successor
                fs.push_back(crt_ct->d_next_m->d_n);
                crt_ct = crt_ct->d_next_s;
            }
            fs.push_back(0);
            crt = crt->d_next_m;
            ++i;
        }
        fs[0] = fs.size() - 1;
    }

    void graph::mat_adj_not_oriented(vector<vector<int>>& mat_adj)
    {
        // nombre of arrete
        int m = 0;
        //current node id;
        int i = 1;

        node *crt = d_tete;
        while(crt)
        {
            node* crt_ct = crt->d_next_s;
            // going to all the successors of the current node
            while(crt_ct)
            {
                // d_n in crt_ct is the cost and d_next_m->d_n is the successor
                mat_adj[i][crt_ct->d_next_m->d_n] = 1;
                mat_adj[crt_ct->d_next_m->d_n][i] = 1;
                crt_ct = crt_ct->d_next_s;
                ++m;
            }
            crt = crt->d_next_m;
            ++i;
        }
        mat_adj[0][1] = m;
    }

    void graph::mat_adj_oriented(vector<vector<int>>& mat_adj)
    {
        // nombre of arrete
        int m = 0;
        //current node id;
        int i = 1;

        node *crt = d_tete;
        while(crt)
        {
            node* crt_ct = crt->d_next_s;
            // going to all the successors of the current node
            while(crt_ct)
            {
                // d_n in crt_ct is the cost and d_next_m->d_n is the successor
                mat_adj[i][crt_ct->d_next_m->d_n] = 1;
                crt_ct = crt_ct->d_next_s;
                ++m;
            }
            crt = crt->d_next_m;
            ++i;
        }
        mat_adj[0][1] = m;
    }

    void graph::mat_adj(vector<vector<int>>& mat_adj)
    {
        if(!d_tete)
            return;

        mat_adj.clear();
        mat_adj.resize(d_n + 1);

        mat_adj[0] = vector<int>(d_n + 1, -1);
        mat_adj[0][0] = d_n;

        // creating of each line
        for(int s = 1; s <= d_n; ++s)
            mat_adj[s] = vector<int>(d_n + 1, 0);

        if(d_oriented)
            mat_adj_oriented(mat_adj);
        else
            mat_adj_not_oriented(mat_adj);
    }

}
