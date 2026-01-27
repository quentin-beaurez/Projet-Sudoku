#include "sudoku.hpp"
#include <iostream>
#include <vector>
#include <complex>
#include <algorithm>

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
    for(int i = k_ligne; i <= k_ligne + n; i++){
        for(int j = k_col; j  <= k_col + n; j++){
            suint test = (*this)[i][j];
            if(estPresent(vals, test)){
                vals.erase(remove(vals.begin(), vals.end(), test), vals.end());
            }
        }
    }
    return vals;
}

//========================================
// Classe Sudoku =========================
// =======================================

// Constructeurs

Sudoku :: Sudoku(int n) // Constructeur par défaut 
{
    this->ordre = n;
    this->grille_ini = Grille(n)
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
        return true
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