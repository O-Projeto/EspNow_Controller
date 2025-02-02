#include "ControlerConfig.h"
#include "config.h"

// Controler representa toda leitura de inputs
//

#define DEBUG // descomentar essa linha em caso de uso em produção

void setup()
{
#ifdef DEBUG
    Serial.begin(115200);

#endif // DE
    ControlerBegin();
}

void loop()
{
    getControlerData();

#ifdef DEBUG

    debug();
#endif // DEBUG

    delay(10); // Pequeno atraso para reduzir o uso da CPU
}
