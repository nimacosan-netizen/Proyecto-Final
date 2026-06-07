#include <Wire.h>
#include <MPU6050.h>
MPU6050 mpu;
int minPulgar = 0,  maxPulgar = 1023;
int minIndice = 0,  maxIndice = 1023;
int minMedio = 0,   maxMedio = 1023;
int minAnular = 0,  maxAnular = 1023;
int minMenique = 0, maxMenique = 1023;
int letras[27][5] = {
//  pulgar;indice;medio;anular;meñique
    {500,  1000, 1000, 1000, 1000}, // A 
    {0, 0, 0, 0, 0}, // B 
    {500,  500,  500,  500,  500},  // C 
    {500, 0, 500, 500, 500}, // D 
    {1000, 1000, 1000, 1000, 1000}, // E 
    {0, 0, 1000, 1000, 1000}, // F 
    {1000, 500, 1000, 1000, 1000},    // G  
    {0, 0,  0, 1000, 1000}, // H  
    {0, 1000, 1000, 1000, 0},    // I  
    {1000, 1000, 1000, 1000, 0},    // J 
    {0, 0, 0, 1000, 1000},    // K  
    {0, 0, 1000, 1000, 1000}, // L  
    {0, 0, 0, 0, 1000}, // M  
    {1000, 0, 0, 1000, 1000}, // N  
    {500,  500,  500,  500,  500},  // O 
    {0,    0,    500,  1000, 1000}, // P  
    {500,  0,    500,  500, 500}, // Q  
    {0,    0,    0,    1000, 1000}, // R  
    {1000, 1000, 1000, 1000, 1000}, // S  
    {1000, 0,    1000, 1000, 1000}, // T  
    {0,    0,    0,    0,    1000}, // U  
    {0,    0,    0,    500,  500},  // V  
    {0,    0,    0,    0,    0},    // W  
    {1000, 0,    1000, 1000, 1000}, // X  
    {0,    1000, 1000, 1000, 0},    // Y 
    {0,    0,    1000, 1000, 1000}, // Z
    {0,    0,    0,    1000, 1000}  // CH 
};

String abecedario[27] = {
    "A","B","C","D","E","F","G","H","I","J","K",
    "L","M","N","O","P","Q","R","S","T","U","V",
    "W","X","Y","Z","CH"
};

String reconocerLetra() {
    int sensores[5];
    sensores[0] = map(analogRead(A0), minPulgar, maxPulgar, 0, 1000);
    sensores[1] = map(analogRead(A1), minIndice, maxIndice, 0, 1000);
    sensores[2] = map(analogRead(A2), minMedio, maxMedio, 0, 1000);
    sensores[3] = map(analogRead(A3), minAnular, maxAnular, 0, 1000);
    sensores[4] = map(analogRead(A4), minMenique, maxMenique, 0, 1000);

    for (int i = 0; i < 27; i++) {
        bool coincide = true;
        for (int j = 0; j < 5; j++) {
            if (abs(sensores[j] - letras[i][j]) > 100) { 
                coincide = false;
                break;
            }
        }
        if (coincide) {
            return abecedario[i]; 
        }
    }
    return ""; 
}
bool confirmarOrientacion(String letra) {
    int16_t ax, ay, az, gx, gy, gz;
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    int umbral = 5000; 

    if (letra == "A" || letra == "E" || letra == "F" || letra == "I" || letra == "L" || letra == "R" || letra == "V" || letra == "W" || letra == "Y" || letra == "Z") {
        return (ay > umbral);
    }
    else if (letra == "B" || letra == "C" || letra == "D" || letra == "O" || letra == "T") {
        return (ay > umbral && ax > umbral);
    }
    else if (letra == "J") {
    int16_t ax, ay, az, gx, gy, gz;
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    return (abs(gz) > 8000);
    }   
    else if (letra == "H") {
        int16_t ax, ay, az, gx, gy, gz;
        mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
        return (ay < -umbral && gz < -5000);
    }
    else if (letra == "G") {
    int16_t ax, ay, az, gx, gy, gz;
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    return (ax > umbral && abs(gx) > 5000 && abs(gy) > 5000);
    }
    else if (letra == "K" || letra == "X") {
        return (ax > umbral);
    }
    else if (letra == "P") {
        return (ay < -umbral);
    }
    else if (letra == "M" || letra == "N" || letra == "Ñ" || letra == "Q") {
        return (abs(ay) < 5000 && abs(ax) < 5000); 
    }
}
bool confirmarHola(){
    int indice = map(analogRead(A1), minIndice, maxIndice, 0, 1000);
    int pulgar = map(analogRead(A0), minPulgar, maxPulgar, 0, 1000);
    int medio  = map(analogRead(A2), minMedio,  maxMedio,  0, 1000);
    int anular = map(analogRead(A3), minAnular, maxAnular, 0, 1000);
    int menique= map(analogRead(A4), minMenique,maxMenique,0, 1000);

    int16_t ax, ay, az, gx, gy, gz;
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    bool dedosCorrectos = (indice < 300 && pulgar > 700 && medio > 700 && anular > 700 && menique > 700);
    bool movimientoCorrecto = (gx > 5000);

    return (dedosCorrectos && movimientoCorrecto);
}
bool detectarPorFavor(){
    int pulgar = map(analogRead(A0), minPulgar, maxPulgar, 0, 1000);
    int indice = map(analogRead(A1), minIndice, maxIndice, 0, 1000);
    int medio  = map(analogRead(A2), minMedio,  maxMedio,  0, 1000);
    int anular = map(analogRead(A3), minAnular, maxAnular, 0, 1000);
    int menique= map(analogRead(A4), minMenique,maxMenique,0, 1000);

    int16_t ax, ay, az, gx, gy, gz;
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    bool dedosCorrectos = (pulgar > 700 && indice > 700 && medio > 700 && anular > 700 && menique > 700);
    bool movimientoCorrecto = (abs(gx) > 5000 && abs(gy) > 5000);
    return (dedosCorrectos && movimientoCorrecto);
}
bool detectarConGusto(){
    int pulgar = map(analogRead(A0), minPulgar, maxPulgar, 0, 1000);
    int indice = map(analogRead(A1), minIndice, maxIndice, 0, 1000);
    int medio  = map(analogRead(A2), minMedio,  maxMedio,  0, 1000);
    int anular = map(analogRead(A3), minAnular, maxAnular, 0, 1000);
    int menique= map(analogRead(A4), minMenique,maxMenique,0, 1000);

    int16_t ax, ay, az, gx, gy, gz;
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    bool dedosCorrectos = (pulgar < 300 && indice < 300 && medio < 300 && anular < 300 && menique < 300);
    bool movimientoCorrecto = (gx > 5000);
    return (dedosCorrectos && movimientoCorrecto);
}
bool detectarChao(){
    int pulgar = map(analogRead(A0), minPulgar, maxPulgar, 0, 1000);
    int indice = map(analogRead(A1), minIndice, maxIndice, 0, 1000);
    int medio  = map(analogRead(A2), minMedio,  maxMedio,  0, 1000);
    int anular = map(analogRead(A3), minAnular, maxAnular, 0, 1000);
    int menique= map(analogRead(A4), minMenique,maxMenique,0, 1000);

    int16_t ax, ay, az, gx, gy, gz;
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    bool dedosCorrectos = (pulgar < 300 && indice < 300 && medio < 300 && anular < 300 && menique < 300);
    bool movimientoCorrecto = (abs(gy) > 5000);
    return (dedosCorrectos && movimientoCorrecto);
}
bool detectarPerdon(){
    int pulgar = map(analogRead(A0), minPulgar, maxPulgar, 0, 1000);
    int indice = map(analogRead(A1), minIndice, maxIndice, 0, 1000);
    int medio  = map(analogRead(A2), minMedio,  maxMedio,  0, 1000);
    int anular = map(analogRead(A3), minAnular, maxAnular, 0, 1000);
    int menique= map(analogRead(A4), minMenique,maxMenique,0, 1000);

    int16_t ax, ay, az, gx, gy, gz;
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    bool dedosCorrectos = (pulgar < 300 && menique < 300 && indice > 700 && medio > 700 && anular > 700);
    bool orientacionCorrecta = (ay < -10000);
    return (dedosCorrectos && orientacionCorrecta);
}
bool detectarTeAmo(){
    int pulgar = map(analogRead(A0), minPulgar, maxPulgar, 0, 1000);
    int indice = map(analogRead(A1), minIndice, maxIndice, 0, 1000);
    int medio  = map(analogRead(A2), minMedio,  maxMedio,  0, 1000);
    int anular = map(analogRead(A3), minAnular, maxAnular, 0, 1000);
    int menique= map(analogRead(A4), minMenique,maxMenique,0, 1000);

    int16_t ax, ay, az, gx, gy, gz;
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    bool dedosCorrectos = (pulgar < 300 && indice < 300 && medio > 700 && anular > 700 && menique < 300);
    bool movimientoCorrecto = (gx > 5000 && ay > 10000);
    return (dedosCorrectos && movimientoCorrecto);
}
void setup() {
    Serial.begin(9600);
    Wire.begin();
    mpu.initialize();
    
    if (!mpu.testConnection()) {
        Serial.println("Error: MPU6050 no conectado correctamente.");
    }
}
void loop() {
   int16_t ax, ay, az, gx, gy, gz;
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  
  Serial.print("ay: "); Serial.println(ay);
  Serial.print("A0: "); Serial.println(analogRead(A0));
  Serial.print("A1: "); Serial.println(analogRead(A1));
  Serial.print("A2: "); Serial.println(analogRead(A2));
  Serial.print("A3: "); Serial.println(analogRead(A3));
  Serial.print("A4: "); Serial.println(analogRead(A4));
  Serial.println("---");
  delay(1000);
  Serial.println("loop corriendo...");
  delay(500);
    if (confirmarHola()) {
        Serial.println("HOLA");
        delay(2000);
    } 
    else if (detectarChao()) {
    Serial.println("CHAO");
    delay(2000);
    }
    else if(detectarConGusto()){
        Serial.println("Con Gusto");
        delay(2000);
    }
    else if(detectarPorFavor()){
        Serial.println("Por Favor");
        delay(2000);
    }
    else if(detectarPerdon()){
        Serial.println("Perdon");
        delay(2000);
    }
    else if(detectarTeAmo()){
        Serial.println("Te Amo");
        delay(2000);
    }
    else {
        String letraDetectada = reconocerLetra();
        if (letraDetectada != "") {
            if (confirmarOrientacion(letraDetectada)) {
                Serial.print("Letra Confirmada: ");
                Serial.println(letraDetectada);
                delay(1000);
            }
        }
    }
    }
}
