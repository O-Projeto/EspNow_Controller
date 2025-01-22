#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Definição das portas
#define DIGITAL_PIN_1 19
#define DIGITAL_PIN_2 18
#define DIGITAL_PIN_3 5
#define DIGITAL_PIN_4 17
#define ANALOG_X 27
#define ANALOG_Y 26

#define OLED_ADDR   0x3C
Adafruit_SSD1306 display(128, 64, &Wire, -1);

void setup() {
  // Configuração das portas digitais como entrada
  pinMode(DIGITAL_PIN_1, INPUT);
  pinMode(DIGITAL_PIN_2, INPUT);
  pinMode(DIGITAL_PIN_3, INPUT);
  pinMode(DIGITAL_PIN_4, INPUT);

  // Inicialização da comunicação serial
  Serial.begin(115200);
  Serial.println("Iniciando leitura das portas...");

  // Inicialização do display OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println("Falha ao inicializar o display OLED!");
    while (true); // Para a execução se o display não for inicializado
  }

  display.clearDisplay();
}

void drawCircle(int x, int y) {
  display.fillCircle(30 + y, 32 + x, 3, WHITE); // Desenhar um pequeno círculo deslocado
}

void drawCheckbox(int x, int y, bool checked) {
  if (checked) {
    display.fillCircle(x, y, 5, WHITE); // Caixa preenchida
  } else {
    display.drawCircle(x, y, 5, WHITE); // Caixa vazia
  }
}

void loop() {
  // Leitura das portas digitais
  int digitalValue1 = digitalRead(DIGITAL_PIN_1);
  int digitalValue2 = digitalRead(DIGITAL_PIN_2);
  int digitalValue3 = digitalRead(DIGITAL_PIN_3);
  int digitalValue4 = digitalRead(DIGITAL_PIN_4);

  // Leitura dos valores analógicos para o controle
  int xValue = analogRead(ANALOG_X);
  int yValue = analogRead(ANALOG_Y);

  // Normalização dos valores de X e Y para o range -15 a 15 e inversão do X
  int xNorm = -map(yValue, 0, 4095, -15, 15); // Rotação do controle e inversão
  int yNorm = -map(xValue, 0, 4095, 15, -15); // Rotação do controle

  // Calcular a distância radial limitada ao círculo
  float radius = sqrt(xNorm * xNorm + yNorm * yNorm);
  if (radius > 15) {
    float scale = 15 / radius;
    xNorm *= scale;
    yNorm *= scale;
  }

  // Limpar o display
  display.clearDisplay();

  // Desenhar o círculo fixo no centro do controle
  display.drawCircle(30, 32, 15, WHITE); // Círculo fixo representando a trajetória

  // Exibir os checkboxes como cruz à direita do círculo
  drawCheckbox(98, 17, !digitalValue1); // Superior (19)
  drawCheckbox(98, 47, !digitalValue4); // Inferior (17)
  drawCheckbox(83, 32, !digitalValue2); // Esquerda (18)
  drawCheckbox(113, 32, !digitalValue3); // Direita (5)

  // Desenhar uma linha vertical no centro X da tela em cinza
  for (int y = 0; y < 64; y++) {
    display.drawPixel(64, y, WHITE); // Substitua por uma cor diferente se o display suportar
  }

  // Desenhar uma linha do centro X até o valor máximo em Y
  display.drawLine(64, 32, 64, 63, WHITE); // Substitua por uma cor diferente se o display suportar

  // Exibir o círculo indicando a direção do controle analógico
  drawCircle(xNorm, yNorm);

  // Atualizar o display
  display.display();

  // Pausar por 100 ms
  delay(10);
}
