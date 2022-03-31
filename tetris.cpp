#include <Arduino.h>
#include "tetris.h"

void start_tetris(struct TetrisGame*);

// image "bannière"
static const char banner_data[] PROGMEM = "\x0c\x0e\x20\x12\x6a\x3f\x23\x4b\x37\x23\x4f\x23\x1b\x57\x20\x1b\x40\x5c\x12\x42\x30\x35\x58\x5c\x20\x5c\x54\x48\x34\x48\x5c\x54\x2b\x5f\x5d\x34\x48\x5e\x27\x58\x56\x21\x5f\x12\x48\x35\x4a\x5f\x12\x42\x20\x5f\x12\x41\x37\x5f\x12\x43\x20\x5f\x12\x42\x35\x4a\x5f\x3f\x40\x5f\x12\x41\x35\x4a\x5f\x30\x2b\x5f\x55\x5f\x12\x48\x35\x4a\x5f\x12\x42\x20\x5c\x12\x41\x34\x5f\x12\x43\x20\x5f\x12\x42\x35\x48\x34\x28\x5f\x12\x42\x35\x4a\x5f\x12\x41\x5c\x30\x2b\x5f\x12\x48\x35\x4a\x5f\x12\x42\x20\x5f\x12\x42\x3f\x5b\x5f\x12\x41\x20\x5f\x12\x42\x35\x4a\x5f\x34\x2b\x5f\x12\x41\x35\x4a\x5f\x2a\x5f\x12\x41\x34\x4a\x5f\x12\x46\x57\x51\x52\x5b\x5f\x53\x50\x53\x12\x42\x50\x5f\x12\x41\x53\x50\x53\x5f\x57\x51\x52\x5b\x5f\x54\x52\x53\x51\x52\x5b\x58\x56\x53\x58\x5f\x12\x70\x12\x70\x12\x50\x1b\x54\x3f\x4f\x5f\x12\x64\x3f\x1b\x44\x1b\x57\x25\x1b\x52\x20\x1b\x40\x1b\x54\x4f\x5f\x12\x63\x55\x1b\x41\x1b\x57\x50\x1b\x44\x1b\x52\x58\x1b\x40\x1b\x54\x5a\x5f\x12\x64\x35\x4a\x5f\x12\x65\x35\x4a\x5f\x12\x65\x1b\x44\x1b\x51\x36\x1b\x54\x20\x1b\x50\x12\x65\x1b\x42\x58\x1b\x44\x1b\x57\x4b\x1b\x52\x12\x41\x1b\x40\x1b\x54\x2b\x5f\x12\x61\x3f\x1b\x57\x27\x1b\x41\x58\x1b\x52\x25\x5e\x2a\x1b\x40\x1b\x51\x2b\x1b\x54\x4f\x5f\x12\x5e\x1b\x52\x27\x1b\x44\x1b\x57\x41\x1b\x41\x5e\x1b\x52\x25\x48\x1b\x51\x20\x1b\x52\x12\x41\x4d\x1b\x51\x20\x1b\x40\x1b\x54\x2b\x5f\x12\x5b\x3f\x1b\x57\x27\x1b\x41\x58\x1b\x51\x20\x1b\x52\x21\x20\x5e\x35\x20\x4a\x5d\x3b\x1b\x40\x1b\x51\x2b\x1b\x54\x4f\x5f\x12\x58\x3f\x1b\x44\x1b\x57\x25\x1b\x41\x5e\x1b\x51\x20\x1b\x52\x25\x20\x4a\x1b\x51\x20\x1b\x52\x35\x20\x12\x41\x1b\x51\x12\x41\x1b\x52\x5d\x22\x1b\x44\x1b\x51\x2a\x1b\x40\x1b\x54\x4f\x5f\x12\x51\x1b\x51\x35\x5f\x12\x44\x1b\x44\x36\x1b\x41\x1b\x57\x48\x1b\x51\x20\x12\x41\x1b\x52\x12\x42\x1b\x51\x12\x42\x1b\x52\x55\x20\x12\x41\x4a\x1b\x51\x20\x1b\x52\x5d\x2a\x1b\x54\x20\x1b\x50\x12\x4d\x40\x20\x12\x43\x5e\x34\x20\x12\x43\x1b\x44\x1b\x57\x35\x1b\x41\x4a\x1b\x51\x20\x12\x41\x1b\x52\x12\x42\x1b\x51\x12\x43\x1b\x52\x34\x20\x12\x41\x1b\x51\x12\x42\x1b\x52\x12\x41\x1b\x44\x1b\x51\x4a\x1b\x50\x20\x12\x49\x1b\x41\x40\x20\x58\x1b\x51\x20\x1b\x40\x22\x4f\x5f\x3f\x20\x2a\x5f\x3f\x1b\x55\x2b\x5f\x1b\x44\x1b\x51\x49\x1b\x41\x1b\x57\x2a\x1b\x51\x20\x12\x41\x1b\x52\x34\x20\x2a\x1b\x51\x20\x12\x41\x1b\x52\x5d\x20\x12\x41\x1b\x51\x12\x41\x1b\x52\x37\x5a\x1b\x54\x20\x1b\x50\x12\x4a\x4a\x20\x4f\x1b\x51\x20\x12\x41\x1b\x40\x5a\x5f\x35\x20\x12\x41\x5f\x3f\x2a\x5f\x12\x41\x1b\x44\x45\x1b\x41\x1b\x57\x2b\x4f\x1b\x51\x20\x1b\x52\x54\x20\x2b\x1b\x51\x20\x12\x41\x1b\x52\x30\x4a\x3f\x58\x1b\x54\x20\x1b\x50\x12\x41\x4e\x24\x20\x12\x47\x1b\x51\x12\x41\x1b\x40\x4a\x37\x23\x12\x41\x5f\x37\x21\x20\x12\x41\x2b\x25\x20\x4f\x21\x23\x1b\x44\x1b\x55\x4b\x1b\x57\x53\x1b\x52\x12\x45\x1b\x51\x12\x41\x5b\x12\x41\x1b\x52\x57\x1b\x54\x20\x1b\x40\x1b\x51\x2f\x20\x45\x1b\x57\x27\x0f";
static const unsigned short banner_len = 704;

// Tetronimos
static const uint16_t PIECES_PROGRAMME[35] PROGMEM = {
    4, 0xf0, 0x4444, 0xf00, 0x2222,
    2, 0x0f, 0x0f, 0x0f, 0x0f,
    3, 0x39, 0x96, 0x138, 0xd2,
    3, 0x3c, 0x192, 0x78, 0x93,
    3, 0x1e, 0x132, 0xf0, 0x99,
    3, 0x3a, 0xb2, 0xb8, 0x9a,
    3, 0x33, 0xb4, 0x198, 0x5a
};

void charger_pieces(struct TetrisGame* game) {
  unsigned char* data = game->memoire_pieces;
  
  for (int i = 0; i < 7; i++) {
    uint16_t taille = pgm_read_byte(PIECES_PROGRAMME + i * 5);
    game->pieces[i].taille = taille;
    
    for (int j = 0; j < 4; j++) {
      uint16_t format = pgm_read_byte(PIECES_PROGRAMME + i * 5 + j + 1);
      game->pieces[i].rotations[j] = data;

      // on lit les bits !
      for (int b = 0; b < taille * taille; b++) {
        if (format & (1<<b)) {
          *data = 1;
        } else {
          *data = 0;
        }
        data++;
      }
    }
  }
}

void dessiner_piece(struct Piece piece, bool effacer, struct Piece* ignorer) {
  // ignorer est une pièce avec une position potentiellement différente,
  // donc on doit vérifier si on ignore en vérifiant :
  // - la différence de taille
  // - la différence de position
  // - la différence de forme

  // DOIT_IGNORER(x, y)
  // x, y étant une position de notre caractère dans la pièce actuelle,
  // donc x + pos_ignorer.x et y + pos_ignorer.y doit être une position dans le caractère ignorer
  #define DOIT_IGNORER(x, y) \
    (ignorer && \
      x + pos_ignorer.x >= 0 && x + pos_ignorer.x < taille_ignorer && \
      y + pos_ignorer.y >= 0 && y + pos_ignorer.y < taille_ignorer && \
      forme_ignorer[(y + pos_ignorer.y) * taille_ignorer + (x + pos_ignorer.x)] \
    )

  // infos de la pièce
  unsigned char taille = piece.info->taille;
  unsigned char* forme = piece.info->rotations[piece.rotation];

  // les infos de ignorer, ça peut permettre d'optimiser
  unsigned char taille_ignorer;
  unsigned char* forme_ignorer;
  struct Position pos_ignorer;
  
  if (ignorer) {
    taille_ignorer = ignorer->info->taille;
    forme_ignorer = ignorer->info->rotations[ignorer->rotation];
    pos_ignorer.x = piece.position.x - ignorer->position.x;
    pos_ignorer.y = piece.position.y - ignorer->position.y;
  }
  
  bool flag;
  int x;
  
  for (int y = 0; y < taille; y++) {
    // on vérifie si il y a au moins un caractère
    flag = false;
    for (x = 0; x < taille; x++) {
      if (forme[y * taille + x] && !DOIT_IGNORER(x, y)) {
        flag = true;
        break;
      }
    }

    // pas de caractère, alors on ignore
    if (!flag) {
      continue;
    }

    deplacer_curseur(1 + TETRIS_POSITION + piece.position.x + x, 1 + piece.position.y + y);
    
    // on a forcément un caractère, alors on va dessiner
    // (le x initial a déjà été trouvé plus haut)
    for (; x < taille; x++) {
      unsigned char valeur = forme[y * taille + x];
      if (valeur) {
        if (DOIT_IGNORER(x, y))
          Serial.write(HT);
        else
          Serial.write(effacer ? 0x20 : 0x7F);

        flag = false;
        
      } else {
        if (!flag)
          break;
          
        Serial.write(HT);
      }
    }
  }
}

inline void dessiner_piece(struct Piece piece) {
  dessiner_piece(piece, false, NULL);
}

void remplacer_piece(struct Piece piece, struct Piece piece_prev) {
  dessiner_piece(piece, false, &piece_prev);
  dessiner_piece(piece_prev, true, &piece);
}

bool verifie_collision(struct TetrisGame* game, struct Piece piece) {
  unsigned char taille = piece.info->taille;
  unsigned char* forme = piece.info->rotations[piece.rotation];
  
  for (int x = 0; x < taille; x++) {
    for (int y = 0; y < taille; y++) {
      if (forme[y * taille + x]) {
        int posX = piece.position.x + x;
        int posY = piece.position.y + y;
  
        if (posY >= HAUTEUR_ECRAN)
          return true;
  
        if (posX < 0 or posX >= TETRIS_LONGUEUR)
          return true;
  
        if (game->grille[posY][posX]) {
          return true;
        }
      }
    }
  }

  return false;
}

void changer_piece(struct TetrisGame* game, struct Piece piece) {
  remplacer_piece(piece, game->piece);
  game->piece = piece;
}

void deplacer_piece(struct TetrisGame* game, bool droite) {
  struct Piece piece = game->piece;
  if (droite)
    piece.position.x++;
  else
    piece.position.x--;

  if (!verifie_collision(game, piece))
    changer_piece(game, piece);
}

void tourner_piece(struct TetrisGame* game) {
  struct Piece piece = game->piece;
  piece.rotation = (piece.rotation + 1) % 4;

  if (!verifie_collision(game, piece))
    changer_piece(game, piece);
}

void reception_caractere_tetris(struct TetrisGame* game, unsigned char code) {
  if (game->state == 0) {
    start_tetris(game);
    
  } else if (game->state == 1) {
    if (code == 0x08 || code == 'Q') { // gauche
      deplacer_piece(game, false);
        
    } else if (code == 0x09 || code == 'D') {
      deplacer_piece(game, true);
        
    } else if (code == 0x0A || code == 'S') {
      // chute rapide : à implémenter
        
    } else if (code == 0x0B || code == 'Z') {
      // tourner piece
      tourner_piece(game);
    }
  }
}

void gameover_tetris() {
  effacer_ecran();
  Serial.println("GAME OVER\n");
  delay(2000);

  // on reset
  setup();
}

void piece_suivante(struct TetrisGame* game) {
  struct Piece piece;
  piece.info = game->pieces + random(0, 7);
  piece.rotation = 0;
  piece.position.x = (TETRIS_LONGUEUR - piece.info->taille) / 2;
  piece.position.y = 0;
  
  if (verifie_collision(game, piece)) {
    gameover_tetris();
  }

  // NOTE: on ne doit PAS effacer l'ancienne piece
  game->piece = piece;
  dessiner_piece(piece);
  //changer_piece(game, piece);
}

void redessiner_jeu(struct TetrisGame* game) {
  for (int y = 0; y < HAUTEUR_ECRAN; y++) {
    deplacer_curseur(1 + TETRIS_POSITION, 1 + y);
    for (int x = 0; x < TETRIS_LONGUEUR; x++) {
      if (game->grille[y][x])
        Serial.write(0x7F);
      else
        Serial.write(0x20);
    }
  }
  // on attends
  Serial.flush();
}

bool ligne_pleine(struct TetrisGame* game, char y) {
  for (int x = 0; x < TETRIS_LONGUEUR; x++) {
    if (!game->grille[y][x]) {
      return false;
    }
  }
  return true;
}

static void dessiner_interface(unsigned char y) {
  deplacer_curseur(1 + TETRIS_POSITION - 1, 1 + y);
  Serial.write(0x7F);

  deplacer_curseur(1 + TETRIS_POSITION + TETRIS_LONGUEUR, 1 + y);
  Serial.write(0x7F);
  
}

static void dessiner_interface() {
  effacer_ecran();
  for (int y = 0; y < HAUTEUR_ECRAN; y++) {
    dessiner_interface(y);
  }

  // on attends que tout soit dessiné
  Serial.flush();
}

void verifier_lignes(struct TetrisGame* game) {
  int y = HAUTEUR_ECRAN - 1;
  
  while (y >= 0) {
    if (ligne_pleine(game, y)) {
      // La ligne est pleine, on doit donc déplacer vers le bas
      // toutes les lignes qui sont au dessus
      memmove(game->grille + 1, game->grille + 0, y * TETRIS_LONGUEUR);
      memset(game->grille, 0, TETRIS_LONGUEUR);

      // On va à la ligne en question
      deplacer_curseur(1, 1+y);

      // Et on la bazarde
      Serial.write(0x1B);
      Serial.write(0x5B);
      Serial.write(0x31);
      Serial.write(0x4D);
      
      // On va tout en haut
      Serial.write(0x1E);

      // On ajoute une ligne
      Serial.write(0x1B);
      Serial.write(0x5B);
      Serial.write(0x31);
      Serial.write(0x4C);

      // On doit aussi recréer la ligne de l'interface
      dessiner_interface(0);
      
      // (On attends quand même)
      Serial.flush();
      
    } else {
      y -= 1;
    }
  }
}

void poser_piece(struct TetrisGame* game) {
  unsigned char taille = game->piece.info->taille;
  unsigned char* forme = game->piece.info->rotations[game->piece.rotation];
  struct Position pos = game->piece.position;
  
  for (int x = 0; x < taille; x++) {
    for (int y = 0; y < taille; y++) {
      if (forme[y * taille + x]) {
        game->grille[y + pos.y][x + pos.x] = 1;
      }
    }
  }

  // est-ce qu'il y a des lignes à effacer ?
  verifier_lignes(game);
  piece_suivante(game);
}

void frame_tetris(struct TetrisGame* game) {
  game->frame = millis();
  
  // on fait une copie de la pièce qu'on déplace
  struct Piece piece = game->piece;
  piece.position.y++;

  // est-ce qu'elle touche le sol ?
  if (verifie_collision(game, piece)) {
    // on s'arrête là
    poser_piece(game);
    
  } else {
    changer_piece(game, piece);
  }
}

// lancer le jeu
void start_tetris(struct TetrisGame* game) {  
  // on initialise la grille
  for (int x = 0; x < TETRIS_LONGUEUR; x++) {
    for (int y = 0; y < HAUTEUR_ECRAN; y++) {
      game->grille[y][x] = 0;
    }
  }

  // on initialise la pièce
  piece_suivante(game);
  
  // on indique qu'on est dans l'état de jeu
  game->state = 1;
  
  // on démarre la première frame
  dessiner_interface();
  dessiner_piece(game->piece);
  
  frame_tetris(game);
}

void init_tetris(struct TetrisGame* game, struct Evenements* events) {
  reset_events(events);
  events->data = game;
  events->on_caractere = reception_caractere_tetris;

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
  charger_pieces(game);

  // bannière
  for (int i = 0; i < banner_len; i++) {
    Serial.write(pgm_read_byte(banner_data + i)); // output the buffer.
  }
}

void loop_tetris(struct TetrisGame* game) {
  if (game->state == 1) {
    if (millis() - game->frame >= 120) {
      frame_tetris(game);
    }
  }
}
