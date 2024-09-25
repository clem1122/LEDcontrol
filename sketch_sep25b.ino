#include <Adafruit_NeoPixel.h>

// Définir le nombre de LEDs sur le bandeau
#define NUM_LEDS 300  // Remplace par le nombre de LEDs de ton bandeau

// Définir la pin à laquelle le bandeau est connecté
#define PIN 8

// Initialiser la bande LED
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

int decalage = 0;
uint32_t magenta = strip.Color(224, 155, 0);
uint32_t black = strip.Color(0, 0, 0);
uint32_t white = strip.Color(255, 255, 255);
int R;
int G;
int B;

int H = 0;
float S = 1;
float Bri = 1;
float c = 10000;
bool state = true;

void setup() {
  // Initialisation du bandeau LED
  Serial.begin(9600);
  strip.begin();
  strip.show();  // Éteindre toutes les LED au démarrage
}

void loop() {
  // Allumer toutes les LED en bleu (R, G, B -> 0, 0, 255)
  strip.fill(black);



  for (int i = c; i < c + 10; i++) {
    for (int d = 0; d < 300; d += 25) {
      HSBtoRGB(H - d, S, Bri, R, G, B);
      magenta = strip.Color(R, G, B);


      strip.setPixelColor((i - d) % NUM_LEDS, magenta);
    }
  }

  strip.show();  // Envoyer les couleurs mises à jour au bandeau
  //delay(30);
  H = (H + 6) % 360;
  c = c - 1;
  state = !state;
  //Bri = (cos(c)+1)/2;
  // Attendre une seconde
}

// Fonction pour convertir HSV en RGB
void HSBtoRGB(float H, float S, float B, int &r, int &g, int &b) {
  float C = B * S;
  float X = C * (1 - abs(fmod(H / 60.0, 2) - 1));
  float m = B - C;

  float r1, g1, b1;

  if (H >= 0 && H < 60) {
    r1 = C;
    g1 = X;
    b1 = 0;
  } else if (H >= 60 && H < 120) {
    r1 = X;
    g1 = C;
    b1 = 0;
  } else if (H >= 120 && H < 180) {
    r1 = 0;
    g1 = C;
    b1 = X;
  } else if (H >= 180 && H < 240) {
    r1 = 0;
    g1 = X;
    b1 = C;
  } else if (H >= 240 && H < 300) {
    r1 = X;
    g1 = 0;
    b1 = C;
  } else {
    r1 = C;
    g1 = 0;
    b1 = X;
  }

  // Convertir les valeurs de [0,1] à [0,255] et ajuster avec m
  r = (r1 + m) * 255;
  g = (g1 + m) * 255;
  b = (b1 + m) * 255;
}