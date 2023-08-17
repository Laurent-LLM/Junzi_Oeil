/* 
 * Déclaration des prototypes de fonctions pour Junzi
 * 
 * par LLM, le 21/07/2023
 */

 // Cablage de la matrice en SPI (hors gnd et +V)
#define SPI_CLK  13  // SCK  (Clock)
#define SPI_MOSI 11  // MOSI (Master Out, Slave In)
#define SPI_CS   10  // SS   (Slave Select)

// Pour la matrice de leds
void initMatrice();
void clearMatrice();
void showIcon(byte data[1][8]);
void showSequence(byte data[][8], int nbElt, int pause);

// Pour les évènements
int serialInput(); // saisie dans la console (0 à 9)