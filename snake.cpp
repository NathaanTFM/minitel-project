#include <Arduino.h>
#include "snake.h"

void start_snake(struct SnakeGame*);

// image "bannière"
static const char banner[] PROGMEM = {12,14,10,32,18,69,80,18,65,48,32,80,48,32,18,65,80,18,65,32,18,66,64,32,18,66,64,80,18,65,32,80,18,65,32,80,18,67,48,32,18,73,94,32,18,65,37,32,74,79,48,32,18,65,53,32,18,65,64,79,52,32,18,66,74,32,18,65,54,32,18,66,53,32,18,65,53,32,18,73,43,92,80,32,18,65,74,32,79,52,32,53,32,18,65,58,32,93,32,18,66,74,64,86,32,18,67,85,80,53,32,18,76,35,93,32,74,32,18,65,75,52,53,32,64,45,44,78,52,32,18,65,74,33,42,85,32,18,66,53,32,37,32,18,74,85,80,18,65,39,32,90,80,32,18,65,75,53,64,90,48,32,80,93,48,64,90,80,32,91,84,48,80,85,80,18,65,94,32,18,112,18,112,18,68,27,70,80,64,80,32,18,99,27,71,72,36,60,27,67,38,27,70,53,32,18,98,27,71,42,27,67,68,27,71,45,27,67,68,27,70,85,32,18,96,88,94,27,86,32,18,67,27,87,63,27,68,43,27,64,27,86,48,35,79,95,18,92,34,32,33,27,70,27,87,63,39,27,64,27,83,88,27,87,36,64,27,65,27,86,53,32,27,64,34,95,18,92,27,87,84,80,27,70,18,65,27,64,88,63,33,88,53,32,27,86,18,66,95,18,92,63,27,65,65,27,68,27,87,35,27,64,65,88,94,47,27,68,65,27,70,94,27,86,32,27,64,90,95,18,91,87,52,95,18,66,27,87,39,27,68,65,27,70,88,27,86,32,27,64,64,94,95,18,95,27,87,63,32,27,70,72,27,86,32,18,65,27,64,56,35,18,65,79,39,35,47,33,95,18,90,27,87,53,32,27,70,42,27,86,32,18,65,27,87,63,27,86,32,18,66,27,64,64,92,27,68,27,87,83,27,64,27,86,94,95,18,91,27,87,84,80,18,65,92,95,92,18,65,94,95,15,0};

// utilitaires pour le serpent
struct Position get_head(struct Snake* snake)
{
  return snake->positions[(snake->index + snake->score - 1) % SNAKE_TAILLE_MAX];
}

struct Position get_tail(struct Snake* snake)
{
  return snake->positions[snake->index];
}

unsigned int get_next_index(struct Snake* snake)
{
  return (snake->index + snake->score) % SNAKE_TAILLE_MAX;
}

void move_next_pos(struct Snake* snake)
{
  snake->index = (snake->index + 1) % SNAKE_TAILLE_MAX;
}

// utilitaires pour le minitel
void set_pixel(unsigned char x, unsigned char y) {
  deplacer_curseur(x + 1, y + 1);
  Serial.write(0x7F);
}

void clear_pixel(unsigned char x, unsigned char y) {
  deplacer_curseur(x + 1, y + 1);
  Serial.write(0x20);
}

void reception_caractere_snake(struct SnakeGame* game, unsigned char code) {
  if (game->state == 0) {
    start_snake(game);
    
  } else if (game->state == 1) {
    if (code == 0x08 || code == 'Q') { // gauche
      if (game->snake.direction != D_DROITE)
        game->snake.direction = D_GAUCHE;
        
    } else if (code == 0x09 || code == 'D') {
      if (game->snake.direction != D_GAUCHE)
        game->snake.direction = D_DROITE;
        
    } else if (code == 0x0A || code == 'S') {
      if (game->snake.direction != D_HAUT)
        game->snake.direction = D_BAS;
        
    } else if (code == 0x0B || code == 'Z') {
      if (game->snake.direction != D_BAS)
        game->snake.direction = D_HAUT;   
    }
  }
}

void gameover_snake() {
  effacer_ecran();
  Serial.println("GAME OVER\n");
  delay(2000);

  // on reset
  setup();
}

void ajouter_bouffe_snake(struct SnakeGame* game) {
  for (;;) {
    unsigned char x = random(3, 38);
    unsigned char y = random(3, 21);

    if (game->grille[x][y] == 0) {
      game->grille[x][y] = 2;
      set_pixel(x, y);
      break;
    }
  }
}

void frame_snake(struct SnakeGame* game) {
  game->frame = millis();

  // on récupère les infos sur le serpent
  struct Position tail = get_tail(&game->snake);
  struct Position head = get_head(&game->snake);
  unsigned int idx = get_next_index(&game->snake);
  
  switch (game->snake.direction) {
    case D_DROITE:
      head.x += 1;
      break;

    case D_GAUCHE:
      head.x -= 1;
      break;

    case D_HAUT:
      head.y -= 1;
      break;

    case D_BAS:
      head.y += 1;
      break;
  }

  // est-ce qu'on est sorti ?
  if (head.x < 0 || head.x >= LONGUEUR_ECRAN) {
    return gameover_snake();
  }
  if (head.y < 0 || head.y >= HAUTEUR_ECRAN) {
    return gameover_snake();
  }
  if (game->grille[head.x][head.y] == 1) {
    return gameover_snake();
  }

  // nouvelle tête
  game->snake.positions[idx] = head;

  // est-ce qu'on a mangé?
  if (game->grille[head.x][head.y] == 2) {
    game->snake.score++;
    ajouter_bouffe_snake(game);
  } else {
    move_next_pos(&game->snake);
  }

  // on met à jour la grlile
  game->grille[tail.x][tail.y] = 0;
  game->grille[head.x][head.y] = 1;

  // on oublie pas de dessiner
  set_pixel(head.x, head.y);
  clear_pixel(tail.x, tail.y);
}

// lancer le jeu
void start_snake(struct SnakeGame* game) {  
  // on initialise la grille
  for (int x = 0; x < LONGUEUR_ECRAN; x++) {
    for (int y = 0; y < HAUTEUR_ECRAN; y++) {
      game->grille[x][y] = 0;
    }
  }

  // on initialise le serpent
  game->snake.index = 0;
  game->snake.score = 4;
  game->snake.direction = D_DROITE;
  
  // on initialise les positions du serpent
  for (int i = game->snake.index; i < game->snake.index + game->snake.score; i++) {
    game->snake.positions[i].x = 4;
    game->snake.positions[i].y = 4;
  }

  // on précise dans la grille qu'il y a un serpent à cette position
  game->grille[4][4] = 1;
  
  // on indique qu'on est dans l'état de jeu
  game->state = 1;

  // on efface l'écran (avant d'ajouter la bouffe)
  effacer_ecran();

  // on ajoute de la bouffe
  ajouter_bouffe_snake(game);
  
  // on démarre la première frame
  frame_snake(game);
}


// le jeu public
void init_snake(struct SnakeGame* game, struct Evenements* events) {
  reset_events(events);
  events->data = game;
  events->on_caractere = reception_caractere_snake;

  // on remet à zéro
  reset_minitel();
  
  // on veut pas d'echo chiant
  aiguillage_off(CR_ECRAN, CE_MODEM);

  // on veut accéder aux flèches
  fonction_start(CR_CLAVIER, ETEN);
  fonction_start(CR_CLAVIER, C0);

  // on vide l'écran
  effacer_ecran();

  // on initialise le jeu
  game->state = 0;
  game->frame = 0;

  // on affiche la bannière
  banner_begin(banner);
}

void loop_snake(struct SnakeGame* game) {
  if (game->state == 0) {
    banner_step();

  } else if (game->state == 1) {
    if (millis() - game->frame >= 100) {
      frame_snake(game);
    }
  }
}
