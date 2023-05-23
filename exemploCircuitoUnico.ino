#include <Adafruit_NeoPixel.h>

#define BOTAO_PIN 2
#define POTENCIOMETRO_PIN A0
#define NEOPIXEL_PIN 7
#define NUMPIXELS 12
#define MOTOR_PWM_PIN 3

Adafruit_NeoPixel pixels(NUMPIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

void setup()
{
    pinMode(BOTAO_PIN, INPUT);
    pinMode(POTENCIOMETRO_PIN, INPUT);
    pinMode(MOTOR_PWM_PIN, OUTPUT);
    pixels.begin();
}

void loop()
{
    int estadoBotao = digitalRead(BOTAO_PIN);
    int potenciometroValor = analogRead(POTENCIOMETRO_PIN);
    int velocidadeMotor = map(potenciometroValor, 0, 1023, 0, 255);
    int numeroLedsAcender = map(velocidadeMotor, 0, 255, 0, NUMPIXELS);

    /////////////////////////////////////////////////////////
    // ESTRUTURA DO BOTÃO
    ////////////////////////////////////////////////////////

    if (estadoBotao == HIGH)
    {
        analogWrite(MOTOR_PWM_PIN, 255); // Controle de velocidade em PWM
        pixels.setBrightness(255);       // Brilho máximo
        for (int i = 0; i < NUMPIXELS; i++)
        {
            pixels.setPixelColor(i, pixels.Color(0, 255, 0)); // Cor verde
        }
        pixels.show();
    }
    else
    {
        analogWrite(MOTOR_PWM_PIN, velocidadeMotor); // Velocidade baseada no potenciômetro
        pixels.setBrightness(255);                   // Brilho máximo
        for (int i = 0; i < NUMPIXELS; i++)
        {
            if (i < numeroLedsAcender)
            {
                pixels.setPixelColor(i, pixels.Color(255, 0, 0)); // Cor vermelha
            }
            else
            {
                pixels.setPixelColor(i, pixels.Color(0, 0, 0)); // LED desligado
            }
        }
        pixels.show();
    }

    /////////////////////////////////////////////////////////
    // ESTRUTURA DO POTENCIÔMETRO
    ////////////////////////////////////////////////////////
}