#include <Arduino.h>
#include "minitel.h"

// fonctions externes
extern void setup_minitel();
extern void loop_minitel();

// fonctions internes (mais référencées plus loin)
void lire_minitel();
void minitel_ping();

// variables
struct Evenements minitel_events;
static unsigned long minitel_dernier_ping;
static unsigned long minitel_timeout;
static unsigned int minitel_vitesse;
static bool minitel_pret;

//
// connexion
//
void tentative_connexion_minitel() {
  Serial.flush();
  Serial.begin(minitel_vitesse, SERIAL_7E1);
  minitel_ping();
}

void initialiser_minitel() {
  minitel_pret = false;
  minitel_vitesse = 1200;
  minitel_dernier_ping = 0;
  minitel_timeout = 0;

  reset_events(&minitel_events);
  tentative_connexion_minitel();
}

void minitel_ping() {
  demande_status_terminal();
  minitel_dernier_ping = millis();
  
  if (minitel_pret) {
    minitel_timeout = minitel_dernier_ping + 1000;
  } else {
    minitel_timeout = minitel_dernier_ping + 250;
  }
}

void minitel_pong() {
  // plus de timeout, on a eu notre pong
  minitel_timeout = 0;
  
  if (!minitel_pret) {
    if (minitel_vitesse == 4800) {
      minitel_pret = true;
      setup_minitel();
          
    } else {
      // on tente de basculer en 4800
      protocole(PROG, 0x76);
      minitel_vitesse = 4800;
      tentative_connexion_minitel();
    }
  }
}

void boucle_minitel() {
  lire_minitel();
    
  if (!minitel_pret) {    
    if (millis() > minitel_timeout) {
      // on passe à la vitesse suivante
      switch (minitel_vitesse) {
        case 1200:
          minitel_vitesse = 4800;
          break;
    
        case 4800:
          minitel_vitesse = 300;
          break;
          
        case 300:
          minitel_vitesse = 1200;
          break;
      }
      tentative_connexion_minitel();
    }
    
  } else {
    // connexion perdue ou pas ?
    if (minitel_timeout) {
      if (millis() > minitel_timeout) {
        initialiser_minitel();
        return;
      }
    } else {
      if (millis() - minitel_dernier_ping >= 1000) {
        minitel_ping();
      }
    }
    loop_minitel();
  }
}

//
// fonctions de base
//
void deplacer_curseur(unsigned char x, unsigned char y) {
  Serial.write(0x1F);
  Serial.write(0x40 + y);
  Serial.write(0x40 + x);
}

void effacer_ecran() {
  Serial.write(0x0C);
}

//
// base protocole
//
void protocole(unsigned char code1) {
  Serial.write(ESC);
  Serial.write(PRO1);
  Serial.write(code1);
}

void protocole(unsigned char code1, unsigned char code2) {
  Serial.write(ESC);
  Serial.write(PRO2);
  Serial.write(code1);
  Serial.write(code2);
}

void protocole(unsigned char code1, unsigned char code2, unsigned char code3) {
  Serial.write(ESC);
  Serial.write(PRO3);
  Serial.write(code1);
  Serial.write(code2);
  Serial.write(code3);
}

//
// fonctions protocole
//
void demande_status_terminal() {
  protocole(DEMANDE_STATUS_TERMINAL);
}

void fonction_start(enum CodeReception code1, unsigned char code2) {
  protocole(START, code1, code2);
}

void fonction_stop(enum CodeReception code1, unsigned char code2) {
  protocole(STOP, code1, code2);
}

void aiguillage_on(enum CodeReception code1, enum CodeEmission code2) {
  protocole(ON, code1, code2);
}

void aiguillage_off(enum CodeReception code1, enum CodeEmission code2) {
  protocole(OFF, code1, code2);
}

void reset_minitel() {
  protocole(RESET);
}

//
// reception
//
unsigned char lire_octet() {
  while (!Serial.available()) {
    delay(1);
  }

  unsigned char val = Serial.read();
  return val;
}

void reception_protocole(unsigned char code1) {
  if (minitel_pret && minitel_events.on_protocole_1) {
    minitel_events.on_protocole_1(minitel_events.data, code1);
  }
}

void reception_protocole(unsigned char code1, unsigned char code2) {
  if (code1 == REP_STATUS_TERMINAL) {
    minitel_pong();
    return;
  }
  if (minitel_pret && minitel_events.on_protocole_2) {
    minitel_events.on_protocole_2(minitel_events.data, code1, code2);
  }
}

void reception_protocole(unsigned char code1, unsigned char code2, unsigned char code3) {
  if (minitel_pret && minitel_events.on_protocole_3) {
    minitel_events.on_protocole_3(minitel_events.data, code1, code2, code3);
  }
}

void reception_separateur(unsigned char code) {
  if (minitel_pret && minitel_events.on_separateur) {
    minitel_events.on_separateur(minitel_events.data, code);
  }
}

void reception_caractere(unsigned char code) {
  if (minitel_pret && minitel_events.on_caractere) {
    minitel_events.on_caractere(minitel_events.data, code);
  }
}

void reception_escape(unsigned char code) {
  if (minitel_pret && minitel_events.on_escape) {
    minitel_events.on_escape(minitel_events.data, code);
  }
}

void lire_minitel() {
  if (Serial.available()) {
    unsigned char val = lire_octet();
    
    if (val == ESC) { // ESC
      val = lire_octet();
      
      if (val == PRO1) {
        unsigned char code1 = lire_octet();
        reception_protocole(code1);
        
      } else if (val == PRO2) {
        unsigned char code1 = lire_octet();
        unsigned char code2 = lire_octet();
        reception_protocole(code1, code2);
        
      } else if (val == PRO3) {
        unsigned char code1 = lire_octet();
        unsigned char code2 = lire_octet();
        unsigned char code3 = lire_octet();
        reception_protocole(code1, code2, code3);
        
      } else {
        reception_escape(val);
      }
    
    } else if (val == SEP) { // SEP
      val = lire_octet();
      reception_separateur(val);
      
    } else {
      reception_caractere(val);
    }
  }
}
