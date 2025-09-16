#include <stdio.h>
#include "board.h"
#include <ctype.h>

void afficher_board(board game) {
	int cmt = 0;//counter counting each lines
	printf("        0   1   2   3   4   5   6\n");
	printf("      ├───┼───┼───┼───┼───┼───┼───┤\n");
    for (int i = 0; i < NB_LINES ; i++) {
		cmt++;
		printf("    ");
		printf("%d ", cmt-1);//display number of each lines
        for (int j = 0; j < NB_COLS ; j++) {
			printf("│");
            cell content = get_content(game, i, j);
            switch (content) {
                case EMPTY: printf("   "); 
                break;
                case NORTH_KING: printf(" \033[34mN\033[0m ");//change the color of N to blue
                break;
                case SOUTH_KING: printf(" \033[32mS\033[0m ");//change the color of S to green
                break;
                case KILLED: printf(" \033[31mX\033[0m "); //change the color of X to green
                break;
            }
        }
        printf("│\n");
        printf("      ├───┼───┼───┼───┼───┼───┼───┤\n");
    }
    printf("\n");
}

void afficher_move(){
	printf("   ┌───┬───┬───┐\n");
	printf("   │NW │ N │NE │\n");
	printf("   ├───┼───┼───┤\n");
	printf("   │ W │ * │ E │\n");
	printf("   ├───┼───┼───┤\n");
	printf("   │SW │ S │SE │\n");
	printf("   └───┴───┴───┘\n");
	printf("\n");
}

int nom_valide(char nom[]) { //verifie si le nom est un char et pas un int
    int i = 0;
    while (nom[i] != '\0') {
        if (isdigit(nom[i]) || nom[i] == ' ') {
            return 0;
        }
        i++;
    }
    return 1;
}

void saisir_noms(char joueur1[], char joueur2[]){//enter a name for each players
	do {
	printf("Entrez un nom pour le joueur 1 : ");
	scanf("%s", joueur1);
	} while (!nom_valide(joueur1));
	do {
	printf("Entrez un nom pour le joueur 2 : ");
	scanf("%s", joueur2);
	} while (!nom_valide(joueur2));
}

int choix_portee(){//if return true then range rules are applied
	char portee;
	printf("Voulez-vous jouer avec les règles de portées ? : (Y/N) ");
	scanf("%c", &portee);
	if(portee == 'Y'){
		return true;
	}else{
		return false;
	}
}

void scenario() { //scenario of a game for tests
	board game = new_game();
	afficher_board(game);
	
	printf("joueur : %d\n", current_player(game));
	move_toward(game, SE);	
	afficher_board(game);
	
	printf("joueur : %d\n", current_player(game));
	move_toward(game, N);
	afficher_board(game);
	
	printf("joueur : %d\n", current_player(game));
}

void initialiser(board* game, char joueur1[], char joueur2[]){
	*game = new_special_game(false,choix_portee());
	saisir_noms(joueur1,joueur2);
	printf("Un plateau est créé.\n");
	afficher_board(*game);
}

void joueur_tour(board game, char nom_joueur[]) {
	printf("Tour du joueur %s \n",nom_joueur);
	afficher_move(game);
	int direction;
	printf("déplacez-vous \n");
	printf("(0=SW, 1=S, 2=SE, 3=W, 4=E, 5=NW, 6=N, 7=NE) : ");
	scanf("%d", &direction);
	while (direction < 0 || direction > 7) {
		printf("Direction invalide ! Essayez encore\n");
		printf("(0=SW, 1=S, 2=SE, 3=W, 4=E, 5=NW, 6=N, 7=NE) : ");
		scanf("%d", &direction);
	}
	enum return_code result = move_toward(game, direction);
	if(result == OK){
		printf("déplacement réussi.\n");
	} else {
		printf("Aïe, pas réussi à déplacer la pièce, Essayez encore\n");
		printf("(0=SW, 1=S, 2=SE, 3=W, 4=E, 5=NW, 6=N, 7=NE) : ");
		scanf("%d", &direction);
	}
	afficher_board(game); //display board after modifications
	int line;
	int col;
	printf("retirer une case (ligne colonne) : ");
	scanf("%d %d", &line, &col);//indicate which cell we want to kill
	enum return_code result2 = kill_cell(game, line, col);
	while(result2 == RULES){
		printf("La case que vous tentez de supprimer est occupé\n");
		printf("ou vous n'êtes pas à la bonne porté\n");
		printf("Essayez encore (ligne colonne) : ");
		scanf("%d %d", &line, &col);
		kill_cell(game, line, col);
	}
	if(result2 == OK){
		printf("ça marche!\n");
		kill_cell(game, line, col);
	}
	else {
		printf("Bizarre. Essayez encore\n");
		printf("retirer une case (ligne colonne) : ");
		scanf("%d %d", &line, &col);
		kill_cell(game, line, col);
	}
	afficher_board(game);
}

int main() {
	board game;
	char joueur1[50];
	char joueur2[50];

	initialiser(&game, joueur1, joueur2);
	while (get_winner(game) == NO_PLAYER){
		joueur_tour(game,joueur1);
		if (get_winner(game) != NO_PLAYER) break;
		joueur_tour(game,joueur2);
	}

	player winner = get_winner(game);
	printf("La partie est terminée ! Le gagnant est : %s\n",
	(winner == NORTH) ? "North" : "South");//give the winner of the game
	
	destroy_game(game);
	printf("suppression du plateau et sortie\n");
    return 0;
}

  git config --global user.email "gorkemyildiz7ds@gmail.com"
  git config --global user.name "Gorkem387"