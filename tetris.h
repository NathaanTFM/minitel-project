#include "games.h"
#include "minitel.h"

#define TETRIS_LONGUEUR 10
#define TETRIS_POSITION 15

struct InfoPiece {
  unsigned char taille;
  unsigned char* rotations[4];
};

struct Piece {
  struct InfoPiece* info;
  unsigned char rotation;
  struct Position position;
};

struct TetrisGame {
  unsigned int state;
  unsigned long frame;
  
  unsigned char memoire_pieces[260];
  struct InfoPiece pieces[7];

  unsigned char grille[HAUTEUR_ECRAN][TETRIS_LONGUEUR];
  
  // piece en cours
  struct Piece piece;
};

void init_tetris(struct TetrisGame* game, struct Evenements* events);
void loop_tetris(struct TetrisGame* game);
