#include "sudoku.hpp"
#include <iostream>
#include <vector>
#include <complex>
#include <algorithm> //nouveau !
#include <cmath> //nouveau !
#include <random> //nouveau !
#include <fstream>
#include <list>

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
    suint k_ligne = (ligne / n) * n; //division entière (tronque la partie décimale)
    suint k_col = (col / n) * n;
    
    //on créé une liste de toutes les valeurs possibles pour une case (dans une grille vide)
    vector<suint> vals;
    for(int i=1; i <= taille_cote; ++i){
        vals.push_back(i);
    }

    //on parcourt la ligne
    for(int i=0; i<taille_cote; i++){
        suint test = (*this)[ligne][i];
        if(estPresent(vals, test)){
            vals.erase(remove(vals.begin(), vals.end(), test), vals.end());
        }
    }
    //on parcourt la colonne
    for(int i=0; i<taille_cote; i++){
        suint test = (*this)[i][col];
        if(estPresent(vals, test)){
            vals.erase(remove(vals.begin(), vals.end(), test), vals.end());
        }
    }
    //on parcourt le bloc
    for(int i = k_ligne; i < k_ligne + n; i++){
        for(int j = k_col; j  < k_col + n; j++){
            suint test = (*this)[i][j];
            if(estPresent(vals, test)){
                vals.erase(remove(vals.begin(), vals.end(), test), vals.end());
            }
        }
    }
    return vals;
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

void Grille::afficher(){
    int taille = n*n;

    for(int i = 0; i<taille;i++){
        for(int j = 0; j<taille;j++){
            cout<<(*this)[i][j]<<" ";
        }
        cout<<endl;
    }
}



//========================================
// Classe Sudoku =========================
// =======================================

// Constructeurs

Sudoku :: Sudoku(int n) // Constructeur par défaut 
{
    this->ordre = n;
    this->grille_ini = Grille(n);
}

Sudoku :: Sudoku(const Grille& g): grille_ini(g) // Constructeur à partir de la classe Grille 
{
    this->ordre = g.n;
    this->grille_ini.majcasesVides();
}

bool Sudoku::Solution(int n)
{
    if(n==grille_ini.casesVides.size()) // Condition d'arrêt si n = N 
    {
        grille_sol.push_back(grille_ini);
        if (allSol)
        {
            return false;
        }
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
            return true;
        }
    }

    grille_ini[ligne][col] = 0;
    return false;
}

// Liste des grilles //


// Affichage de la solution //

void Sudoku::affiche_sudoku() 
{
    cout << "========================================" << endl;
    cout << "          GRILLE INITIALE" << endl;
    cout << "========================================" << endl;
    grille_ini.afficher();

    cout << endl;
    cout << "Nombre de solutions trouvees : " << grille_sol.size() << endl;
    
    int i = 1;
    for (auto& solution : grille_sol) 
    {
        cout << endl;
        cout << ">>> Solution " << i << " <<<" << endl;
        solution.afficher();
        i++;
    }
    cout << "========================================" << endl;
}