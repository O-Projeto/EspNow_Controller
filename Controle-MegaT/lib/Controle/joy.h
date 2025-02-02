#include <Arduino.h>
#ifndef JOY
#define JOY
// JOY class by DinossauroBebado @ 2025
// Classe feita para leitura de um joystick, objetivo de tratar os maiores problemas conhecidos
// do joystick afim ter um controle mais preciso ,
// dead zone o joystick não volta exatamente para o zero ficando sempre em um intervalo conhecido e constante,
// que tende a aumentar com o tempo, se o robo estiver andando sozinho mesmo sem ninguem segurando o controle tem
// altas chances desse ser o problema
// rampa : as vezes não é interessante a velocidade de um robo ir de 0 a 100 de forma instanea, sendo nessario ter
// uma aceleração gradual a rampa faz essa função, quanto maior o valor mais rapido é essa aceleração
// range ou sensibilidade qual é a relação entre o movimento do joystick e do robo para um controle mais fino aumente
// o valor e para movimentos mais brutos e rapido diminiua

struct joyData
{
    int x;
    int y;
    bool sw;
};

class joy
{
private:
    int _deadZone = 30;
    int _rampaAcc = 0;
    int _range = 1000;
    joyData joy_data;
    int readX, readY, readSw;
    int PIN_X, PIN_Y, PIN_SW;
    int rangeConverter(int data);

public:
    joy(int pin_x, int pin_y, int pin_sw);
    void begin();
    joyData getJoyData();
    int deadZoneFilter(int data);
    void setDeadZone(int dead_zone);
    void setRampaAcc(int rampaAcc);
    void setRange(int range);
};

#endif // JOY