
#ifndef BUTTON_MANAGER_H
#define BUTTON_MANAGER_H

#include <Arduino.h>

// buttonManager class by DinossauroBebado @ 2025
// Essa classe recebe os pinos de quantos botoes estirem no projeto e de forma dinamica,
// retorna se o botão esta apertado ou não com o debounce
// sempre atualize a leitura no começo do loop
// ispressed -> retorna true enquanto o botão estiver apertado
// isrelease -> retorna true se o botao estava solto e é apertado

class ButtonManager
{
private:
    int *pins;                   // Array de pinos dos botões
    bool *states;                // Estados atuais dos botões
    bool *lastStates;            // Estados anteriores dos botões
    unsigned long *lastTimes;    // Temporizadores para debounce
    int numButtons;              // Número de botões
    unsigned long debounceDelay; // Tempo de debounce em milissegundos

public:
    // Construtor
    ButtonManager(int *buttonPins, int numButtons, unsigned long debounce = 50);

    // Inicialização dos pinos
    void begin();

    // Atualização dos estados dos botões
    void update();

    // Retorna o estado de um botão (true = pressionado, false = não pressionado)
    bool isPressed(int buttonIndex);

    // Retorna true somente quando o botão vai de LOW para HIGH
    bool isReleased(int buttonIndex);

    // Destrutor
    ~ButtonManager();
};

#endif // BUTTON_MANAGER_H