#include "doctest.h"
#include "../graph.h"
#include <vector>

using std::vector;

void compare(const vector<int>& arr1, const vector<int>& arr2)
{
    REQUIRE_EQ(arr1.size(), arr2.size());
    for(int i = 0; i < arr2.size(); ++i)
        REQUIRE_EQ(arr1.size(), arr2.size());
}

TEST_CASE("[graph]") {
    vector<int> g_fs = {31, 2, 3, 0, 4, 9, 0, 4, 6, 8, 0, 8, 9, 11, 0, 3, 6, 0, 7, 8, 0, 8, 0, 10, 0, 0, 11, 0, 10, 12, 0, 0};
    vector<int> g_aps = {12, 1, 4, 7, 11, 15, 18, 21, 23, 25, 26, 28, 31};
    SUBCASE("Constructeur par défaut") {
        graphalgo::graph g{};

        REQUIRE_EQ(g.n(), 0);
    }

    SUBCASE("Constructeur avec le  nombre de sommets et type de graphe") {
        graphalgo::graph g{2, true};

        vector<int> fs, aps;
        REQUIRE_EQ(g.n(), 2);
        REQUIRE_EQ(g.oriented(), true);
    }

    SUBCASE("Methodes") {
        graphalgo::graph g{5, false};

        SUBCASE("graph::find(s)") {
            REQUIRE_EQ(g.find(1)->n(), 1);
            REQUIRE_EQ(g.find(3)->n(), 3);
            REQUIRE_EQ(g.find(6), nullptr);
        }

        SUBCASE("graph::add_node()") {
            g.add_node();
            REQUIRE_EQ(g.n(), 6);
            REQUIRE_EQ(g.find(6)->n(), 6);
        }

        SUBCASE("graph::add_successor(s, ss, cost)") {
            g.add_successor(2, 7, 0);
            graphalgo::node* nd = g.find(2);
            REQUIRE_EQ(&nd->next_s(), nullptr);

            g.add_successor(2, 3, 0);
            REQUIRE_EQ(nd->next_s().next_m().n(), 3);
            REQUIRE_EQ(nd->next_s().n(), 0);
        }

        SUBCASE("graph::vertex(s, ss, *& ct)") {
            graphalgo::node* ct;
            graphalgo::node* s;

            g.add_successor(2, 3, 0);
            s = g.vertex(2, 3, ct);
            REQUIRE_EQ(s->n(), 2);
            REQUIRE_EQ(ct->n(), 0);
            REQUIRE_EQ(ct->next_m().n(), 3);

            s = g.vertex(3, 2, ct);
            REQUIRE_EQ(s->n(), 2);
            REQUIRE_EQ(ct->n(), 0);
            REQUIRE_EQ(ct->next_m().n(), 3);

            REQUIRE_EQ(g.vertex(2, 4, ct), nullptr);
        }

        SUBCASE("graph::cost(s, ss)") {
            g.add_successor(2, 3, 5);
            REQUIRE_EQ(g.cost(2, 3), 5);
            REQUIRE_EQ(g.cost(2, 4), -__INT_MAX__);
        }

        SUBCASE("graph::cost(s, ss, cost)") {
            g.add_successor(2, 3, 5);
            g.cost(2, 3, 7);
            REQUIRE_EQ(g.cost(2, 3), 7);
        }
        
        SUBCASE("graph::fs_aps(&fs, &aps") {
            graphalgo::graph g2{12};
            vector<int> t_fs = {13, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
            vector<int> t_aps = {12, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
            vector<int> fs, aps;

            g2.fs_aps(fs, aps);
            compare(t_fs, fs);
            compare(t_aps, aps);

            g2.add_successor(1, 2, 0);
            g2.add_successor(1, 3, 0);
            g2.add_successor(2, 4, 0);
            g2.add_successor(2, 9, 0);
            g2.add_successor(3, 4, 0);
            g2.add_successor(3, 6, 0);
            g2.add_successor(3, 8, 0);
            g2.add_successor(4, 8, 0);
            g2.add_successor(4, 9, 0);
            g2.add_successor(4, 11, 0);
            g2.add_successor(5, 3, 0);
            g2.add_successor(5, 6, 0);
            g2.add_successor(6, 7, 0);
            g2.add_successor(6, 8, 0);
            g2.add_successor(7, 8, 0);
            g2.add_successor(8, 10, 0);
            g2.add_successor(10, 11, 0);
            g2.add_successor(11, 10, 0);
            g2.add_successor(11, 12, 0);

            t_fs.clear();
            t_aps.clear();

            g2.fs_aps(fs, aps);
            compare(g_fs, fs);
            compare(g_aps, aps);
        }
    }

    // SUBCASE("Constructeur avec fs aps") {
    //     vector<int> t_fs = {31, 2, 3, 0, 4, 9, 0, 4, 6, 8, 0, 8, 9, 11, 0, 3, 6, 0, 7, 8, 0, 8, 0, 10, 0, 0, 11, 0, 10, 12, 0, 0};
    //     vector<int> t_aps = {12, 1, 4, 7, 11, 15, 18, 21, 23, 25, 26, 28, 31};

    //     graphalgo::graph g{t_fs, t_aps, true};

    //     vector<int> fs, aps;
    //     REQUIRE_EQ(g.n(), 12);
    //     REQUIRE_EQ(g.oriented(), true);
    //     //g.fs_aps(fs, aps);

    //     // REQUIRE_EQ(t_fs.size(), fs.size());
    //     // for(int i = 0; i < fs.size(); ++i)
    //     //     REQUIRE_EQ(t_fs.size(), fs.size());

    //     // REQUIRE_EQ(t_aps.size(), aps.size());
    //     // for(int i = 0; i < aps.size(); ++i)
    //     //     REQUIRE_EQ(t_aps.size(), aps.size());
    // }
}
