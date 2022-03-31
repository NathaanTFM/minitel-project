#include "games.h"
#include "minitel.h"

#define SNAKE_TAILLE_MAX 128

struct Snake {
  struct Position positions[SNAKE_TAILLE_MAX];
  unsigned int index;
  unsigned int score;
  enum Direction direction;
};

struct SnakeGame {
  unsigned int state;
  unsigned long frame;
  
  struct Snake snake;
  unsigned char grille[LONGUEUR_ECRAN][HAUTEUR_ECRAN];
};

void init_snake(struct SnakeGame* game, struct Evenements* events);
void loop_snake(struct SnakeGame* game);
