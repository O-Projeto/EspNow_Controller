#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "espdata.h"

#define OLED_ADDR 0x3C
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

class screen
{
private:
    Adafruit_SSD1306 display;

public:
    screen();
    ~screen();
    void begin();
    void test(const espdata &controler);
    void update();
    void drawCheckbox(int x, int y, bool checked);
    void drawCircle(int x, int y);
};

screen::screen() : display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1) {}

screen::~screen() {}

void screen::begin()
{
    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR))
    {
        Serial.println(F("Falha ao iniciar o display OLED!"));
        return;
    }
    display.clearDisplay();
    display.display(); // Garante que o display está limpo antes de iniciar
}

void screen::drawCircle(int x, int y)
{
    display.fillCircle(30 + x, 32 + y, 3, WHITE); // Ajustando deslocamento correto
}

void screen::drawCheckbox(int x, int y, bool checked)
{
    if (checked)
    {
        display.fillCircle(x, y, 5, WHITE); // Caixa preenchida
    }
    else
    {
        display.drawCircle(x, y, 5, WHITE); // Caixa vazia
    }
}

void screen::test(const espdata &controler)
{
    // Normalização dos valores de X e Y para o range -15 a 15 e inversão do X
    int xNorm = -map(controler.joy_data.y, -1000, 1000, -15, 15); // Rotação e inversão
    int yNorm = -map(controler.joy_data.x, -1000, 1000, 15, -15); // Rotação

    // Calcular a distância radial limitada ao círculo
    float radius = sqrt(xNorm * xNorm + yNorm * yNorm);
    if (radius > 15)
    {
        float scale = 15.0 / radius;
        xNorm *= scale;
        yNorm *= scale;
    }

    // Limpar o display e atualizar para evitar flickering
    display.clearDisplay();

    // Desenhar o círculo fixo no centro do controle
    display.drawCircle(30, 32, 15, WHITE); // Círculo fixo representando a trajetória

    // Exibir os checkboxes como cruz à direita do círculo
    drawCheckbox(98, 17, controler.arrow_data.up);
    drawCheckbox(98, 47, controler.arrow_data.down);
    drawCheckbox(83, 32, controler.arrow_data.left);
    drawCheckbox(113, 32, controler.arrow_data.right);

    // Desenhar uma linha vertical no centro X da tela
    for (int y = 0; y < SCREEN_HEIGHT; y++)
    {
        display.drawPixel(64, y, WHITE);
    }

    // Desenhar uma linha do centro X até o valor máximo em Y
    display.drawLine(64, 32, 64, SCREEN_HEIGHT - 1, WHITE);

    // Exibir o círculo indicando a direção do controle analógico
    drawCircle(xNorm, yNorm);

    // Atualizar o display
    update();
}

void screen::update()
{
    display.display();
}
