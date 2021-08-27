/*
  Rusu Cosmin-Constantin
  Gradinaduino v1.0
*/
#include "LedControl.h"
//definitii pentru valorile de timp
const uint32_t _SECUNDA = 1000UL;
const uint32_t _MINUT = 60 * _SECUNDA;
const uint32_t _ORA = 60 * _MINUT;
const int d_delay = 400;
//definitii pentru volum
const double _DEBIT = 1000 / 60 ; // mililitru/s ;
const uint32_t _LITRU = 1 ;
//pin layout
const int pin_pompa = 5;            //DEBIT  POMPA ~= 1L/MIN
const int pin_trn_senzor = 3;       //Switch on-off pentru senzor
const int out_senzor = 2;           //Intrare pentru senzor
const int d_din = 9;                //display
const int d_cs = 10;                //display
const int d_clk = 11;               //display

LedControl lc = LedControl(d_din, d_clk, d_cs, 1); //display
void afisareNumar(unsigned long n) {
  Serial.println("Afisare numar");
  lc.clearDisplay(0);
  int v[8] = {0};
  int p = 0;
  while (n > 0)  v[p] = n % 10, n = n / 10, p++;
  //construirea vectorului de cifre
  if (p > 8)     error();
  //daca numarul are mai multe cifre decat poate displayul sa arata, afiseaza eroare
  for (int i = p - 1, poz = 7; i >= 0 && poz >= 0; i--, poz--) lc.setChar(0, poz, v[i], false), delay(d_delay);
  delay(400);
  //afisare cfra cu cifra a numarului
}
void error() {
  lc.clearDisplay(0), delay(d_delay);
  lc.setChar(0, 7, 'E', false), delay(d_delay); //setare pozitie cu char respectiv, delay dupa
  lc.setChar(0, 6, 4, false), delay(d_delay);
  lc.setChar(0, 5, 0, false), delay(d_delay);
  lc.setChar(0, 4, 4, false), delay(d_delay);
  Serial.println("ERROR !");
  digitalWrite(pin_pompa, LOW);
  digitalWrite(pin_trn_senzor, LOW);
  for (;;) Serial.println("ERROR !"), delay(10 * _SECUNDA);
}
int CitireUmiditate () {
  digitalWrite(pin_trn_senzor, HIGH); //pornire senzor
  Serial.print("Shhht, vorbeste planta si spune ca: ");
  delay(1000);
  int valoare_umiditate = digitalRead(out_senzor); //citire valoare
  delay(1000);
  digitalWrite(pin_trn_senzor, LOW);  //oprire senzor
  switch (valoare_umiditate) {
    case 0:
      Serial.println("trebuie udata ! :/");
      break;
    case 1:
      Serial.println("este in stare buna !:)");
  }
  return valoare_umiditate;
}
void uda(double VOLUM, String UNITATE) {
  if (UNITATE == "LITRI") VOLUM *= 1000; // convertit din l in ml
  //volum in ml, ne-am asigurat de asta
  double dt = VOLUM / _DEBIT; //v in ml, debit in ml/s
  digitalWrite(pin_pompa, HIGH);
  Serial.println(String("Udam florile cu " + String(VOLUM) + " ml apa intr-un interval de: " + String(dt) + " secunde"));
  delay(dt * _SECUNDA);
  digitalWrite(pin_pompa, LOW);
  Serial.println("Am terminat de udat ! :)");
}
void wait(int VAL, String unit) {
  char cs = (unit == "SECUNDE")*'s' + (unit == "ORE")*'o' + (unit == "MINUTE")*'m' + (unit == "MILISECUNDE")*'u';
  switch (cs) {
    case 'o': //ore
      delay(VAL * _ORA);
      break;
    case 'm'://minute
      delay(VAL * _MINUT);
      break;
    case 's'://secunde
      delay(VAL * _SECUNDA);
      break;
    case 'u'://milsecunde
      delay(VAL);
      break;
    default ://ai belit pla
      error();
  }
}
void pornire_display(){
  lc.shutdown(0, false);    //pornit display
  lc.clearDisplay(0);
  delay(d_delay);//curatat display
  Serial.println("Pornire display...");
}
void oprire_display(){
  lc.clearDisplay(0),delay(d_delay);
  lc.setChar(0,7,'0',false),delay(d_delay);
  lc.setChar(0,6,'F',false),delay(d_delay);
  lc.setChar(0,5,'F',false),delay(d_delay);
  delay(3*_SECUNDA);
  Serial.println("Oprire display");
  lc.shutdown(0,true);  
}
void setup() {
  Serial.begin(9600);
  pinMode(pin_trn_senzor, OUTPUT);
  pinMode(d_din, OUTPUT);
  pinMode(d_cs, OUTPUT);
  pinMode(d_clk, OUTPUT);
  pinMode(pin_pompa, OUTPUT); //setat pini
  digitalWrite(pin_trn_senzor, LOW); //off senzor
  digitalWrite(pin_pompa, LOW);     //off pompa
  Serial.println("START OK");

}
void loop() {
  // put your main code here, to run repeatedly:
  pornire_display();
  int valoare_umiditate = CitireUmiditate();
  lc.setChar(0, 7, valoare_umiditate, false); //valoarea umiditatii afisata in permanenta pe display
  delay(2*_SECUNDA);                             //delayul pt vizibilitate
  switch (valoare_umiditate) {
    case 1:
      //sol destul de umed, totul ok
      break;
    case 0:
      //sol nefiind umed
      uda(100,"MILILITRI");//actionare pompa
      break;
    default :
      //cum naiba a ajuns aici nu imi pot imagina, ori e pe analog, ori am descoperit calculaotare cuantice
      Serial.println("Eroare : Senzorul a transmis o valoare neasteptata, asigura-te ca senzorul este conectat digital. :)");
      error();
      break;
  }
  Serial.println("Ne revedem peste 8 ore !");
  oprire_display();
  wait(8, "ORE");
  //wait(8,"SECUNDE");
}
