#define led 0
#define botao 4
void setup() 
{
 pinMode(0,OUTPUT);
 pinMode(4,INPUT);
 digitalWrite(led,0);
}

void loop() {
    int estadoBotao = digitalRead(BOTAO_PIN);
    if(estadoBotao == HIGH){
        digitalWrite(led,1);
    }
    else{
        digitalWrite(led,1);
    }
 }