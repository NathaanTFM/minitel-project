#pragma once
#include <stddef.h>

//
// Codes ASCII utiles
//
#define BS 0x08
#define HT 0x09
#define LF 0x0A
#define VT 0x0B
#define SEP 0x13
#define SUB 0x1A
#define ESC 0x1B
#define US 0x1F

//
// Suivis de ESC
//
#define PRO1 0x39
#define PRO2 0x3A
#define PRO3 0x3B

//
// Quelques commandes utiles
//
#define OFF 0x60
#define ON 0x61
#define START 0x69
#define STOP 0x6A
#define PROG 0x6B
#define DEMANDE_STATUS_TERMINAL 0x70
#define REP_STATUS_TERMINAL 0x71
#define RESET 0x7F

//
// Configurations clavier
//
#define ETEN 0x41
#define C0 0x43

//
// Quelques constantes utiles
//
#define LONGUEUR_ECRAN 40
#define HAUTEUR_ECRAN 24

//
// Enums
//
enum CodeEmission {
  CE_ECRAN = 0x50,
  CE_CLAVIER = 0x51,
  CE_MODEM = 0x52,
  CE_PRISE = 0x53
};

enum CodeReception {
  CR_ECRAN = 0x58,
  CR_CLAVIER = 0x59,
  CR_MODEM = 0x5A,
  CR_PRISE = 0x5B
};

//
// Interface pour evenements
//
typedef void (*EvtFunc1)(void*, unsigned char);
typedef void (*EvtFunc2)(void*, unsigned char, unsigned char);
typedef void (*EvtFunc3)(void*, unsigned char, unsigned char, unsigned char);

struct Evenements {
  void* data;
  EvtFunc1 on_protocole_1;
  EvtFunc2 on_protocole_2;
  EvtFunc3 on_protocole_3;
  EvtFunc1 on_separateur;
  EvtFunc1 on_caractere;
  EvtFunc1 on_escape;
};

inline void reset_events(struct Evenements* events) {
  events->data = NULL;
  events->on_protocole_1 = NULL;
  events->on_protocole_2 = NULL;
  events->on_protocole_3 = NULL;
  events->on_separateur = NULL;
  events->on_caractere = NULL;
  events->on_escape = NULL;
}

//
// Fonctions publiques
//
void initialiser_minitel();
void boucle_minitel();

void deplacer_curseur(unsigned char x, unsigned char y);
void effacer_ecran();

void protocole(unsigned char code1);
void protocole(unsigned char code1, unsigned char code2);
void protocole(unsigned char code1, unsigned char code2, unsigned char code3);

void demande_status_terminal();
void fonction_start(enum CodeReception code1, unsigned char code2);
void fonction_stop(enum CodeReception code1, unsigned char code2);
void aiguillage_on(enum CodeReception code1, enum CodeEmission code2);
void aiguillage_off(enum CodeReception code1, enum CodeEmission code2);
void reset_minitel();
