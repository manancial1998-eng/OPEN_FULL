//int RPM[2] = {A0,A1};
int TENSAO[3] = {A2,A3,A4}; // 0: motor | 1: bateria | 2: 5V ref
int CORRENTE[2] = {A5,A6}; // 0: motor | 1: bateria


// ==== Constantes ====
const float VREF = 5.0;         // Ref. ADC (Arduino Nano ~5V)
const int ADC_RES = 1023;       // 10 bits
const float ACS_SENS = 0.066;   // Sensibilidade ACS712 30A (66mV/A)

// ==== Tempo ====
unsigned long ultimaLeitura = 0;
const unsigned long tempoLeitura = 200; // ms entre leituras

// ==== Função leitura de tensão ====
float readVoltage(int pin, float R1 = 1.0, float R2 = 1.0) {
  int raw = analogRead(pin);
  float Vadc = (raw * VREF) / ADC_RES;
  return Vadc * ((R1 + R2) / R2);
}

// ==== Função leitura de corrente ====
float readCurrent(int pin, float offset) {
  int raw = analogRead(pin);
  float Vadc = (raw * VREF) / ADC_RES;
  float Vdiff = Vadc - offset;
  return Vdiff / ACS_SENS;
}

//LORA
int CS = 10;
//int SCK = 13; // informação a cada curso 
//int MISO = 12;// dados enviados
//int MOSI = 11;

int PONTE_H[2] = {5,6}; //PINO 5 É O RPWM GIRANDO PARA A DIREITA E O PINO 6 LPWM GIRANDO PARA A ESQUERDA
int i = 0;
int ACELERADOR = 9;
  int AceState = 0;
  int AcePState = 0;
  int Ace = 0;
  int eps = 6;
  int mov = 0;
  int aux = 0;
int SWITCH = A0;
int Sw = 0;
byte SwState = 0;
//int POT = A7;
int Pot = 0;
int PotState = 0;
int PotPState = 0;
int SONAR[2] = {7,8}; //PINO 7 = ECHO, PINO 8 = TRIG

unsigned long tempoPulso = 0;
unsigned long ultimaLeituraS = 0;
float distancia = 0;

//REGISTRADOR DESLOCAMENTO
int REG74HC595[3] = {2,3,4}; // pino2= dados DS; Pino3= latsh/ST_CP; Pino4= Clock/SH_CP 
byte estadoRegistrador = B11111111;



unsigned long timer;
unsigned long last;

//RPM

volatile unsigned long pulsos[2] = {0, 0};

// ==== Ajustes ====
const int pulsosPorVolta = 20;              // nº de pulsos por rotação (ajuste conforme seu sensor)
const unsigned long intervaloRPM = 1000;   // janela de cálculo em ms

// ==== Variáveis ====
unsigned long ultimoTempoRPM = 0;
float rpm[2] = {0, 0};

// ==== Interrupções PCINT ====
//ISR(PCINT1_vect) {  
  // PCINT1 cobre pinos A0–A5
  //if (digitalRead(RPM[0]) == HIGH) pulsos[0]++;
  //if (digitalRead(RPM[1]) == HIGH) pulsos[1]++;
//}

void setup() {

  Serial.begin(9600);

  //for(i=0;i<2;i++){
    //pinMode(RPM[i], INPUT_PULLUP);
    
  //}

    // Habilita PCINT para grupo A0-A5
 // PCICR  |= (1 << PCIE1);   

  // Ativa interrupções individuais
  //PCMSK1 |= (1 << PCINT8);  // A0
  //PCMSK1 |= (1 << PCINT9);  // A1*/

  for(i=0;i<3;i++){
    pinMode(TENSAO[i], INPUT_PULLUP);
  }
  for(i=0;i<2;i++){
    pinMode(CORRENTE[i], INPUT_PULLUP);
  }
  for(i=0;i<2;i++){
    pinMode(PONTE_H[i], OUTPUT);
  }
  pinMode(SONAR[1], OUTPUT);
  pinMode(SONAR[0], INPUT);
  for(i=0;i<3;i++){
    pinMode(REG74HC595[i], OUTPUT);
  }
  pinMode(ACELERADOR, INPUT_PULLUP);
  pinMode(SWITCH, INPUT_PULLUP);
//  pinMode(POT, INPUT_PULLUP);
}

void loop() {
//  potenc();
  logica();
  if(aux != 0 ){
   // S_RPM();
    //DistSonar();
    //Tensao_Corrente();
  }

}
int Tensao_Corrente(){
  unsigned long agora = millis();

  if (agora - ultimaLeitura >= tempoLeitura) {
    ultimaLeitura = agora;

    // --- Offset ACS712 (meio da tensão de 5V real) ---
    float V5   = readVoltage(TENSAO[2]); 
    float offset = V5 / 2.0;  

    // --- Tensões ---
    float Vmotor = readVoltage(TENSAO[0], 10000.0, 2000.0); // ajuste divisor
    float Vbat   = readVoltage(TENSAO[1], 10000.0, 2000.0);

    // --- Correntes ---
    float Imotor = readCurrent(CORRENTE[0], offset);
    float Ibat   = readCurrent(CORRENTE[1], offset);

    // --- Serial Monitor ---
    Serial.print("Vmotor: "); Serial.print(Vmotor); Serial.print(" V  |  ");
    Serial.print("Vbat: ");   Serial.print(Vbat);   Serial.print(" V  |  ");
    Serial.print("5V: ");     Serial.print(V5);     Serial.print(" V  ||  ");
    Serial.print("Imotor: "); Serial.print(Imotor); Serial.print(" A  |  ");
    Serial.print("Ibat: ");   Serial.print(Ibat);   Serial.println(" A");
  }
}

int S_RPM(){
  unsigned long now = millis();

  if (now - ultimoTempoRPM >= intervaloRPM) {
    ultimoTempoRPM = now;

    // Captura e zera contadores com segurança
    noInterrupts();
    unsigned long p1 = pulsos[0]; pulsos[0] = 0;
    unsigned long p2 = pulsos[1]; pulsos[1] = 0;
    interrupts();

    // Frequência em Hz
    float f1 = (float)p1 / (intervaloRPM / 1000.0);
    float f2 = (float)p2 / (intervaloRPM / 1000.0);

    // Converte para RPM
    rpm[0] = (f1 / pulsosPorVolta) * 60.0;
    rpm[1] = (f2 / pulsosPorVolta) * 60.0;

    // Serial
    Serial.print("RPM1: "); Serial.print(rpm[0]);
    Serial.print("  |  RPM2: "); Serial.println(rpm[1]);
  }

}

void DistSonar() {
    unsigned long agoraS = micros();

  // Disparo do pulso de trigger a cada 60ms
  if (agoraS - ultimaLeituraS >= 60000) {
    digitalWrite(SONAR[1], LOW);
    delayMicroseconds(2);
    digitalWrite(SONAR[1], HIGH);
    delayMicroseconds(10);
    digitalWrite(SONAR[1], LOW);

    // Medir duração do pulso no Echo
    tempoPulso = pulseIn(SONAR[0], HIGH, 30000); // timeout de 30ms

    // Converter para cm
    distancia = tempoPulso * 0.0343 / 2.0;

    // Mostrar no Serial
    Serial.print("Distancia: ");
    Serial.print(distancia);
    Serial.println(" cm");

    ultimaLeituraS = agoraS;
  }
}

//SWITCH QUE DANIFICOU
int SW(){
  Sw = map(constrain(pulseIn(SWITCH, HIGH), 1000, 1978), 1000, 2000, 0, 255);
  //Serial.print("SW : ");
  //Serial.println(Sw);
  if(pulseIn(SWITCH,HIGH)<1200){
    SwState = 0;
    //Serial.println("SW POS1");
  }
  else if((Sw >= 90) && (Sw < 210)){
   SwState = 127; 
  }
  else if(pulseIn(SWITCH,HIGH)>1400){
    SwState = 255; 
   // Serial.println("SW POS2");
  }
  return SwState;
}


int logica(){

  byte SwS;
  SwS = SW();
  Acelerador();
  if((SwState == 0) && (aux != 0)){
        bitClear(estadoRegistrador, 0); // coloca bit 0 em LOW
        bitClear(estadoRegistrador, 1); // coloca bit 1 em LOW
        bitSet(estadoRegistrador, 2); // coloca bit 0 em HIGH
        bitSet(estadoRegistrador, 3); // coloca bit 1 em HIGH
        bitClear(estadoRegistrador, 4); // coloca bit 4 em LOW
        bitClear(estadoRegistrador, 5); // coloca bit 5 em LOW
        bitClear(estadoRegistrador, 6); // coloca bit 6 em LOW
        bitClear(estadoRegistrador, 7); // coloca bit 7 em LOW
        
        digitalWrite(REG74HC595[1], LOW);
        shiftOut(REG74HC595[0], REG74HC595[2], MSBFIRST, estadoRegistrador);
        digitalWrite(REG74HC595[1], HIGH);  
        Serial.println("ENTROU CONDIÇÃO 0");
        aux = 0;        
  }
  else if((SwState == 127) && (aux != 127)){
      bitSet(estadoRegistrador, 0); // coloca bit 0 em HIGH EN R
      bitSet(estadoRegistrador, 1); // coloca bit 1 em HIGH EN L
      bitSet(estadoRegistrador, 2); // coloca bit 4 em HIGH MR1
      bitSet(estadoRegistrador, 3); // coloca bit 4 em HIGH MR2
      bitSet(estadoRegistrador, 4); // coloca bit 4 em HIGH
      bitClear(estadoRegistrador, 5); // coloca bit 5 em HIGH
      bitClear(estadoRegistrador, 6); // coloca bit 6 em LOW
      bitClear(estadoRegistrador, 7); // coloca bit 7 em LOW
      digitalWrite(REG74HC595[1], LOW);
      shiftOut(REG74HC595[0], REG74HC595[2], MSBFIRST, estadoRegistrador);
      digitalWrite(REG74HC595[1], HIGH);  
      Serial.println("ENTROU CONDIÇÃO 1");
      aux = 127;
  }
    else if(SwState == 255){
      if((mov == 0) && (aux !=0)){ 
        bitClear(estadoRegistrador, 0); // coloca bit 0 em HIGH EN R
        bitClear(estadoRegistrador, 1); // coloca bit 1 em HIGH EN L
        bitSet(estadoRegistrador, 2); // coloca bit 4 em HIGH MR1
        bitSet(estadoRegistrador, 3); // coloca bit 4 em HIGH MR2
        bitSet(estadoRegistrador, 4); // coloca bit 4 em HIGH
        bitClear(estadoRegistrador, 5); // coloca bit 5 em HIGH
        bitClear(estadoRegistrador, 6); // coloca bit 6 em LOW
        bitSet(estadoRegistrador, 7); // coloca bit 7 em LOW
        digitalWrite(REG74HC595[1], LOW);
        shiftOut(REG74HC595[0], REG74HC595[2], MSBFIRST, estadoRegistrador);
        digitalWrite(REG74HC595[1], HIGH);  
        Serial.println("ENTROU CONDIÇÃO 2");
        aux = 0;
      }
      else if((mov == 1) && (aux != 255)){ 
        bitClear(estadoRegistrador, 0); // coloca bit 0 em HIGH EN R
        bitClear(estadoRegistrador, 1); // coloca bit 1 em HIGH EN L
        bitSet(estadoRegistrador, 2); // coloca bit 4 em HIGH MR1
        bitSet(estadoRegistrador, 3); // coloca bit 4 em HIGH MR2
        bitSet(estadoRegistrador, 4); // coloca bit 4 em HIGH
        bitClear(estadoRegistrador, 5); // coloca bit 5 em HIGH
        bitSet(estadoRegistrador, 6); // coloca bit 6 em LOW
        bitSet(estadoRegistrador, 7); // coloca bit 7 em LOW
        digitalWrite(REG74HC595[1], LOW);
        shiftOut(REG74HC595[0], REG74HC595[2], MSBFIRST, estadoRegistrador);
        digitalWrite(REG74HC595[1], HIGH);
        Serial.println("ENTROU CONDIÇÃO 3");
        aux = 255;  
      }
      else if((mov == -1) && (aux != -255)){ 
        bitClear(estadoRegistrador, 0); // coloca bit 0 em HIGH EN R
        bitClear(estadoRegistrador, 1); // coloca bit 1 em HIGH EN L
        bitSet(estadoRegistrador, 2); // coloca bit 4 em HIGH MR1
        bitSet(estadoRegistrador, 3); // coloca bit 4 em HIGH MR2
        bitSet(estadoRegistrador, 4); // coloca bit 4 em HIGH
        bitSet(estadoRegistrador, 5); // coloca bit 5 em HIGH
        bitClear(estadoRegistrador, 6); // coloca bit 6 em LOW
        bitSet(estadoRegistrador, 7); // coloca bit 7 em LOW
        digitalWrite(REG74HC595[1], LOW);
        shiftOut(REG74HC595[0], REG74HC595[2], MSBFIRST, estadoRegistrador);
        digitalWrite(REG74HC595[1], HIGH);
        Serial.println("ENTROU CONDIÇÃO 4");
        aux = -255;  
      }
  }
}


int Acelerador() {  
  Ace = pulseIn(ACELERADOR,HIGH);
  Serial.print("ace: ");
  Serial.println(Ace);
  AceState = constrain(map(Ace,989,1974,-255,255),-255,255);
      //COMO TEM VALORES QUE OSCILAM RAPIDO ENTRE eps*mult*2 UNIDADES E O VALOR ACABA VARIANDO EM +/-12 O IF É PARA DEIXAR UMA ZONA MORTA NO MEIO DO ACELERADOR
      if (abs(AceState) <= eps*2){
        AceState = 0; 
        mov = 0;
      }
  
      //AS VEZES O VALOR NÃO ALCANÇA O -255, ESSE IF É PARA FORÇAR O -255 POR CONTA DAS OSCILAÇÕES
      else if (AceState <= -255 + (eps)){
        AceState = -255; 
        mov = -1; 
      }
  
      
      //AS VEZES O VALOR NÃO ALCANÇA O 255, ESSE IF É PARA FORÇAR O 255 POR CONTA DAS OSCILAÇÕES
      else if (AceState >= 255 - (eps+2)){
        AceState = 255; 
        mov = 1; 
      }
  if (abs(AceState - AcePState)> eps){
    //Serial.print("ACESTATE: ");
    //Serial.println(AceState);
    ponteh();
    AcePState = AceState;
  }
}

void ponteh(){
      if (abs(AceState) <= 16){
        //Serial.println("ENTROU NO 0 ");
        analogWrite(PONTE_H[1], 0);
        analogWrite(PONTE_H[0], 0);
      }
      else if (AceState < -16){
       // Serial.println("ENTROU RE"); 
        analogWrite(PONTE_H[1], abs(AceState));
        analogWrite(PONTE_H[0], 0);
      }
      else if (AceState > 16){
       // Serial.println("ENTROU AVANTE ");
        analogWrite(PONTE_H[0], AceState);
        analogWrite(PONTE_H[1], 0);
      }
 }

// strings
String criarStringDados() {
  String dados = "";
  
  // bateria e motor
  float V5 = readVoltage(TENSAO[2]); 
  float offset = V5 / 2.0;
  float Vmotor = readVoltage(TENSAO[0], 10000.0, 2000.0);
  float Vbat = readVoltage(TENSAO[1], 10000.0, 2000.0);
  float Imotor = readCurrent(CORRENTE[0], offset);
  float Ibat = readCurrent(CORRENTE[1], offset);
  
  // RPM
  unsigned long now = millis();
  if (now - ultimoTempoRPM >= 1000) {
    ultimoTempoRPM = now;
    noInterrupts();
    unsigned long p1 = pulsos[0]; pulsos[0] = 0;
    unsigned long p2 = pulsos[1]; pulsos[1] = 0;
    interrupts();
    
    float f1 = (float)p1 / 1.0;
    float f2 = (float)p2 / 1.0;
    rpm[0] = (f1 / pulsosPorVolta) * 60.0;
    rpm[1] = (f2 / pulsosPorVolta) * 60.0;
  }
  
  // sonar
  if (now - ultimaLeituraS >= 100) {
    ultimaLeituraS = now;
    digitalWrite(SONAR[1], LOW);
    delayMicroseconds(2);
    digitalWrite(SONAR[1], HIGH);
    delayMicroseconds(10);
    digitalWrite(SONAR[1], LOW);
    unsigned long tempoPulso = pulseIn(SONAR[0], HIGH, 30000);
    distancia = tempoPulso * 0.0343 / 2.0;
  }
  
  // acelerator
  int AceState = constrain(map(Ace, 989, 1974, -255, 255), -255, 255);or
  int Ace = pulseIn(ACELERADOR, HIGH);
  
  // Formatar a string
  dados += "SONAR:" + String(distancia, 2);
  dados += "|MOTOR:" + String(rpm[0], 0);
  dados += "|HELICE:" + String(rpm[1], 0);
  dados += "|MOTOR_POW:" + String(Vmotor, 1) + "," + String(Imotor, 1);
  dados += "|BATERIA:" + String(Vbat, 1) + "," + String(Ibat, 1);
  
  return dados;
}

