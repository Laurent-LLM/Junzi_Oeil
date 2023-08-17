/* 
 * Fonctions et Instructions pour Junzi
 * 
 * par LLM, le 21/07/2023
 */

#include "Arduino.h"
#include "Junzi_functions.h"

/* *****
 * Fonctions "privées" : utilisées en interne
 */

// Ecriture dans le driver MAX7219
// (un octet, par décalage de bits)
void writeMax7219(char data) {
  for (int i = 1; i <= 8; i++) {
    digitalWrite(SPI_CLK, LOW);
    digitalWrite(SPI_MOSI, data & 0x80);
    data = data << 1;
    digitalWrite(SPI_CLK, HIGH);
  }
}

// Ecriture d'une donnée dans une matrice de leds 
// (0x01 à 0x08 => registre de ligne, 0x0A à 0x0F => registre de paramètre)
void writeMatrice(char registre, char valeur) {
  digitalWrite(SPI_CS, LOW);
  writeMax7219(registre);  // registre Max7219
  writeMax7219(valeur);    // valeur
  digitalWrite(SPI_CS, HIGH);
}

/* ****
 * Fonctions "publiques" : disponibles pour le code .ino
 */

// Initialisation d'une matrice de leds (module Max7219)
void initMatrice() {
  //           reg.   Val.
  writeMatrice(0x09, 0x00);  // Mode de décodage : DCB
  writeMatrice(0x0a, 0x03);  // Luminosité : 0x00 à 0x0F
  writeMatrice(0x0b, 0x07);  // Nombre de leds par ligne : 8 LEDs
  writeMatrice(0x0c, 0x01);  // Mode veille : 0x00 = En veille, 0x01 = Actif
  writeMatrice(0x0f, 0x00);  // Mode test : 0x01 = ON, 0x00 = OFF
}

// Eteindre toutes les leds de la matrice
void clearMatrice() {
  // Remettre à zéro les registres de ligne de la matrice
  for (int reg = 0x01; reg <= 0x08; reg++) writeMatrice(reg, 0x00);
}

// Affichage d'une icône dans la matrice
void showIcon(byte data[1][8]){
    for (int lg = 0; lg < 8; lg++) {
      writeMatrice(lg + 1, data[0][lg]);
    }
}

// Affichage d'une suite d'icônes formant une séquence
void showSequence(byte data[][8], int nbElt, int pause){
  int max = nbElt -1;

  for (int icon = 0; icon <= max; icon++) {
   for (int lg = 0; lg < 8; lg++) {
      writeMatrice(lg + 1, data[icon][lg]);
    }
   delay(35);
  }
  delay(pause);
  for (int icon = max; icon >= 0; icon--) {
    for (int lg = 0; lg < 8; lg++) {
      writeMatrice(lg + 1, data[icon][lg]);
    }
    delay(35);
  }
}

// Réception Port Série
int serialInput() {
  if (Serial.available()) {
    String saisie = Serial.readString();
    saisie.replace("\n", "");
    saisie.replace("\r", "");
    Serial.print("saisie = ");
    Serial.print(saisie.toInt());
    Serial.print(" => ");
    return saisie.toInt();
  } else {
    return 5;
  }
}

