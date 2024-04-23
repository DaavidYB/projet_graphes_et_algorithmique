//
// Created by Arthur Mathis on 22/04/2024.
//

#include "algorithmes.h"
#include "graph.h"

void empiler(int x, vector<int>& pilch) {
    pilch[x] = pilch[0];
    pilch[0] = x;
}

vector<int> distance(int sommet, const vector<int>& fs, const vector<int>& aps){
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

vector<int> rang(const vector<int>& fs, const vector<int>& aps) {
    // On déclare les variables tampons
    int nbSommets = aps[0], tailleFS = fs[0], sommetCourant, r, indiceSuccesseur, successeur;
    // On initialise le tableau de rangs
    vector<int> rang (nbSommets + 1, -1);

    // On initialise les tableaux tampons
    vector<int> ddi (nbSommets + 1, 0); // Degré entrant de chaque sommet
    vector<int> pilch (nbSommets + 1); // Pile des sommets sans prédécesseurs
    vector<int> prem (nbSommets + 1); // Tableau des premiers sommets de chaque rang

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

    // On retourne le vecteur de rangs calculé
    return rang;
}

vector<int> prufer(const vector<vector<int>>& a) {
    int nbSommets = a[0][0];
    // Création du vecteur pour stocker le codage de Prufer
    vector<int> prf (nbSommets - 1);
    vector<vector<int>> matriceAdj = a;

    prf[0] = nbSommets - 2;
    int k = 1;

    while(k <= nbSommets - 2) {
        // On cherche le premier sommet feuille
        int i = 1;
        for(; matriceAdj[i][0] != 1; i++);

        // On cherche le voisin du sommet feuille
        int j=1;
        for(; matriceAdj[i][j] != 1; j++);

        // On stocke du voisin dans le codage de Prufer
        prf[k++] = j;

        // On supprime le sommet feuille et ses arêtes
        matriceAdj[i][j] = 0;
        // On supprime  l'arête entre le sommet feuille et son voisin
        matriceAdj[j][i] = 0;
        matriceAdj[i][0] = 0; // On marque le sommet feuille comme supprimé
        matriceAdj[j][0]--;
    }
    return prf;
}


pair<vector<int>, vector<int>> Dijkstra(int s, const vector<int>& fs, const vector<int>& aps, const vector<vector<int>>& C) {
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
}

void fusion(int s, int t, vector<int>& prem, vector<int>& pilch, vector<int>& cfc) {
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

void kruskal(const graphalgo::graph& G, graphalgo::graph& T) {
    int nbSommets = G.n();
    // Initialiser T avec le même nombre de noeuds et orientation que G
    T = graphalgo::graph(nbSommets, G.oriented());

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

    // On trie les arêtes par poids croissant
    // sort(aretes.begin(), aretes.end());

    int k = 0;
    for(const auto& a : aretes) {
        // On récupère les extrémités de l'arête
        // int s = a.second.first;
        // int t = a.second.second;
        // if(cfc[s] != cfc[t]) {
        //     // Ajouter le successeur si on n'a pas de circuit (même CFC)
        //     T.add_successor(s, t, a.first);
        //     fusion(s, t, prem, pilch, cfc);
        //     k++;
        //tests◊
        //     // On s'arrête quand l'arbre couvrant est complet
        //     if(k == nbSommets - 1) break;
        // }
    }
}
