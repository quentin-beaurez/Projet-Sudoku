// On inclut Windows.h SEULEMENT si on est sur Windows
#ifdef _WIN32
    #include <Windows.h>
#endif

#include "sudoku.hpp"


using namespace std;

int main(){
    // Cette commande ne s'exécutera que sur Windows
    #ifdef _WIN32
        SetConsoleOutputCP(65001);
    #endif
    
    //affichage et importation de grille
    /*
    Grille Test(2);
    Test.importer("grille1.txt");
    Test.afficher();
    */ 
   
    //generation de grille
    Grille gr(3);
    gr.generation();
    gr.afficher();
    
    // - Test des fonctions de résolution d'un Sudoku - //
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
    sudoku.Solution(0);

    //affichage des solutions :
    sudoku.affiche_sudoku();
    //cout << "Nombre de solutions trouvées :" << sudoku.grille_sol.size() << endl;
    

    
    // ==========================================
    // ETAPE 1 : GENERATION DE LA GRILLE UNIQUE
    // ==========================================
    cout << "--- Generation d'un Sudoku à solution unique en cours... ---" << endl;
    
    // On crée un objet Sudoku juste pour utiliser la méthode (l'ordre 3 = 9x9)
    Sudoku generateur(3); 
    
    // Appel de la fonction solution unique
    Grille maGrilleUnique = generateur.Solution_unique(0.3);

    cout << "\n>>> Grille partielle générée (avec des trous) :" << endl;
    maGrilleUnique.afficher();

    // ==========================================
    // ETAPE 2 : VERIFICATION (PREUVE de l'unicité)
    // ==========================================
    cout << "\n--- Verification de l'unicite ---" << endl;

    // On crée un nouveau jeu avec cette grille
    Sudoku verificateur(maGrilleUnique);
    
    // IMPORTANT : On active la recherche de TOUTES les solutions (on s'arrête à 2 car ça suffit pour prouver qu'il n'y a pas unicité)
    verificateur.allSol = true;
    verificateur.maxSol = 2;
    
    // On lance la résolution
    verificateur.Solution(0);

    // ==========================================
    // ETAPE 3 : RESULTAT
    // ==========================================
    if (verificateur.grille_sol.size() != 1){
        cout << "ERREUR : il n'y a pas unicité" << endl;
    } else {
        cout << "\nVoici la solution unique :" << endl;
        verificateur.grille_sol.front().afficher();
    }

    // ==========================================================
    // ETAPE 4 : TEST - JOUER AU SUDOKU
    // ==========================================================
    Sudoku sudo(3); 
    sudo.grille_ini.diagonale = false;
    sudo.Solution_unique(0.3);

    sudo.jouer();


    // ==========================================================
    // ETAPE 5 : TEST - SUDOKU DIAGONAL
    // ==========================================================
    cout << "\n\n================================================" << endl;
    cout << "   TEST SUPPLEMENTAIRE : SUDOKU DIAGONAL        " << endl;
    cout << "================================================" << endl;

    // Initialisation
    Sudoku geneDiag(3);

    // On active le mode diagonale
    geneDiag.grille_ini.diagonale = true;

    cout << "Generation d'une grille diagonale..." << endl;
    Grille gDiag = geneDiag.Solution_unique(0.3);
    gDiag.diagonale = true;

    cout << "\n>>> Grille partielle générée (avec des trous) : " << endl;
    gDiag.afficher();

    // Vérification
    cout << "\n --- Verification de l'unicite avec contraintes des diagonales principales ---" << endl;
    Sudoku verifDiag(gDiag);
    
    verifDiag.allSol = true;
    verifDiag.grille_ini.diagonale = true;

    verifDiag.Solution(0);

    int nbSolDiag = verifDiag.grille_sol.size();
    cout << "Nombre de solutions diagonales trouvees par le solveur : " << nbSolDiag << endl;

    if (nbSolDiag == 1) {
        cout << " VICTOIRE ! Grille diagonale unique trouvée." << endl;
        cout << "Solution avec contraintes des diagonales principales :" << endl;
        verifDiag.grille_sol.front().afficher();
    } 
    else if (nbSolDiag == 0) {
        cout << ">>> RATE. Aucune solution." << endl; 
        cout << "Cela veut dire que la generation aleatoire a place un chiffre interdit sur une diagonale." << endl;
    }
    else {
        cout << ">>> RATE. Pas unique (" << nbSolDiag << " solutions)." << endl;
    }
    


    // ==========================================================
    // ETAPE 6 : TEST - 3DOKU
    // ==========================================================
    
    // On initialise notre 3Doku
    Grille_3D cube;

    // On le remplit de manière à ce qu'il ait une solution unique
    cube.Solution_unique(0.4); 
    
    // - On prépare l'affichage de la solution en cas d'abandon de la partie - 
    // On copie la grille partielle dans une nouvelle variable
    Grille_3D vraie_solution = cube;
    vraie_solution.allSol = false; // On s'arrête à la première solution
    vraie_solution.Solution(0);    // L'ordinateur résout la grille partielle

    //Jeu
    cout << "\nVoici votre grille de depart :" << endl;
    cube.jouer(); 

    //on vérifie à la fin de la partie
    // Si la liste des cases vides n'est pas vide, c'est que le joueur a tapé 0 0 0 0 pour abandonner ou s'est retrouvé bloqué
    if (!cube.casesVides.empty()) {
        cout << "\nVous avez interrompu la partie. Voici la solution qu'il fallait trouver :" << endl;
        vraie_solution.afficher();
    } 
    else {
        // Le joueur a rempli toutes les cases, on vérifie s'il ne s'est pas trompé
        bool erreur = false;
        
        for (int f = 0; f < 6; f++) {
            for (int l = 0; l < 4; l++) {
                for (int c = 0; c < 4; c++) {
                    // On compare la grille du joueur avec la vraie solution
                    if (cube.faces[f][l][c] != vraie_solution.faces[f][l][c]) {
                        erreur = true;
                    }
                }
            }
        }

        if (erreur) {
            cout << "\nDommage, vous avez fait une erreur dans vos placements !" << endl;
            cout << "Voici la solution exacte attendue :" << endl;
            vraie_solution.afficher();
        } else {
            cout << "La grille est parfaitement exacte, bien joue !" << endl;
        }
    }
    
    return 0;
}