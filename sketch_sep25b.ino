#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>
// Définir le nombre de LEDs sur le bandeau
#define NUM_LEDS 300  // Remplace par le nombre de LEDs de ton bandeau

SoftwareSerial BT(10, 11);  //RX, TX

// Définir la pin à laquelle le bandeau est connecté
#define PIN 8
#define BT_PIN 9
#define sec() millis() / 1000.0;

// Initialiser la bande LED
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

uint32_t magenta = strip.Color(255, 0, 255);
uint32_t red = strip.Color(255, 0, 0);
uint32_t blue = strip.Color(0, 0, 255);
uint32_t black = strip.Color(0, 0, 0);
uint32_t white = strip.Color(255, 255, 255);
uint32_t current_color;

int R;
int G;
int B;

float H;
float S;
float Bri;

int mode = 0;

int counter = 0;

void rainbow(float);
void droplets(float, int, int, int);

void setup() {
  // Initialisation du bandeau LED
  digitalWrite(BT_PIN, HIGH);

  Serial.begin(9600);
  BT.begin(9600);

  strip.begin();
  strip.fill(black);
  strip.show();  // Éteindre toutes les LED au démarrage
}


void loop() {


  if (BT.available() > 0) {
    int commande = BT.read();
    mode_selection(commande);
  }


  switch (mode) {
    case 0:
      strip.clear();
      break;
    case 1:
      rainbow(0);
      break;
    case 2:
      moving_rainbow(20, 0);
      break;
    case 3:
      droplets(20, 20, 30, 4);
      break;
    case 4:
      breathe(0.5, current_color);
      break;
    default:
      break;
  }

  strip.show();
  delay(10);
}

void mode_selection(int commande) {
  strip.setBrightness(255);
  switch (commande) {
    case 48:  //0
      Serial.println("OFF");
      mode = 0;
      break;
    case 82:  //R
      Serial.println("Rainbow");
      mode = 1;
      break;
    case 77:  //M
      Serial.println("Moving Rainbow");
      mode = 2;
      break;
    case 68:  // D
      Serial.println("Droplets");
      mode = 3;
      break;
    case 66: // B
      current_color = strip.getPixelColor(0);
      mode = 4;
    default:
      Serial.print("Unknown command");
      Serial.println(commande);
      break;
  }
}


void bluetooth_RGB() {
  static String command = "";
  while (BT.available() > 0) {
    char incomingByte = BT.read();
    if (incomingByte == '\n') {  // Détecter la fin de la commande
      command.trim();            // Retirer les espaces inutiles
      if (command.length() == 9) {
        int R = command.substring(0, 3).toInt();
        int G = command.substring(3, 6).toInt();
        int B = command.substring(6, 9).toInt();

        Serial.print("R: ");
        Serial.println(R);
        Serial.print("G: ");
        Serial.println(G);
        Serial.print("B: ");
        Serial.println(B);

        strip.fill(strip.Color(R, G, B));

      } else {
        Serial.print("Error : ");
        Serial.println(command);
      }
      command = "";  // Réinitialiser la commande pour la prochaine lecture
    } else if(incomingByte == 'B') {
      mode = 0;
    }
    else {
      command += incomingByte;  // Ajouter le caractère à la commande
    }
  }
}

void droplets(float freq, int length = 10, int space = 50, int fading = 4) {
  long time = freq * sec();
  for (int i = 0; i < NUM_LEDS; i += space) {
    for (int j = 0; j < length; j++) {
      HSBtoRGB(180, 1, pow(((1.0f / length) * j), fading), R, G, B);
      strip.setPixelColor(fmod(i + j + time, NUM_LEDS), strip.Color(R, G, B));
    }
  }
}

void rainbow(float start_hue = 0) {
  for (int i = 0; i < NUM_LEDS; i++) {
    HSBtoRGB(fmod(start_hue + (i * 360.0f / NUM_LEDS), 360), 1, 1, R, G, B);
    strip.setPixelColor(i, strip.Color(R, G, B));
  }
}

void moving_rainbow(float freq, float start_hue) {
  long time = freq * sec();
  Serial.println(time);
  rainbow(start_hue + time);
}

void fill(int red, int green, int blue) {
  strip.fill(strip.Color(red, green, blue));
}

void fill(uint32_t color) {
  strip.fill(color);
}

void breathe(float freq, uint32_t color) {
    long time = freq * sec();
    strip.fill(color);
    uint8_t brightness = 255 * int((cos(2*PI*freq*time)+1)/2);
    strip.setBrightness(brightness); //(cos(2*PI*freq*time)+1)
}

// Fonction pour convertir HSV(360,1,1) en RGB(255, 255, 255)
#include <math.h>  // Pour utiliser fabs() et fmod()

void HSBtoRGB(float H, float S, float B, int &r, int &g, int &b) {
  float C = B * S;                                  // Chroma
  float X = C * (1 - fabs(fmod(H / 60.0, 2) - 1));  // Composante intermédiaire
  float m = B - C;                                  // Ajustement avec la luminosité minimale

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
