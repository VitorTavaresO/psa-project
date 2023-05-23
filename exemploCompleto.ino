#include <Adafruit_NeoPixel.h>

#define NEOPIXEL_BOTAO_PIN 6
#define NUMPIXELS_BOTAO 4
#define MOTOR_DIR_PIN 8
#define BOTAO_PIN 2
Adafruit_NeoPixel pixelsBotao(NUMPIXELS_BOTAO, NEOPIXEL_BOTAO_PIN, NEO_GRB + NEO_KHZ800);

#define NEOPIXEL_POTENCIOMETRO_PIN 7
#define NUMPIXELS_POTENCIOMETRO 12
#define MOTOR_PWM_PIN 9
#define POTENCIOMETRO_PIN A0

Adafruit_NeoPixel pixelsPotenciometro(NUMPIXELS_POTENCIOMETRO, NEOPIXEL_POTENCIOMETRO_PIN, NEO_GRB + NEO_KHZ800);

void setup()
{
    pinMode(BOTAO_PIN, INPUT);
    pinMode(POTENCIOMETRO_PIN, INPUT);
    pinMode(MOTOR_PWM_PIN, OUTPUT);
    pinMode(MOTOR_DIR_PIN, OUTPUT);
    pixelsBotao.begin();
    pixelsPotenciometro.begin();
}

void loop()
{
    int estadoBotao = digitalRead(BOTAO_PIN);
    int potenciometroValor = analogRead(POTENCIOMETRO_PIN);
    int velocidadeMotor = map(potenciometroValor, 0, 1023, 0, 255);
    int numeroLedsAcender = map(velocidadeMotor, 0, 255, 0, NUMPIXELS_POTENCIOMETRO);

    /////////////////////////////////////////////////////////
    // ESTRUTURA DO BOTÃO
    ////////////////////////////////////////////////////////

    if (estadoBotao == HIGH)
    {
        digitalWrite(MOTOR_DIR_PIN, HIGH);
        // analogWrite(MOTOR_PWM_PIN, 255); // Controle de velocidade em PWM
        pixelsBotao.setBrightness(255); // Brilho máximo
        for (int i = 0; i < NUMPIXELS_BOTAO; i++)
        {
            pixelsBotao.setPixelColor(i, pixelsBotao.Color(0, 255, 0)); // Cor verde
        }
        pixelsBotao.show();
    }
    else
    {
        pixelsBotao.clear();
        pixelsBotao.show();
    }

    /////////////////////////////////////////////////////////
    // ESTRUTURA DO POTENCIÔMETRO
    ////////////////////////////////////////////////////////

    digitalWrite(MOTOR_DIR_PIN, LOW);
    analogWrite(MOTOR_PWM_PIN, velocidadeMotor); // Velocidade baseada no potenciômetro
    pixelsPotenciometro.setBrightness(255);      // Brilho máximo
    for (int i = 0; i < NUMPIXELS_POTENCIOMETRO; i++)
    {
        if (i < numeroLedsAcender)
        {
            pixelsPotenciometro.setPixelColor(i, pixelsPotenciometro.Color(255, 0, 0)); // Cor vermelha
        }
        else
        {
            pixelsPotenciometro.setPixelColor(i, pixelsPotenciometro.Color(0, 0, 0)); // LED desligado
        }
    }
    pixelsPotenciometro.show();
}