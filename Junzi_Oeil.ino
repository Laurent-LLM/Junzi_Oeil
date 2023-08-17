/* 
 * Animation de l'oeil de Junzi par une matrice de led
 * 
 *   Les mouvements de l'oeil sont contrôlés par le pavé numérique : 
 *   8 => regard en haut, 4 => regard à gauche, 6 => regard à droite, 2 => regard en bas  
 *   Un clin d'oeil est généré toute les 10 à 30 secondes
 *
 * par LLM, le 21/07/2023
 */

// Fonctions & Instructions externalisées pour Junzi
#include "Junzi_functions.h"

// Variables
long timer = 0;
long tempoCligne = 0;

// Oeil normal
byte normal[1][8] = {
  { 0b00111100, 0b01000010, 0b01000010, 0b01011010, 0b01011010, 0b01000010, 0b01000010, 0b00111100 }
};

// Gros oeil
byte gros[1][8] = {
  { 0b00111100, 0b01111110, 0b11000011, 0b11011011, 0b11011011, 0b11000011, 0b01111110, 0b00111100 }
};

// Mouvement fermeture
byte animClos[5][8] = {
  { 0b00000000, 0b00111100, 0b01000010, 0b01011010, 0b01011010, 0b01000010, 0b01000010, 0b00111100 },
  { 0b00000000, 0b00000000, 0b00111100, 0b01000010, 0b01011010, 0b01011010, 0b01000010, 0b00111100 },
  { 0b00000000, 0b00000000, 0b00000000, 0b00111100, 0b01011010, 0b01011010, 0b00111100, 0b00000000 },
  { 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00111100, 0b01011010, 0b00111100, 0b00000000 },
  { 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b01111110, 0b00111100, 0b00000000 }
};

// Mouvement vers la gauche
byte animGauche[3][8] = {
  { 0b00111100, 0b01000010, 0b01000010, 0b01110010, 0b01110010, 0b01000010, 0b01000010, 0b00111100 },
  { 0b01111000, 0b10000100, 0b10000100, 0b10110100, 0b10110100, 0b10000100, 0b10000100, 0b01111000 },
  { 0b01111000, 0b10000100, 0b10000100, 0b11100100, 0b11100100, 0b10000100, 0b10000100, 0b01111000 }
};

// Mouvement vers la droite
byte animDroite[3][8] = {
  { 0b00111100, 0b01000010, 0b01000010, 0b01001110, 0b01001110, 0b01000010, 0b01000010, 0b00111100 },
  { 0b00011110, 0b00100001, 0b00100001, 0b00101101, 0b00101101, 0b00100001, 0b00100001, 0b00011110 },
  { 0b00011110, 0b00100001, 0b00100001, 0b00100111, 0b00100111, 0b00100001, 0b00100001, 0b00011110 }
};

// Mouvement vers le haut
byte animHaut[2][8] = {
  { 0b00111100, 0b01000010, 0b01011010, 0b01011010, 0b01000010, 0b01000010, 0b01000010, 0b00111100 },
  { 0b00111100, 0b01011010, 0b01011010, 0b01000010, 0b01000010, 0b01000010, 0b01000010, 0b00111100 },
};

// Mouvement vers le bas
byte animBas[2][8] = {
  { 0b00111100, 0b01000010, 0b01000010, 0b01000010, 0b01011010, 0b01011010, 0b01000010, 0b00111100 },
  { 0b00111100, 0b01000010, 0b01000010, 0b01000010, 0b01000010, 0b01011010, 0b01011010, 0b00111100 },
};

// Smiley (:
byte smiley[1][8] = {
  { 0b00000000, 0b00100100, 0b00000000, 0b00000000, 0b01000010, 0b00100100, 0b00011000, 0b00000000 }
};

//----------------------------------------------------------
//Le programme d affichage
void setup() {
  // Pins à utiliser
  pinMode(SPI_CLK, OUTPUT);
  pinMode(SPI_CS, OUTPUT);
  pinMode(SPI_MOSI, OUTPUT);

  // Communication Série
  Serial.begin(9600);
  Serial.setTimeout(100);

  // Initialisation de la matrice de leds
  initMatrice();
  clearMatrice();

  randomSeed(A0);
  timer = millis();

  // Temporisation 1er clignement
  tempoCligne = 5000;
}


void loop() {
  // Affichage de l'oeil "normal"
  showIcon(normal);
  
  // Selon la saisie clavier (chiffres)
  switch (serialInput()) {
    case 0:
      showIcon(gros);
      delay(1000);
      break;
    case 2:
      Serial.println("En bas");
      showSequence(animBas, sizeof(animBas)/8, 1000);
      break;
    case 4:
      Serial.println("A gauche");
      showSequence(animGauche, sizeof(animGauche)/8, 1000);
      break;
    case 6:
      Serial.println("A droite");
      showSequence(animDroite, sizeof(animDroite)/8, 1000);
      break;
    case 8:
      Serial.println("En haut");
      showSequence(animHaut, sizeof(animHaut)/8, 1000);
      break;
    case 9:
      showIcon(smiley);
      delay(2000);
      break;
    default:
      break;
  }

  // Clignement épisodique (la tempo est recalculée après chaque action)
  if (millis() - timer > tempoCligne) {
    Serial.println("Clin d'oeil");
    showSequence(animClos, sizeof(animClos)/8, 150);

    timer = millis(); // Synchronisation
    tempoCligne = random(2, 7) * 5000; // temporisation aléatoire de 10 à 30 secondes, par pas de 5 secondes
    Serial.print("(le prochain dans ");
    Serial.print(tempoCligne);
    Serial.println(" s)");
  }
}
