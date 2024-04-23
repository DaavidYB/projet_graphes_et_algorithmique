#include "doctest.h"
#include "../graph.h"
#include <vector>

using std::vector;

void compare(const vector<int>& arr1, const vector<int>& arr2)
{
    REQUIRE_EQ(arr1.size(), arr2.size());
    for(int i = 0; i < arr2.size(); ++i)
        REQUIRE_EQ(arr1[i], arr2[i]);
}

TEST_CASE("[graph]") {
    vector<int> FS = {31, 2, 3, 0, 4, 9, 0, 4, 6, 8, 0, 8, 9, 11, 0, 3, 6, 0, 7, 8, 0, 8, 0, 10, 0, 0, 11, 0, 10, 12, 0, 0};
    vector<int> APS = {12, 1, 4, 7, 11, 15, 18, 21, 23, 25, 26, 28, 31};
    graphalgo::graph G{};
    graphalgo::graph G1{5};
    SUBCASE("Constructeur par défaut") {
        REQUIRE_EQ(G.n(), 0);
    }

    SUBCASE("Constructeur avec le  nombre de sommets et type de graphe") {
        REQUIRE_EQ(G1.n(), 5);
        REQUIRE_EQ(G1.oriented(), true);
    }

    SUBCASE("Methodes") {
        SUBCASE("graph::find(s)") {
            REQUIRE_EQ(G1.find(1)->n(), 1);
            REQUIRE_EQ(G1.find(3)->n(), 3);
            REQUIRE_EQ(G1.find(6), nullptr);
        }

        SUBCASE("graph::add_node()") {
            REQUIRE_EQ(G.n(), 0);
            G.add_node();
            REQUIRE_EQ(G.n(), 1);
            REQUIRE_EQ(G.find(1)->n(), 1);
            G.add_node();
            REQUIRE_EQ(G.n(), 2);
            REQUIRE_EQ(G.find(2)->n(), 2);
        }

        SUBCASE("graph::add_successor(s, ss, cost)") {
            G1.add_successor(2, 7, 0);
            graphalgo::node* nd = G1.find(2);
            REQUIRE_EQ(&nd->next_s(), nullptr);

            G1.add_successor(2, 3, 0);
            REQUIRE_EQ(nd->next_s().next_m().n(), 3);
            REQUIRE_EQ(nd->next_s().n(), 0);

            G1.add_successor(2, 4, 5);
            REQUIRE_EQ(nd->next_s().next_s().n(), 5);
            REQUIRE_EQ(nd->next_s().next_s().next_m().n(), 4);

            G1.add_successor(2, 3, 6);
            REQUIRE_EQ(nd->next_s().next_m().n(), 3);
            REQUIRE_EQ(nd->next_s().n(), 6);

            REQUIRE_EQ(nd->next_s().next_s().n(), 5);
            REQUIRE_EQ(nd->next_s().next_s().next_m().n(), 4);
        }

        SUBCASE("graph::vertex(s, ss, *& ct)") {
            graphalgo::node* ct;
            graphalgo::node* s;

            G1.add_successor(2, 3, 0);
            s = G1.vertex(2, 3, ct);
            REQUIRE_EQ(s->n(), 2);
            REQUIRE_EQ(ct->n(), 0);
            REQUIRE_EQ(ct->next_m().n(), 3);

            s = G1.vertex(3, 2, ct);
            REQUIRE_EQ(s->n(), 2);
            REQUIRE_EQ(ct->n(), 0);
            REQUIRE_EQ(ct->next_m().n(), 3);

            REQUIRE_EQ(G1.vertex(2, 4, ct), nullptr);
        }

        SUBCASE("graph::cost(s, ss)") {
            G1.add_successor(2, 3, 5);
            REQUIRE_EQ(G1.cost(2, 3), 5);
            REQUIRE_EQ(G1.cost(2, 4), -__INT_MAX__);
        }

        SUBCASE("graph::cost(s, ss, cost)") {
            G1.add_successor(2, 3, 5);
            G1.cost(2, 3, 7);
            REQUIRE_EQ(G1.cost(2, 3), 7);
        }
        
        SUBCASE("graph::fs_aps(&fs, &aps") {
            graphalgo::graph g{12};
            vector<int> t_fs = {12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
            vector<int> t_aps = {12, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
            vector<int> fs, aps;

            g.fs_aps(fs, aps);
            compare(t_fs, fs);
            compare(t_aps, aps);

            g.add_successor(1, 2);
            g.add_successor(1, 3);
            g.add_successor(2, 4);
            g.add_successor(2, 9);
            g.add_successor(3, 4);
            g.add_successor(3, 6);
            g.add_successor(3, 8);
            g.add_successor(4, 8);
            g.add_successor(4, 9);
            g.add_successor(4, 11);
            g.add_successor(5, 3);
            g.add_successor(5, 6);
            g.add_successor(6, 7);
            g.add_successor(6, 8);
            g.add_successor(7, 8);
            g.add_successor(8, 10);
            g.add_successor(10, 11);
            g.add_successor(11, 10);
            g.add_successor(11, 12);

            t_fs.clear();
            t_aps.clear();

            g.fs_aps(fs, aps);
            compare(FS, fs);
            compare(APS, aps);
        }
    }

    // SUBCASE("Constructeur avec fs aps") {
    //     vector<int> t_fs = {31, 2, 3, 0, 4, 9, 0, 4, 6, 8, 0, 8, 9, 11, 0, 3, 6, 0, 7, 8, 0, 8, 0, 10, 0, 0, 11, 0, 10, 12, 0, 0};
    //     vector<int> t_aps = {12, 1, 4, 7, 11, 15, 18, 21, 23, 25, 26, 28, 31};

    //     graphalgo::graph g{t_fs, t_aps, true};

    //     vector<int> fs, aps;
    //     REQUIRE_EQ(G1.n(), 12);
    //     REQUIRE_EQ(G1.oriented(), true);
    //     //G1.fs_aps(fs, aps);

    //     // REQUIRE_EQ(t_fs.size(), fs.size());
    //     // for(int i = 0; i < fs.size(); ++i)
    //     //     REQUIRE_EQ(t_fs.size(), fs.size());

    //     // REQUIRE_EQ(t_aps.size(), aps.size());
    //     // for(int i = 0; i < aps.size(); ++i)
    //     //     REQUIRE_EQ(t_aps.size(), aps.size());
    // }
}
