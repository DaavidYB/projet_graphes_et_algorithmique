//
// Created by Arthur Mathis on 22/04/2024.
//

#include "algorithmes.h"
#include "graph.h"

void graphalgo::empiler(int x, vector<int>& pilch) {
    pilch[x] = pilch[0];
    pilch[0] = x;
}

int graphalgo::depiler(vector<int>& pilch) {
    int x = pilch[0];
    pilch[0] = pilch[x];
    return x;
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

inline vector<int> retireIndex(const std::vector<int> &tab, int index)
{
    index++;
    vector<int> res;
    for(unsigned i = 0; i < tab.size(); i++){
        if(tab[i] != index)
            res.push_back(tab[i]);
    }
    return res;
}

graphalgo::graph graphalgo::dijkstra(int s_depart, graphalgo::graph &g)
{
    const int nb_sommets = g.n();

    // On récupère fs-aps et la matrice des coûts
    std::vector<int> fs, aps;
    g.fs_aps(fs, aps);
    std::vector<vector<int>> matCout = g.cost_matrice();

    // On déclare et initialise les variables tampons
    vector<int> d(nb_sommets, graphalgo::MAXPOIDS), pred(nb_sommets, -1);
    d[s_depart - 1] = 0;
    pred[s_depart - 1] = 0;

    // On déclare un tableau indiquant les sommets non-parcourus
    vector<int> sommets_non_traites(nb_sommets);
    for(int i = 0; i < nb_sommets; i++)
        sommets_non_traites[i] = i + 1;


    // On traite le sommet dont la distance est la plus petite
    for(int i = 0; i < nb_sommets; i++){
        // On déclare des variables tampons pour chercher le sommet de travail (plus courte distance)
        int s_courant = graphalgo::MAXPOIDS, distSommet = graphalgo::MAXPOIDS;
        bool inacessible = true;

        // On cherche le sommet de travail
        for(unsigned j = 0; j < sommets_non_traites.size(); j++){
            int s = sommets_non_traites[j] - 1;
            if(d[s] < distSommet){
                s_courant = s;
                distSommet = d[s];
                // Il reste au moins un sommet accessible
                inacessible = false;
            }
        }

        // Je vérifie que les sommets restant sont accessibles
        if(!inacessible){
            // On traite les successeurs du sommet de travail
            int indice = aps[s_courant + 1];
            while(fs[indice] != 0){
                // On récupère les sonnées
                int sommet_successeur = fs[indice] - 1;
                int c_sommet = d[s_courant];
                int c_chemin = matCout[s_courant][sommet_successeur];

                // On test si le nouveau chemin est plus court
                if(c_sommet + c_chemin < d[sommet_successeur]){
                    // On implémente
                    d[sommet_successeur] = c_sommet + c_chemin;
                    pred[sommet_successeur] = s_courant + 1;
                }
                indice++;
            }
        }

        // sommets_non_traites.erase(sommets_non_traites.begin() + s_courant);
        sommets_non_traites = retireIndex(sommets_non_traites, s_courant);
    }


    // On déclare le graph réduit
    graphalgo::graph newG = graphalgo::graph(nb_sommets, g.oriented());
    // On remplit newG
    for(int i = 0; static_cast<unsigned>(i) < pred.size(); i++){
        if(pred[i] != i + 1)
            newG.add_successor(pred[i], i + 1, d[i]);
    }
    // On retourne le graph réduit
    return newG;
}

/*graphalgo::graph dijkstra(int s, graphalgo::graph &g) {
    int ind;
    int i, j, k, v, m;
    bool oriented = g.oriented();
    std::vector<int> fs, aps;
    g.fs_aps(fs, aps);
    std::vector<std::vector<int>> p = g.cost_matrice();
    int n = aps[0];
    m = fs[0];

    // Initialiser les tableaux et structures de données
    std::vector<int> pr(n + 1, -1);
    std::vector<int> d(n + 1, graphalgo::MAXPOIDS);
    std::vector<int> inS(n + 1, 1); // 1 pour les sommets à traiter

    for (i = 0;i < n; i++)
        d[i+1] = p[s-1][i];

    // Initialiser les tableaux de résultats
    d[s] = 0;
    pr[s] = 0;
    inS[s] = 0; // Marquer le sommet 's' comme traité
    ind = n - 1; // Nombre d'éléments restants à traiter

    while (ind > 0) {
        // Trouver le sommet non traité avec la plus petite distance
        m = graphalgo::MAXPOIDS;
        for (int i = 1; i <= n; i++) {
            if (inS[i] == 1 && d[i] < m) {
                m = d[i];
                j = i;
            }
        }

        // S'il n'y a plus de sommets à traiter, terminer
        if (m == graphalgo::MAXPOIDS) break;

        inS[j] = 0; // Marquer le sommet 'j' comme traité
        ind--;

        // Mettre à jour les distances et les prédécesseurs pour les successeurs de 'j'
        k = aps[j];
        while (fs[k] != 0) {
            if (inS[fs[k]] == 1) {
                v = d[j] + p[j-1][fs[k]-1];
                if (v < d[fs[k]]) {
                    d[fs[k]] = v;
                    pr[fs[k]] = j;
                }
            }
            k++;
        }
    }

    // Construction du graphe résultant à partir des prédécesseurs
    graphalgo::graph result(n, oriented);
    for (int i = 1; i <= n; i++) {
        if (pr[i] != -1) {
            result.add_successor(pr[i], i, d[i]);
        }
    }

    return result;
}*/

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



void graphalgo::traversee(int s, int &k, int &p, const std::vector<int> &fs, const std::vector<int> &aps, std::vector<int>& prem, std::vector<int>& pilch, std::vector<int>& cfc, std::vector<int>& pred, std::vector<int>& tarj, std::vector<bool>& entarj, std::vector<int>& num, std::vector<int>& ro)
{
    int t, u;
    // numérote s et initialise ro[s]
    p++;
    num[s] = p;
    ro[s] = p;
    empiler(s, tarj);
    entarj[s] = true;
    for (int k = aps[s]; (t=fs[k]) != 0 ; k++) {
        if (num[t] == 0) { // si t n'est pas encore numéroté
            pred[t] = s;
            traversee(t, k, p, fs, aps, prem, pilch, cfc, pred, tarj, entarj, num, ro);
            if (ro[t] < ro[s]) ro[s] = ro[t]; // Z1
        } else {
            if ((num[t] < ro[s]) && entarj[t]) ro[s] = num[t]; // Z2
        }
    }

    if (ro[s] == num[s]) {
        k++;
        do {
            u = depiler(tarj);
            entarj[u] = false;
            empiler(u, pilch);
            cfc[u] = k;
        } while (u != s);
        prem[k] = pilch[0];
        pilch[0] = 0;
    }
}

void graphalgo::fortconnexe(const std::vector<int>& fs, const std::vector<int>& aps, std::vector<int>& prem, std::vector<int>& pilch, std::vector<int>& cfc, std::vector<int>& pred)
{
    int n = aps[0];
    prem.resize(n + 1);
    pilch.resize(n + 1);
    cfc.resize(n + 1);
    pred.resize(n + 1, 0);

    std::vector<int> tarj(n + 1);
    std::vector<bool> entarj(n + 1, false);
    std::vector<int> num(n + 1, 0);
    std::vector<int> ro(n + 1, 0);

    int p = 0;
    int k = 0;

    pilch[0] = 0;
    tarj[0] = 0;

    for (int s = 1; s <= n; s++)
        if (num[s] == 0)
            traversee(s, k, p, fs, aps, prem, pilch, cfc, pred, tarj, entarj, num, ro);

    prem[0] = k;
}

graphalgo::graph graphalgo::graph_reduit(const std::vector<int>& prem, const std::vector<int>& pilch, const std::vector<int>& cfc, const std::vector<int>& fs, const std::vector<int>& aps) {
    int s, kr = 1, k, t, nbc = prem[0];
    std::vector<bool> deja_mis(nbc + 1, false);
    vector<int> fsr(fs[0] + 1);
    vector<int> apsr(nbc + 1);
    apsr[0] = nbc;

    for (int i = 1; i <= nbc; i++) {
        apsr[i] = kr;
        deja_mis[i] = true;
        s = prem[i];
        while (s != 0) {
            for (int k = aps[s]; (t = fs[k]) != 0; k++) {
                if (deja_mis[cfc[t]] == false) {
                    fsr[kr] = cfc[t];
                    kr++;
                    deja_mis[cfc[t]] = true;
                }
            }
            s = pilch[s];
        }
        fsr[kr] = 0;
        kr++;
    }
    fsr[0] = kr - 1;

    graphalgo::graph gr{fsr, apsr};
    return gr;
}
