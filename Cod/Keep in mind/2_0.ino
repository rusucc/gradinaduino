/*
Rusu Cosmin-Constantin
Gradinaduino v1.0
*/
#include "LedControl.h"

const int d_delay = 400;
//pin layout
const int pin_pompa = 5;
const int pin_trn_senzor = 3;
const int out_senzor = 2;
const int d_din = 9;
const int d_cs = 10;
const int d_clk = 11;
//const int out_senzor_ANALOG = A0;
LedControl lc = LedControl(d_din,d_clk,d_cs,1);
void afisareNumar(unsigned long n){
  lc.clearDisplay(0);
  int v[8]={0};
  int p=0;
  while(n>0)  v[p]=n%10,n=n/10,p++;
  //construirea vectorului de cifre
  if(p>8)     error(),wait(24);
  //daca numarul are mai multe cifre decat poate displayul sa arata, afiseaza eroare
  for(int i = p-1,poz=7; i >= 0 && poz >=0; i--, poz--) lc.setChar(0,poz,v[i],false),delay(d_delay);
  delay(400);
  //afisare cfra cu cifra a numarului
}
void error(){
  lc.clearDisplay(0),delay(d_delay);
  lc.setChar(0,7,'E',false),delay(d_delay);//setare pozitie cu char respectiv, delay dupa
  lc.setChar(0,6,4,false),delay(d_delay);
  lc.setChar(0,5,0,false),delay(d_delay);
  lc.setChar(0,4,4,false),delay(d_delay);
}
void hello(){
  lc.clearDisplay(0),delay(d_delay);
  lc.setChar(0,7,'H',false),delay(d_delay);
  lc.setChar(0,6,'E',false),delay(d_delay);
  lc.setChar(0,5,'L',false),delay(d_delay);
  lc.setChar(0,4,'L',false),delay(d_delay);
  lc.setChar(0,3,'0',false),delay(d_delay);
  lc.setChar(0,2,'.',false),delay(d_delay);
  lc.setChar(0,1,'.',false),delay(d_delay);
  lc.setChar(0,0,'.',false),delay(d_delay);
}
int CitireUmiditate (){
  digitalWrite(pin_trn_senzor,HIGH);  //pornire senzor
  delay(100);
  int valoare_umiditate = digitalRead(out_senzor); //citire valoare
  //int valoare_umiditate = analogRead(out_senzor_ANALOG);
  digitalWrite(pin_trn_senzor,LOW);   //oprire senzor
  Serial.println(valoare_umiditate);  //logare valoarea umiditatii
  return valoare_umiditate;
}
void wait(int ore) {
  const uint32_t MINUT = 1000UL;
  const uint32_t ORA = 60 * MINUT;
  delay(ore*ORA);
}
void setup() {
  // put your setup code here, to run once:
    pinMode(pin_trn_senzor,OUTPUT);
    pinMode(d_din,OUTPUT);
    pinMode(d_cs,OUTPUT);
    pinMode(d_clk,OUTPUT);
    pinMode(pin_pompa,OUTPUT);
    lc.shutdown(0,false);
    lc.clearDisplay(0);
    digitalWrite(pin_trn_senzor,LOW);
    digitalWrite(pin_pompa,LOW);
    Serial.begin(9600);
    //hello();

}
void loop() {
  // put your main code here, to run repeatedly:
  //int valoare_umiditate = CitireUmiditate();
  int valoare_umiditate = 0;
  switch(valoare_umiditate){
  case 1:
    //sol destul de umed, totul ok
    break;
  case 0:
    //sol nefiind umed
    //actionare pompa
    //udare(?)
    break;
  default :
    //cum naiba a ajuns aici nu imi pot imagina, ori e pe analog, ori am descoperit calculaotare cuantice
    Serial.println("Eroare : Senzorul a transmis o valoare neasteptata, asigura-te ca senzorul este conectat digital. :)");
    error();
    break;
  }
  afisareNumar(45684);
  digitalWrite(pin_pompa,HIGH);
  delay(1200);
  digitalWrite(pin_pompa,LOW);
  delay(2000);
}
