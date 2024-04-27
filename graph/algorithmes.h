//
// Created by Arthur Mathis on 22/04/2024.
//

#ifndef PROJETGRAPHE_ALGORITHMES_H
#define PROJETGRAPHE_ALGORITHMES_H

#include "graph.h"
#include <vector>

using std::vector, std::pair;

namespace graphalgo{
    const int MAXPOIDS = __INT_MAX__;

    /**
     * @brief Fonction permettant d'empiler un sommet dans une pile chainée
     * @param x - Le sommet à empiler
     * @param pilch - La pile chainée
     */
    void empiler (int x, vector<int>& pilch);
    /**
     * @brief Fonction permettant de dépiler un sommet dans une pile chainée
     * @param pilch - La pile chainée
     * @return La valeur dépilée
     */
    int depiler(vector<int>& pilch);
    void fusion(int s, int t, vector<int>& prem, vector<int>& pilch, vector<int>& cfc);

    /**
     * @brief Fonction retournant le tableau de distances
     * @param sommet Le sommet de départ
     * @param fs
     * @param aps
     * @return Le tableau résultat contenant les distances
     */
    vector<int> distance(int sommet, const vector<int>& fs, const vector<int>& aps);
    /**
     * @brief Fonction retournant le tableau des rangs des sommets d'un graphe orienté.
     * @param fs Le tableau des successeurs de chaque sommet.
     * @param aps Le tableau des adresses des premiers successeurs de chaque sommet.
     * @return Le tableau des rangs des sommets.
     */
    vector<int> rang(const vector<int>& fs, const vector<int>& aps);
    void tarjan();
    void ordonnancement();
    /**
     * @brief Fonction appliquant l'algorithme de Dijsktra.
     * @param s Le sommet source à partir duquel calculer les plus courts chemins.
     * @param fs Le tableau des successeurs de chaque sommet.
     * @param aps Le tableau des adresses des premiers successeurs de chaque sommet.
     * @param C La matrice des poids des arêtes.
     * @return Une paire de vecteurs contenant les prédécesseurs et les distances minimales de chaque sommet par rapport au sommet source.
     */
    // pair<vector<int>, vector<int>> dijsktra(int s, const vector<int>& fs, const vector<int>& aps, const vector<vector<int>>& C);
    graphalgo::graph dijkstra(int s, graphalgo::graph &g);
    /**
     * @brief Fonction appliquant l'algorithme de dantzig.
     * @param matriceAdj La matrice d'adjacence
     * @param matriceCout La matrice des coûts associée
     */
    vector<vector<int>> dantzig(const vector<vector<int>> &matriceAdj, const vector<vector<int>> &matriceCout);
    graphalgo::graph kruskal(const graphalgo::graph& G);
    /**
     * @brief Fonction retournant le codage de Prufer d'un arbre donné par sa matrice d'adjacence.
     * @param a La matrice d'adjacence représentant l'arbre.
     * @return Le codage de Prufer de l'arbre.
     */
    vector<int> prufer(vector<vector<int>> a);
    // décodage prufer à faire ? vector<vector<int>> decodagePrufer(const vector<int>& prufer)
}

#endif //PROJETGRAPHE_ALGORITHMES_H
