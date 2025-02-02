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
    void main_menu(int index);
    void conect_menu(uint8_t baseMac[6]);
    void drawBattery(int x, int y, int level);
    void info_screen(int battery1, int battery2, int x, int y, int z);
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

void screen::drawBattery(int x, int y, int level)
{
    int width = 20, height = 10;

    // Desenha a borda da bateria
    display.drawRect(x, y, width, height, SSD1306_WHITE);
    display.fillRect(x + width, y + 3, 2, 4, SSD1306_WHITE); // Terminal da bateria

    // Desenha o nível de carga interno
    int fillWidth = (level * (width - 2)) / 100; // Escala para a largura da bateria
    display.fillRect(x + 1, y + 1, fillWidth, height - 2, SSD1306_WHITE);
}

void screen::info_screen(int battery1, int battery2, int x, int y, int z)
{
    display.clearDisplay();

    // Parte superior: Desenhar baterias e seus níveis (%)
    drawBattery(10, 5, battery1);
    display.setCursor(35, 7);
    display.setTextSize(1);
    display.print(battery1);
    display.print("%");

    drawBattery(80, 5, battery2);
    display.setCursor(105, 7);
    display.print(battery2);
    display.print("%");

    // Meio da tela: Exibir valores de X, Y, Z
    display.setCursor(10, 25);
    display.setTextSize(1);
    display.print("X: ");
    display.print(x);

    display.setCursor(10, 40);
    display.print("Y: ");
    display.print(y);

    display.setCursor(10, 55);
    display.print("Z: ");
    display.print(z);

    // Parte inferior: Status
    display.setCursor(80, 55);
    display.setTextSize(1);
    display.print("Status");
}

void screen::main_menu(int index_menu)
{
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE); // Define a cor do texto

    // Exibir os subtópicos
    display.setTextSize(2);
    display.setCursor(10, 5);
    display.print("Menu:");

    display.setTextSize(1);
    display.setCursor(10, 25);

    if (index_menu == 0)
    {
        display.print(">>> Config");
    }
    else
    {
        display.print("> Config");
    }
    display.setCursor(10, 40);
    if (index_menu == 1)
    {
        display.print(">>> Conexao");
    }
    else
    {
        display.print("> Conexao");
    }

    display.setCursor(10, 55);

    if (index_menu == 2)
    {
        display.print(">>>Test");
    }
    else
    {
        display.print("> Test");
    }
}

void screen::conect_menu(uint8_t baseMac[6])
{
    display.clearDisplay();
    display.setCursor(10, 40);
    char macStr[18]; // "XX:XX:XX:XX:XX:XX" + null terminator
    snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
             baseMac[0], baseMac[1], baseMac[2],
             baseMac[3], baseMac[4], baseMac[5]);

    display.print(macStr);
}

void screen::update()
{
    display.display();
}
