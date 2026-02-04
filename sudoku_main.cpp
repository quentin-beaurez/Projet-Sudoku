#include <Windows.h>
#include "sudoku.hpp"
using namespace std;

int main(){
    SetConsoleOutputCP(65001);
    /*
    //test affichage et importation de grille
    Grille Test(2);
    Test.importer("grille1.txt");
    Test.afficher();

    //test generation de grille
    Grille gr(3);
    gr.generation();
    gr.afficher();
    
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
    */

    // ==========================================
    // ETAPE 1 : GENERATION DE LA GRILLE UNIQUE
    // ==========================================
    cout << "--- Generation d'un Sudoku a solution unique en cours... ---" << endl;
    
    // On crée un objet Sudoku juste pour utiliser la méthode (l'ordre 3 = 9x9)
    Sudoku generateur(3); 
    
    // Appel de votre fonction
    Grille maGrilleUnique = generateur.Solution_unique(0.30);

    cout << "\n>>> Grille partielle générée (avec des trous) :" << endl;
    maGrilleUnique.afficher();

    // ==========================================
    // ETAPE 2 : VERIFICATION (PREUVE)
    // ==========================================
    cout << "\n--- Verification de l'unicite ---" << endl;

    // On crée un nouveau jeu avec cette grille
    Sudoku verificateur(maGrilleUnique);
    
    // IMPORTANT : On active la recherche de TOUTES les solutions
    verificateur.allSol = true; 
    
    // On lance la résolution
    verificateur.Solution(0);

    // ==========================================
    // ETAPE 3 : RESULTAT
    // ==========================================
    int nbSolutions = verificateur.grille_sol.size();
    cout << "Nombre de solutions trouvees par le solveur : " << nbSolutions << endl;

    if (nbSolutions == 1) {
        cout << "VICTOIRE ! La fonction marche correctement." << endl;
        // Optionnel : Afficher la solution unique trouvée
        cout << "\nVoici la solution unique :" << endl;
        verificateur.grille_sol.front().afficher();
    } else {
        cout << "ECHEC. La grille a " << nbSolutions << " solutions." << endl;
    }

    return 0;
}