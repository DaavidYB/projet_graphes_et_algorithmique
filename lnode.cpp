#include "lnode.h"

namespace graphalgo{
node::node(int n):d_n{n}, d_next_m{nullptr}, d_next_s{nullptr}
{}

lnode::lnode(): d_n{0}, d_tete{nullptr}
{}

lnode::lnode(unsigned int n): d_n{n}
{
    d_tete = new node(1);
    node *crt = d_tete;
    for(int i = 1; i <= d_n; ++i)
    {
        crt->d_next_m = new node(i);
        crt = crt->d_next_m;
    }
}

lnode::lnode(const vector<int>& fs, const vector<int>& aps): d_n{aps[0]}
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
        if(fs[i] != 0)
            add_successor(s, fs[i]);
        else
            ++s;
    }
}

lnode::lnode(const vector<vector<int>>& mat_adj): d_n{mat_adj[0][0]}
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
            if(mat_adj[i][j] == 1)
                add_successor(i, j);
    }
}

lnode::~lnode()
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

node* lnode::find(int s)
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

void lnode::add_node()
{
    if(!d_tete)
        d_tete = new node(1);
    else {
        node* crt = d_tete;

        while(crt->d_next_m != nullptr)
            crt = crt->d_next_m;

        crt->d_next_m = new node(crt->d_n + 1);
        ++d_n;
    }
}

void lnode::add_successor(int s, int ss, int cost)
{
    node* ns = find(s);
    node* nss = find(ss);

    // return if s or ss could not be found break execution
    if(!ns || !nss)
        return;

    // create the node with the cost @cost and connect it to @ss
    node new_c = new node(cost);
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

void lnode::fs_aps(vector<int>& fs, vector<int>& aps)
{
    if(!d_tete)
        return;

    //putting a default size in fs[0]
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

void lnode::mat_adj(vector<vector<int>>& mat_adj)
{
    if(!d_tete)
        return;

    mat_adj.resize(d_n + 1);

    mat_adj[0] = vector<int>(d_n + 1, -1);
    mat_adj[0][0] = d_n;
    // nombre of arrete
    int m = 0;
    //current node id;
    int i = 1;

    node *crt = d_tete;
    while(crt)
    {
        mat_adj[i] = vector<int>(d_n + 1, 0);
        node* crt_ct = crt->d_next_s;
        // going to all the successors of the current node
        while(crt_ct)
        {
            // d_n in crt_ct is the cost and d_next_m->d_n is the successor
            mat_adj[i][crt_ct->d_next_m->d_n] = 1;
            crt_ct = crt_ct->d_next_s;
        }
        crt = crt->d_next_m;
        ++i;
    }
}

}
