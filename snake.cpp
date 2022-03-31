#include <Arduino.h>
#include "snake.h"

void start_snake(struct SnakeGame*);

// image "bannière"
static const char banner_data[] PROGMEM = "\x0c\x0e\x0a\x20\x12\x45\x50\x12\x41\x30\x20\x50\x30\x20\x12\x41\x50\x12\x41\x20\x12\x42\x40\x20\x12\x42\x40\x50\x12\x41\x20\x50\x12\x41\x20\x50\x12\x43\x30\x20\x12\x49\x5e\x20\x12\x41\x25\x20\x4a\x4f\x30\x20\x12\x41\x35\x20\x12\x41\x40\x4f\x34\x20\x12\x42\x4a\x20\x12\x41\x36\x20\x12\x42\x35\x20\x12\x41\x35\x20\x12\x49\x2b\x5c\x50\x20\x12\x41\x4a\x20\x4f\x34\x20\x35\x20\x12\x41\x3a\x20\x5d\x20\x12\x42\x4a\x40\x56\x20\x12\x43\x55\x50\x35\x20\x12\x4c\x23\x5d\x20\x4a\x20\x12\x41\x4b\x34\x35\x20\x40\x2d\x2c\x4e\x34\x20\x12\x41\x4a\x21\x2a\x55\x20\x12\x42\x35\x20\x25\x20\x12\x4a\x55\x50\x12\x41\x27\x20\x5a\x50\x20\x12\x41\x4b\x35\x40\x5a\x30\x20\x50\x5d\x30\x40\x5a\x50\x20\x5b\x54\x30\x50\x55\x50\x12\x41\x5e\x20\x12\x70\x12\x70\x12\x44\x1b\x46\x50\x40\x50\x20\x12\x63\x1b\x47\x48\x24\x3c\x1b\x43\x26\x1b\x46\x35\x20\x12\x62\x1b\x47\x2a\x1b\x43\x44\x1b\x47\x2d\x1b\x43\x44\x1b\x46\x55\x20\x12\x60\x58\x5e\x1b\x56\x20\x12\x43\x1b\x57\x3f\x1b\x44\x2b\x1b\x40\x1b\x56\x30\x23\x4f\x5f\x12\x5c\x22\x20\x21\x1b\x46\x1b\x57\x3f\x27\x1b\x40\x1b\x53\x58\x1b\x57\x24\x40\x1b\x41\x1b\x56\x35\x20\x1b\x40\x22\x5f\x12\x5c\x1b\x57\x54\x50\x1b\x46\x12\x41\x1b\x40\x58\x3f\x21\x58\x35\x20\x1b\x56\x12\x42\x5f\x12\x5c\x3f\x1b\x41\x41\x1b\x44\x1b\x57\x23\x1b\x40\x41\x58\x5e\x2f\x1b\x44\x41\x1b\x46\x5e\x1b\x56\x20\x1b\x40\x5a\x5f\x12\x5b\x57\x34\x5f\x12\x42\x1b\x57\x27\x1b\x44\x41\x1b\x46\x58\x1b\x56\x20\x1b\x40\x40\x5e\x5f\x12\x5f\x1b\x57\x3f\x20\x1b\x46\x48\x1b\x56\x20\x12\x41\x1b\x40\x38\x23\x12\x41\x4f\x27\x23\x2f\x21\x5f\x12\x5a\x1b\x57\x35\x20\x1b\x46\x2a\x1b\x56\x20\x12\x41\x1b\x57\x3f\x1b\x56\x20\x12\x42\x1b\x40\x40\x5c\x1b\x44\x1b\x57\x53\x1b\x40\x1b\x56\x5e\x5f\x12\x5b\x1b\x57\x54\x50\x12\x41\x5c\x5f\x5c\x12\x41\x5e\x5f\x0f";
static const unsigned short banner_len = 432;

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
  for (int i = 0; i < banner_len; i++) {
    Serial.write(pgm_read_byte(banner_data + i)); // output the buffer.
  }
}

void loop_snake(struct SnakeGame* game) {
  if (game->state == 1) {
    if (millis() - game->frame >= 100) {
      frame_snake(game);
    }
  }
}
