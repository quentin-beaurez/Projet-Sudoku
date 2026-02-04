#include <Windows.h>
#include "sudoku.hpp"
using namespace std;

int main(){
    /*
    //test affichage et importation de grille
    Grille Test(2);
    Test.importer("grille1.txt");
    Test.afficher();

    //test generation de grille
    Grille gr(3);
    gr.generation();
    gr.afficher();
    */
    //==============================================
    //Test des fonctions de résolution d'un Sudoku :
    //==============================================
    //création de la grille initiale
    Grille g(3);
    g.generation(0.25);

    //visualisation de la grille initiale 
    cout << "Grille initiale :" << endl;
    g.afficher(); 

    //initialisation du sudoku
    Sudoku sudoku(g);
    sudoku.allSol = true; //on dit qu'on veut chercher toutes les solutions

    cout << "Recherche des solutions..." << endl;
    sudoku.Solution(0); //on cherche toutes les solutions en commençant par la première case vide de la grille

    //affichage des solutions :
    sudoku.affiche_sudoku();
    //cout << sudoku.grille_sol.size() << endl;

    return 0;
}