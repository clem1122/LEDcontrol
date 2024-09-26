#include <Adafruit_NeoPixel.h>

// Définir le nombre de LEDs sur le bandeau
#define NUM_LEDS 300  // Remplace par le nombre de LEDs de ton bandeau

// Définir la pin à laquelle le bandeau est connecté
#define PIN 8
#define sec() millis()/1000;

// Initialiser la bande LED
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

uint32_t magenta = strip.Color(255, 0, 255);
uint32_t red = strip.Color(255, 0, 0);
uint32_t blue = strip.Color(0, 0, 255);
uint32_t black = strip.Color(0, 0, 0);
uint32_t white = strip.Color(255, 255, 255);

int R;
int G;
int B;

float H;
float S;
float Bri;

int counter = 0;

void rainbow(float);
void droplets(float, int, int, int);

void setup() {
  // Initialisation du bandeau LED
  Serial.begin(9600);
  strip.begin();
  strip.fill(black);
  strip.show();  // Éteindre toutes les LED au démarrage
}


void loop() {

  droplets(20, 20, 30, 4);
  strip.show();
  
}

void droplets(float freq, int length = 10, int space = 50, int fading = 4){
  long time = freq * sec();
  for (int i = 0; i < NUM_LEDS; i+=space){
    for (int j = 0; j < length; j++) {
      HSBtoRGB(180, 1, pow(((1.0f/length)*j),fading), R, G, B);
      strip.setPixelColor(fmod(i+j+time, NUM_LEDS), strip.Color(R, G, B));
    }
  }
}

void rainbow(float start_hue = 0){
  for (int i = 0; i < NUM_LEDS; i++){
    HSBtoRGB(fmod(start_hue + (i * 360.0f / NUM_LEDS), 360) , 1, 1, R, G, B);
    strip.setPixelColor(i, strip.Color(R, G, B));
  }
}

void mooving_rainbow(float freq, float start_hue) {
  long time = freq * sec();
  rainbow(start_hue + time);

}

void fill(int red, int green, int blue) {
    strip.fill(strip.Color(red, green, blue));
  
}

void fill(uint32_t color) {
  strip.fill(color);
}

/*void breathe(float period, ) {

}*/
// Fonction pour convertir HSV(360,1,1) en RGB(255, 255, 255)
#include <math.h>  // Pour utiliser fabs() et fmod()

void HSBtoRGB(float H, float S, float B, int &r, int &g, int &b) {
  float C = B * S;  // Chroma
  float X = C * (1 - fabs(fmod(H / 60.0, 2) - 1));  // Composante intermédiaire
  float m = B - C;  // Ajustement avec la luminosité minimale

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
  r = round((r1 + m) * 255);
  g = round((g1 + m) * 255);
  b = round((b1 + m) * 255);
}
