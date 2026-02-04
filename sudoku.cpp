#include "sudoku.hpp"
#include <iostream>
#include <vector>
#include <complex>
#include <algorithm> //nouveau !
#include <cmath> //nouveau !
#include <random> //nouveau !
#include <list>
#include <fstream>


using namespace std;

//===========================================================
// classe Grille ============================================
//===========================================================

//constructeur d'une grille d'ordre quelconque (grille remplie de zéros <=> vide)
Grille::Grille(int ordre){
    this->n = ordre; //on fixe l'ordre de la grille
    int taille_cote = n*n; //on calcule les dimensions de la grille. ex : n = 3 donne une grille de 9x9
    this->resize(taille_cote); //on resize pour avoir le bon nombre de lignes
    
    //pour chaque ligne de 'this' on la resize pour avoir le bon nombre de colonnes et on
    //les rempli de 0 (pour avoir une grille vide)
    for(int i=0; i<taille_cote; i++){ 
        (*this)[i].resize(taille_cote,0);
    }
}

//constructeur de grille par défaut : ordre 3 (appelle le constructeur ci-dessus)
Grille::Grille() : Grille(3){}

bool Grille::estPresent(const vector<suint>& liste, suint valeur) {
    return find(liste.begin(), liste.end(), valeur) != liste.end();
}

vector<suint> Grille::listeadmissibles(pair<suint,suint> coord){
    suint ligne = coord.first;
    suint col = coord.second;
    suint taille_cote = n*n;
   
    //on détermine le coin haut gauche du bloc dans lequel se trouve la case d'intérêt
    suint k_ligne = (ligne / n) * n; //division entière (tronque la partie décimale) : division entière par défaut car on divise un entier par un entier
    suint k_col = (col / n) * n;
    
    // On crée un tableau de booléens initialisés à 'false'.
    // Taille + 1 pour pouvoir utiliser l'index direct
    vector<bool> est_interdit(taille_cote + 1, false);

    //on parcourt la ligne
    for(int j=0; j<taille_cote; j++){
        suint val = (*this)[ligne][j];
        if (val != 0) est_interdit[val] = true;
    }
    
    //on parcourt la colonne
    for(int i=0; i<taille_cote; i++){
        suint val = (*this)[i][col];
        if (val != 0) est_interdit[val] = true;
    }
    //on parcourt le bloc
    for(int i = k_ligne; i < k_ligne + n; i++){
        for(int j = k_col; j  < k_col + n; j++){
            suint val = (*this)[i][j];
            if (val != 0) est_interdit[val] = true;
        }
    }

    //On récupère les valeurs admissibles
    vector<suint> resultats;
    for(suint val = 1; val <= taille_cote; val++){
        if (est_interdit[val] == false){ // Si pas interdit
            resultats.push_back(val);
        }
    }
    return resultats;
}

//fonction permettant de mettre à jour les cases vides de la grille
void Grille::majcasesVides(){
    vector<pair<suint,suint>> cases;
    int taille_cote = (*this).n * (*this).n;
    for(int i = 0; i< taille_cote; i++){
        for(int j = 0; j< taille_cote; j++){
            if( (*this)[i][j] == 0){
                cases.push_back({i,j});
            }
        }
    }
    (*this).casesVides = cases;
}

//Génère une grille aléatoire : pas forcément résolvable, avec des valeurs aléatoires dans des positions aléatoires, en densité précisée
void Grille::generation(float densite){
    //Grille grille(ordre); //on construit une grille d'ordre 'ordre' vide à l'aide du constructeur de la classe
    
    //détermination du nombre de cases remplies initial ( = 0)
    int taille_cote = this->n * this->n;
    int nbr_cases = taille_cote*taille_cote; 
    int casesRemplies = 0;

    //détermination du nombre de cases à remplir
    int cases_a_remplir = ceil(densite*nbr_cases);

    //on détermine une ditribution de nombres aléatoires entre 1 et taille_cote pour avoir des coordonnées aléatoires
    //le static permet de pas recréer le moteur aléatoire à chaque appel de la fonction (optionnel mais permet d'aller plus vite si on appel la fonction de nombreuses fois)
    static random_device rd;
    static default_random_engine eng(rd());
    
    //distribution de valeurs des coordonnées
    uniform_int_distribution<int> distrib_coord(0, taille_cote-1);

    //distibution de valeurs aléatoires à rentrer dans la case désignée
    uniform_int_distribution<int> distrib_val(1, taille_cote); //valeur aléatoire à rentrer dans la case désignée
    
    // Par sécurité : permet d'éviter une boucle infinie si la densité est trop haute
    int tentatives_max = cases_a_remplir * 100; 
    int tentatives = 0;

    while(casesRemplies < cases_a_remplir && tentatives < tentatives_max){
        tentatives++;
        int i_ = distrib_coord(eng);
        int j_ = distrib_coord(eng);

        if((*this)[i_][j_] == 0){ //on modifie uniquement si la case est vide pour ne pas faire des changements pour rien
            int val = distrib_val(eng); //valeur à mettre dans la case (i_,j_)
            vector<suint> possibles = (*this).listeadmissibles({(suint)i_, (suint)j_});

            if ((*this).estPresent(possibles, val)) {
                (*this)[i_][j_] = val;
                casesRemplies++;
            }
        }
        
    }
    (*this).majcasesVides(); //on met à jour les cases vides restantes dans la grille
}

void Grille::importer(const string& nomFichier){
    ifstream fichier(nomFichier);    //ouverture du fichier en lecture

    if (!fichier.is_open()){
        cout<<"Erreur : impossible d'ouvrir le fichier"<<endl;
        return;}

    int taille = n*n;
    
    for (int i = 0;i<taille;i++){
        for(int j =0;j<taille;j++){
        fichier>>(*this)[i][j];
        }
    }
    fichier.close();
    cout<<"Grille importée avec succès depuis "<<nomFichier<<endl;

}

void Grille::exporter(const string& nomFichier){
    ofstream fichier(nomFichier);    //ouverture du fichier en écriture

    if (!fichier.is_open()){
        cout<<"Erreur : impossible d'ouvrir le fichier"<<endl;
        return;
    }
    int taille = n*n;

    for(int i=0;i<taille;i++){
        for(int j=0;j<taille;j++){
            fichier<<(*this)[i][j]<<" ";
        }
        fichier << endl;
    }
    fichier.close();
    cout<<"Grille exportée avec succès dans "<<nomFichier<<endl;

}

void Grille::afficher() {
    int taille = n * n;

    // Caractères Unicode
    const char* gTL = u8"╔"; const char* gTR = u8"╗";
    const char* gBL = u8"╚"; const char* gBR = u8"╝";
    const char* gH  = u8"═"; const char* gV  = u8"║";
    const char* gC  = u8"╬"; const char* gTT = u8"╦";
    const char* gTB = u8"╩"; const char* gTLf = u8"╠";
    const char* gTRg = u8"╣";
    const char* sV = u8"│"; const char* sH = u8"─"; const char* sC = u8"┼";

    // BORDURE DU HAUT 
    cout << gTL;
    for (int b = 0; b < n; b++) {
        for (int k = 0; k < n; k++) {
            cout << gH << gH << gH;     // Largeur de la case
            if (k < n - 1) cout << gH;  // Largeur de la barre fine │
        }
        if (b < n - 1) cout << gTT;     // Jonction barre épaisse ║
    }
    cout << gTR << endl;

    // CONTENU 
    for (int i = 0; i < taille; i++) {
        cout << gV; // Bordure gauche
        for (int j = 0; j < taille; j++) {
            // Chiffre
            if ((*this)[i][j] == 0) cout << "   ";
            else cout << " " << (*this)[i][j] << " ";

            // Séparateur vertical
            if ((j + 1) % n == 0) {
                if (j + 1 < taille) cout << gV; // Barre épaisse
            } else {
                cout << sV; // Barre fine
            }
        }
        cout << gV << endl; // Bordure droite

        // SÉPARATEURS HORIZONTAUX
        if (i < taille - 1) {
            if ((i + 1) % n == 0) {
                // Ligne épaisse entre les blocs
                cout << gTLf;
                for (int b = 0; b < n; b++) {
                    for (int k = 0; k < n; k++) {
                        cout << gH << gH << gH;
                        if (k < n - 1) cout << gH;
                    }
                    if (b < n - 1) cout << gC;
                }
                cout << gTRg << endl;
            } else {
                // Ligne fine à l'intérieur d'un bloc
                cout << gV;
                for (int b = 0; b < n; b++) {
                    for (int k = 0; k < n; k++) {
                        cout << sH << sH << sH;
                        if (k < n - 1) cout << sC;
                    }
                    if (b < n - 1) cout << gV;
                }
                cout << gV << endl;
            }
        }
    }

    // BORDURE DU BAS 
    cout << gBL;
    for (int b = 0; b < n; b++) {
        for (int k = 0; k < n; k++) {
            cout << gH << gH << gH;
            if (k < n - 1) cout << gH;
        }
        if (b < n - 1) cout << gTB;
    }
    cout << gBR << endl;
}


//========================================
// Classe Sudoku =========================
// =======================================

// Constructeurs

//CHANGEMENTS DES CONSTRUCTEURS !!!
Sudoku :: Sudoku(int n) : ordre(n), grille_ini(n) {}// Constructeur par défaut 

Sudoku :: Sudoku(const Grille& g): grille_ini(g), ordre(g.n) // Constructeur à partir de la classe Grille 
{
    this->grille_ini.majcasesVides();
}

//CHANGEMENTS DE LA FONCTION SOLUTION : 
bool Sudoku::Solution(int n)
{
    int N = grille_ini.casesVides.size();
    if(n==N) // Condition d'arrêt si n = N 
    {
        return true;
    }
    suint ligne = grille_ini.casesVides[n].first;
    suint col = grille_ini.casesVides[n].second;

    vector<suint> admissibles = grille_ini.listeadmissibles({ligne, col});

    for(suint val : admissibles)
    {
        grille_ini[ligne][col] = val;
        if (Solution(n + 1))
        {
            if (n < N-1){return true;}
            
            grille_sol.push_back(grille_ini);
            
            if (!allSol){return true;}
        }
        grille_ini[ligne][col] = 0; //val ne convient pas
    }

    return false;
}

// Liste des grilles //


// Affichage de la solution //
void Sudoku::affiche_sudoku(int limite) 
{
    cout << "========================================" << endl;
    cout << "          GRILLE INITIALE" << endl;
    cout << "========================================" << endl;
    grille_ini.afficher();

    cout << endl;
    cout << "Nombre de solutions trouvees : " << grille_sol.size() << endl;
    
    if (limite < grille_sol.size()){
        cout << "On affiche les " << limite << " premieres solutions" << endl;
    }

    int i = 1;
    for (auto& solution : grille_sol) 
    {
        if(i==limite+1){break;}
        cout << endl;
        cout << ">>> Solution " << i << " <<<" << endl;
        solution.afficher();
        i++;
    }
    cout << "========================================" << endl;
}

// Construit une grille à solution unique
Grille Sudoku::Solution_unique(float densite_obj){
    int ordre_ = this->ordre;
    float densite_ = 0.25; // densité de la grille initiale pour trouver facilement une première solution

    // Calcul du nombre de cases qu'on doit garder au minimum
    int taille_totale = ordre_ * ordre_ * ordre_ * ordre_;
    int nombre_cases_cible = ceil(densite_obj * taille_totale);

    //parametres dont la modification devra etre implantée (apres avoir fini la fct)
    int casesSuppr = 1; // nombres de cases à supprimer par itération
    int itermax = 100;

    // ------- ETAPE 1 : on genère une grille pleine -------
    Grille g(ordre_); // construction d'une grille d'ordre 'ordre_'
    Grille grille_complete(ordre_); //on prépare la grille complète
    bool grille_trouvee = false; //booléen pour s'arrêter quand on trouve notre grille complète
    

    // On boucle tant qu'on n'a pas généré une grille valide complète
    while(!grille_trouvee){
        g = Grille(ordre_);     // on reset (inutile à l'étape 1 mais sert pour toutes les autres)
        g.generation(densite_); // Remplissage partiel de la grille 

        Sudoku s(g); 
        s.allSol = false; // On cherche juste une solution pour la compléter
        if(s.Solution(0)){
            // On récupère la grille pleine
            if(!s.grille_sol.empty()){
                grille_complete = s.grille_sol.front();
                grille_trouvee = true;
            }
        }
    }

    // ------- ETAPE 2 : on fait des trous jusqu'à ce qu'on perde l'unicité -------
    Grille g_courante = grille_complete;
    Grille g_precedente = grille_complete; // Sauvegarde de la dernière grille valide
    
    bool estUnique = true; //la solution est unique
    int securite = 0; //pour pas que le suppression soit infinie

    // Compteur de cases remplies (au début, la grille est pleine)
    int cases_remplies_actuelles = taille_totale;

    // Configuration pour le générateur aléatoire
    static random_device rd;
    static default_random_engine eng(rd());
    int taille_cote = ordre_ * ordre_;
    uniform_int_distribution<int> distrib_coord(0, taille_cote-1);

    while(estUnique && securite < itermax && cases_remplies_actuelles > nombre_cases_cible){
        securite++;

        // Sauvegarde de l'état actuel (qui est valide et unique)
        g_precedente = g_courante;

        // Suppression des cases
        int casesAEnlever = casesSuppr;
        int tentatives_suppr = 0; //sécurité
        
        while(casesAEnlever > 0 && tentatives_suppr < 100){
            tentatives_suppr++;
            int i = distrib_coord(eng);
            int j = distrib_coord(eng);

            if(g_courante[i][j] != 0){ // Si la case n'est pas déjà vide
                g_courante[i][j] = 0;  // On la vide
                casesAEnlever--;
                cases_remplies_actuelles--;
            }
        }

        // Vérification de l'unicité
        Sudoku testeur(g_courante);
        testeur.allSol = true; // IMPORTANT : On veut compter les solutions et vérifier qu'il n'y en a qu'une seule
        testeur.Solution(0); 

        if(testeur.grille_sol.size() == 1){
            // C'est toujours unique ! On continue la boucle pour enlever d'autres cases
            estUnique = true;
        } else {
            // solution plus unique, on a enlevé trop de cases
            estUnique = false;
        }
    }
    float densite_actuelle = (float)cases_remplies_actuelles / taille_totale;

    cout << "Densité atteinte : " << densite_actuelle << endl;
    cout << "Densité voulue : " << densite_obj << endl;
    
    if (estUnique) {
        return g_courante;
    } else { // cas où on atteint pas la densité cible
        return g_precedente;
    }   
}