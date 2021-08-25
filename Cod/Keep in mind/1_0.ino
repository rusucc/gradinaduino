/*
Rusu Cosmin-Constantin
Gradinaduino v1.0
*/



const int perioada = 1 ; // in ore
//pin layout
const int pin_trn_senzor = 3;
const int out_senzor = 2; 
//const int out_senzor_ANALOG = A0;


int CitireUmiditate (){
  digitalWrite(pin_trn_senzor,HIGH);
  delay(2000);
  int valoare_umiditate = digitalRead(out_senzor);
  //int valoare_umiditate = analogRead(out_senzor_ANALOG);
  digitalWrite(pin_trn_senzor,LOW);
  Serial.println(valoare_umiditate);
  return valoare_umiditate;
}
void wait() {
  const uint32_t MINUT = 1000UL;
  const uint32_t ORA = 60 * MINUT;
  delay(perioada*ORA);
}
void setup() {
  // put your setup code here, to run once:
    digitalWrite(pin_trn_senzor,LOW);
    Serial.begin(9600);

}
void loop() {
  // put your main code here, to run repeatedly:
  int valoare_umiditate = CitireUmiditate();
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
    break;
  }
  
}
