//
// Created by Arthur Mathis on 22/04/2024.
//

#include "algorithmes.h"
#include "graph.h"

void graphalgo::empiler(int x, vector<int>& pilch) {
    pilch[x] = pilch[0];
    pilch[0] = x;
}

vector<int> graphalgo::distance(int sommet, const vector<int>& fs, const vector<int>& aps){
    // On déclare une variable contenant le nombre de sommets du graph
    int nbSommets = aps[0];
    // Déclaration des variables de début et fin du bloc en cours de traitement
    int debutBloc = 0, finBloc = 1;
    int d = 0, finFile, sommetCourant, successeur, indiceSuccesseur;

    // Déclaration d'un tableau contenant la file d'attente
    vector<int>file (nbSommets + 1);
    file[0] = nbSommets;
    file[1] = sommet;

    // On alloue le tableau de distance
    vector<int>distance (nbSommets + 1, -1);
    distance[0] = nbSommets;
    distance[sommet] = 0;

    // On déroule la file d'atente
    while(debutBloc < finBloc){
        d++;
        finFile = finBloc;
        // On parcourt le bloc courant

        while(debutBloc < finFile){
            // On décale le début du bloc
            debutBloc++;

            // On récupère le prochain successeur
            sommetCourant = file[debutBloc];
            indiceSuccesseur = aps[sommetCourant];
            successeur = fs[indiceSuccesseur];

            // Tant qu'il reste des successeurs
            while(successeur > 0){
                // Si le successeur courant n'est pas encore marqué
                if(distance[successeur] == -1){
                    // On décale la fin du bloc de travail
                    finBloc++;
                    // On place dans le blco suivant et on lui affecte sa distance
                    file[finBloc] = successeur;
                    distance[successeur] = d;
                }
                successeur = fs[++indiceSuccesseur];
            }
        }
    }
    return distance;
}

vector<int> graphalgo::rang(const vector<int>& fs, const vector<int>& aps) {
    // On déclare les variables tampons
    int nbSommets = aps[0], tailleFS = fs[0], sommetCourant, r, indiceSuccesseur, successeur;
    // On initialise le tableau de rangs
    vector<int> rang (nbSommets + 1, -1);

    // On initialise les tableaux tampons
    vector<int> ddi (nbSommets + 1, 0); // Degré entrant de chaque sommet
    vector<int> pilch (nbSommets + 1); // Pile des sommets sans prédécesseurs
    vector<int> prem (nbSommets + 1); // Tableau des premiers sommets de chaque rang

    // AJOUT
    for(int i = 0; i < tailleFS; i++) {
        int sommet = fs[i];
        if(sommet > 0)
            ddi[sommet]++;
    }

    // On calcule le rang
    pilch[0] = 0;
    for(sommetCourant = 1; sommetCourant <= nbSommets; sommetCourant++)
        if(ddi[sommetCourant] == 0)
            empiler(sommetCourant, pilch);

    r = -1;
    sommetCourant = pilch[0];
    prem[0] = sommetCourant;

    while(pilch[0] > 0) {
        // On incrémente le rang courant
        r++;
        // On réinitialise la pile des sommets sans prédécesseurs
        pilch[0] = 0;

        // On parcourt les sommets sans prédécesseurs dans la pile pilch
        while(sommetCourant > 0) {
            // On affecte le rang au sommet courant
            rang[sommetCourant] = r;

            // On récupère le premier successeur du sommet courant
            indiceSuccesseur = aps[sommetCourant];
            successeur = fs[indiceSuccesseur];

            // On parcourt les successeurs du sommet courant
            while(successeur > 0) {
                // On décrémente le degré entrant du successeur
                ddi[successeur]--;
                // On empile le successeur s'il n'a plus de prédécesseurs
                if(ddi[successeur] == 0)
                    empiler(successeur,pilch);

                // On passe au successeur suivant
                indiceSuccesseur++;
                successeur = fs[indiceSuccesseur];
            }

            // On passe au sommet suivant sans prédécesseurs
            sommetCourant = pilch[sommetCourant];
        }

        // On met-à-jour le sommet de début du prochain rang
        sommetCourant = pilch[0];
        prem[r + 1] = sommetCourant;
    }
    rang[0] = aps[0];
    // On retourne le vecteur de rangs calculé
    return rang;
}

vector<int> graphalgo::prufer(vector<vector<int>> a) {
    int nbSommets = a[0][0];
    // Création du vecteur pour stocker le codage de Prufer
    vector<int> prf (nbSommets - 1);

    prf[0] = nbSommets - 2;
    int k = 1;

    while(k <= nbSommets - 2) {
        // On cherche le premier sommet feuille
        int i = 1;
        for(; a[i][0] != 1; i++);

        // On cherche le voisin du sommet feuille
        int j=1;
        for(; a[i][j] != 1; j++);

        // On stocke du voisin dans le codage de Prufer
        prf[k++] = j;

        // On supprime le sommet feuille et ses arêtes
        a[i][j] = 0;
        // On supprime  l'arête entre le sommet feuille et son voisin
        a[j][i] = 0;
        a[i][0] = 0; // On marque le sommet feuille comme supprimé
        a[j][0]--;
    }
    return prf;
}

/*pair<vector<int>, vector<int>> graphalgo::dijsktra(int s, const vector<int>& fs, const vector<int>& aps, const vector<vector<int>>& C) {
    int v, j, minDist;
    int nbSommet = aps[0];
    // Vecteurs pour stocker les distances minimales et les prédécesseurs
    vector<int> d(nbSommet + 1);
    vector<int> pred(nbSommet + 1, 0);
    // Vecteur pour marquer les sommets déjà traités
    vector<bool> S(nbSommet + 1, true);
    d[0] = nbSommet;

    // Initialisation des distances
    for(int i = 1; i <= nbSommet; i++)
        d[i] = C[s][i];

    // Marquer le sommet source comme traité et sa distance à lui-même égale à 0
    S[s] = false;
    d[s] = 0;

    // Boucle principale
    for(int cpt = 0; cpt < nbSommet; cpt++) {
        // Initialisation de la distance minimale à MAXPOIDS
        minDist = graphalgo::MAXPOIDS;

        // Recherche du sommet non traité avec la plus petite distance actuelle
        for(int i = 1; i <= nbSommet; i++) {
            if(S[i] && d[i] < minDist) {
                minDist = d[i];
                j = i;
            }
        }

        // Si la distance minimale est toujours MAXPOIDS, on arrête le traitement
        if(d[j] != graphalgo::MAXPOIDS) {
            // Marquer le sommet sélectionné comme traité
            S[j] = false;

            // Mise à jour des distances des voisins du sommet sélectionné
            for(int l = aps[j]; fs[l] != 0; l++) {
                int k = fs[l];
                if(S[k]) {
                    v = d[j] + C[j][k]; // Calcul du nouveau poids potentiel
                    if(v < d[k]) {
                        d[k] = v; // Mise à jour de la distance minimale
                        pred[k] = j; // Mise à jour du prédécesseur
                    }
                }
            }
        } else {
            break; // Si la distance minimale est toujours infinie, on arrête le traitement
        }
    }

    // On retourne les vecteurs de prédécesseurs et de distances minimales
    return make_pair(pred, d);
}*/

graphalgo::graph graphalgo::dijkstra(int s, graphalgo::graph &g) {
    // Initialiser T avec le même nombre de noeuds et orientation que G
    graphalgo::graph T = graphalgo::graph(g.n(), g.oriented());
    // On récupère fs et aps
    std::vector<int> fs, aps;
    g.fs_aps(fs, aps);
    // On récupère la matrice des coûts
    std::vector<vector<int>> matCout = g.cost_matrice();

    int v, j, minDist;
    int nbSommet = aps[0];
    // Vecteurs pour stocker les distances minimales et les prédécesseurs
    vector<int> d(nbSommet + 1);
    d[0] = nbSommet;
    vector<int> pred(nbSommet + 1, 0);
    // Vecteur pour marquer les sommets déjà traités
    vector<bool> S(nbSommet + 1, true);

    // Initialisation des distances
    for(int i = 0; i < nbSommet; i++)
        d[i+1] = matCout[s-1][i];

    // Marquer le sommet source comme traité et sa distance à lui-même égale à 0
    S[s] = false;
    d[s] = 0;

    // Boucle principale
    for(int cpt = 0; cpt < nbSommet; cpt++) {
        // Initialisation de la distance minimale à MAXPOIDS
        minDist = graphalgo::MAXPOIDS;

        // Recherche du sommet non traité avec la plus petite distance actuelle
        for(int i = 1; i <= nbSommet; i++) {
            if(S[i] && d[i] < minDist) {
                minDist = d[i];
                j = i;
            }
        }

        // Si la distance minimale est toujours MAXPOIDS, on arrête le traitement
        if(d[j] != graphalgo::MAXPOIDS) {
            // Marquer le sommet sélectionné comme traité
            S[j] = false;

            // Mise à jour des distances des voisins du sommet sélectionné
            for(int l = aps[j]; fs[l] != 0; l++) {
                int k = fs[l];
                if(S[k]) {
                    v = d[j] + matCout[j-1][k-1]; // Calcul du nouveau poids potentiel
                    if(v < d[k]) {
                        d[k] = v; // Mise à jour de la distance minimale
                        pred[k] = j; // Mise à jour du prédécesseur
                    }
                }
            }
        } else {
            break; // Si la distance minimale est toujours infinie, on arrête le traitement
        }
    }

    // Construire le graphe T à partir des prédécesseurs
    for (int i = 1; i <= nbSommet; i++) {
        if(pred[i] != 0) {
            T.add_successor(pred[i], i, d[i]);
        }
    }

    return T;
}

vector<vector<int>> graphalgo::dantzig(const vector<vector<int>> &matriceAdj, const vector<vector<int>> &matriceCout)
{
    int i, j, k;
    // Récupère le nombre de nœuds du graphe
    int n = matriceAdj[0][0];
    vector<vector<int>> tabDantzig(n, vector<int>(n));

    // Initialisation de la matrice des coûts
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            if (matriceAdj[i][j+1] != 0)
                tabDantzig[i][j] = matriceCout[i][j+1];
            else
                // Valeur arbitrairement grande pour représenter l'infini
                tabDantzig[i][j] = __INT_MAX__;
        }
        // Valeur spéciale pour indiquer la diagonale de la matrice
        tabDantzig[i][i] = -__INT_MAX__;
    }

    // On représente la distance minimal afin de verifier la distance minimal
    int t = tabDantzig[2][1] + tabDantzig[1][2];
    for (k = 1; k < n - 1; k++)
    {
        for (i = 0; i <= k; i++)
        {
            for (j = 0; j <= k; j++)
            {
                int temp = tabDantzig[i][j] + tabDantzig[j][k+1];
                if(tabDantzig[i][k+1] > temp && i != j && j != k+1)
                    tabDantzig[i][k+1] = temp;

                temp = tabDantzig[k+1][j] + tabDantzig[j][i];
                if (tabDantzig[k+1][i] > temp && i != j && j != k+1)
                    tabDantzig[k+1][i] = temp;
            }
            int temp = tabDantzig[k+1][j] + tabDantzig[j][k+1];
            if (t > temp && j != k+1)
                t = temp;

            // S'il y a un circuit absorbant, retourne un message d'erreur
            if (t < 0)
                return {};

        }
        for (i = 0; i <= k; i++)
        {
            for (j = 0; j <= k; j++)
            {
                int temp = tabDantzig[i][k+1] + tabDantzig[k+1][j];
                if (tabDantzig[i][j] > temp && k+1 != j && j != k+1)
                    tabDantzig[i][j] = temp;
            }
        }
    }
    return tabDantzig;
}

void graphalgo::fusion(int s, int t, vector<int>& prem, vector<int>& pilch, vector<int>& cfc) {
    int cs = cfc[s];
    int ct = cfc[t];
    int x = prem[ct];
    while(x != 0) {
        cfc[x] = cs;
        x = pilch[x];
    }
    x = prem[cs];
    while(pilch[x] != 0) {
        x = pilch[x];
    }
    pilch[x] = prem[ct];
}

graphalgo::graph graphalgo::kruskal(const graphalgo::graph& G) {
    int nbSommets = G.n();
    // Initialiser T avec le même nombre de noeuds et orientation que G
    graphalgo::graph T = graphalgo::graph(nbSommets, G.oriented());

    // On initialise les tableaux tampons
    vector<int> prem(nbSommets + 1);
    vector<int> pilch(nbSommets + 1);
    vector<int> cfc(nbSommets + 1);

    // Initialisation des tableaux tampons
    for (int i = 1; i <= nbSommets; i++) {
        prem[i] = i;
        pilch[i] = 0;
        cfc[i] = i;
    }
    pilch[0] = 0;

    // Stockage des arêtes avec leur poids
    vector<graphalgo::vtx> aretes = G.vertexes();

    int k = 0;
    for(const auto& a : aretes) {
        // On récupère les extrémités de l'arête
        int s = a.s;
        int t = a.t;
        if(cfc[s] != cfc[t]) {
            // Ajouter le successeur si on n'a pas de circuit (même CFC)
            T.add_successor(s, t, a.p);
            fusion(s, t, prem, pilch, cfc);
            k++;
            // On s'arrête quand l'arbre couvrant est complet
            if(k == nbSommets - 1) break;
        }
    }

    return T;
}
