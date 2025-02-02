#include "ButtonManager.h"

// Construtor
ButtonManager::ButtonManager(int *buttonPins, int numButtons, unsigned long debounce)
{
    this->numButtons = numButtons;
    this->debounceDelay = debounce;

    // Alocar memória para os arrays
    pins = new int[numButtons];
    states = new bool[numButtons];
    lastStates = new bool[numButtons];
    lastTimes = new unsigned long[numButtons];

    // Inicializar os arrays
    for (int i = 0; i < numButtons; i++)
    {
        pins[i] = buttonPins[i];
        states[i] = false;
        lastStates[i] = false;
        lastTimes[i] = 0;
    }
}

// Inicialização dos pinos
void ButtonManager::begin()
{
    for (int i = 0; i < numButtons; i++)
    {
        pinMode(pins[i], INPUT_PULLUP); // Configura como INPUT_PULLUP
    }
}

// Atualização dos estados dos botões
void ButtonManager::update()
{
    for (int i = 0; i < numButtons; i++)
    {
        bool currentState = digitalRead(pins[i]) == LOW; // Botão pressionado (ativo em LOW)

        // Verifica debounce
        if (currentState != lastStates[i])
        {
            lastTimes[i] = millis(); // Atualiza o temporizador
        }

        if ((millis() - lastTimes[i]) > debounceDelay)
        {
            if (currentState != states[i])
            {
                states[i] = currentState; // Atualiza o estado do botão
            }
        }

        lastStates[i] = currentState; // Atualiza o estado anterior
    }
}

// Retorna o estado de um botão (true = pressionado, false = não pressionado)
bool ButtonManager::isPressed(int buttonIndex)
{
    if (buttonIndex >= 0 && buttonIndex < numButtons)
    {
        return states[buttonIndex];
    }
    return false; // Retorna false se o índice for inválido
}

// Retorna true somente quando o botão vai de LOW para HIGH
bool ButtonManager::isReleased(int buttonIndex)
{
    if (buttonIndex >= 0 && buttonIndex < numButtons)
    {
        return lastStates[buttonIndex] && !states[buttonIndex];
    }
    return false; // Retorna false se o índice for inválido
}

// Destrutor
ButtonManager::~ButtonManager()
{
    delete[] pins;
    delete[] states;
    delete[] lastStates;
    delete[] lastTimes;
}
