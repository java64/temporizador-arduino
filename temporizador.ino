//TEMPORIZADOR ARDUINO + LCD

#include <LiquidCrystal.h> //Libreria del display LCD
#define TONO_ERROR 600
#define TIME_INTERVAL 3000


LiquidCrystal lcd(12, 11, 5, 4, 3, 2); //Inicializamos la libreria con el numero de los pines a utilizar

int buzzer = 13;           //Alarma
int ahoras = 0;            //Variable a mostrar por LCD de las horas
int aminutos = 0;          //Variable a mostrar por LCD de los minutos
int asegundos = 0;         //Variable a mostrar por LCD de los segundos
int segundostotal = 0;     //Tiempo total
int msg = 0;               //Barrera para el mensaje de bienvenida

int start = A2;            //Pulsador de arranque
int empieza = 1024;        // Variable para almacenaje del pulsador de arranque

int buth = A5;             //Pulsador de Horas
int butm = A4;             //Pulsador de Minutos
int buts = A3;             //Pulsador de segundos

int varbuth = 0;           //Variable para almacenar el valor del pulsador de horas
int varbutm = 0;           //Variable para almacenar el valor del pulsador de minutos
int varbuts = 0;           //Variable para almacenar el valor del pulsador de segundos

void setup()
{
  lcd.begin(16, 2);         // Configuramos el numero de columnas y filas del LCD.

  pinMode(buzzer, OUTPUT);  //Pin de alarma --> Salida
  pinMode(buth, INPUT);     //Pin de pulsador de horas --> Entrada
  pinMode(butm, INPUT);     //Pin de pulsador de minutos --> Entrada
  pinMode(buts, INPUT);     //Pin de pulsador de segundos --> Entrada
  pinMode(start, INPUT);    //Pin de pulsador de arranque --> Entrada
  pinMode (9, OUTPUT);      //pin configurado como salida Melodia.
  pinMode(13, OUTPUT);      //Foco insoladora

  msg = 0;                  //Barrera del mensaje de bienvenida
  empieza = 1024;           //Barrera de arranque

  varbuth = 1;              //Barrera de horas
  varbutm = 1;              //Barrera de minutos
  varbuts = 1;              //Barrera de segundos
}

void loop()
{
  digitalWrite(13,HIGH); //Dejamos el foco apagado

  if (msg == 0)          //Mostramos el mensaje de bienvenida solo una vez
  {
    lcd.setCursor(0, 0);
    lcd.print("   INSOLADORA");
    lcd.setCursor(1, 1);
    lcd.print("  FENIX V.1");
    delay(3600);
    msg = 1;
    lcd.clear();
  }

  //-------------------------------------------------------------------------------------------------
  // LECTURA DE LOS BOTONES Y ELECCIÓN DEL TIEMPO, NO SALE DEL BUCLE HASTA PULSAR
  // EL BOTON DE ARRANQUE
  //-------------------------------------------------------------------------------------------------

  do
  {

    varbuth = analogRead(buth);   //Leemos boton de horas
    varbutm = analogRead(butm);   //Leemos boton de minutos
    varbuts = analogRead(buts);   //Leemos boton de segundos

    if (varbuth == 0)             //Si el boton ha sido pulsado, aumentamos las horas en una unidad
    {
      ahoras = ahoras + 1 ;
      delay(250);
    }

    if (varbutm == 0)           //Si el boton ha sido pulsado, aumentamos los minutos en una unidad
    {
      aminutos = aminutos + 1;
      delay(250);
    }

    if (varbuts == 0)           //Si el boton ha sido pulsado, aumentamos los segundos en una unidad
    {
      asegundos = asegundos + 1;
      delay(250);
    }

    lcd.setCursor(0, 0);
    lcd.print("Tiempo Insolado:");  //Muestra mensaje y las HH:MM:SS que vayamos aumentando

    lcd.setCursor(4, 1);

    if (ahoras < 10) lcd.print("0");    // Si las horas son menor que 10, pone un "0" delante.
    lcd.print(ahoras);                 // Sin este codigo, se muestra asi: H:M:S  (1:M:S)
    lcd.print(":");

    if (aminutos < 10) lcd.print("0");  // Si los minutos son menor que 10, pone un "0" delante.
    lcd.print(aminutos);               // Sin este codigo, se muestra asi: H:M:S  (H:1:S)

    lcd.print(":");
    if (asegundos < 10) lcd.print("0"); // Si los segundos son menor que 10, pone un "0" delante.
    lcd.print(asegundos);              // Sin este codigo, se muestra asi: H:M:S  (H:M:1)

    empieza = analogRead(start);   //Lee el boton de arranque

    if (empieza == 0)              //Si el boton de arranque, fue pulsado...
    {
      segundostotal = asegundos + (aminutos * 60) + (ahoras * 60 * 60);  //Convierte el tiempo elegido en segundos!!
    }

  } while (empieza != 0); // Se repite el menu de elegir tiempo hasta que pulsemos el boton de arranque.

  //-------------------------------------------------------------------------------------------------
  // UNA VEZ PULSADO EL BOTON DE ARRANQUE Y ACUMULADO EL TIEMPO, ENTRA EN EL SIGUIENTE WHILE
  // Y NO FINALIZA HASTA TERMINAR LA CUENTA.
  //-------------------------------------------------------------------------------------------------

  while (segundostotal > 0)
  {
    digitalWrite(13,LOW);

    delay (1000);          //Descontamos en periodos de 1 segundo
    segundostotal--;

    ahoras = ((segundostotal / 60) / 60);  //Convertimos los segundos totales en horas
    aminutos = (segundostotal / 60) % 60;  //Convertimos los segundos totales en minutos
    asegundos = segundostotal % 60;        //Convertimos los segundos totales en periodos de 60 segundos

    lcd.setCursor(0, 0);
    lcd.print("Tiempo restante");        //Mostramos mensaje de tiempo restante

    lcd.setCursor(4, 1);
    if (ahoras < 10) lcd.print("0");     // Si las horas son menor que 10, pone un "0" delante.
    lcd.print(ahoras);                   // Sin este codigo, se muestra asi: H:M:S  (1:M:S)
    lcd.print(":");

    if (aminutos < 10) lcd.print("0");   // Si los minutos son menor que 10, pone un "0" delante.
    lcd.print(aminutos);                 // Sin este codigo, se muestra asi: H:M:S  (H:1:S)

    lcd.print(":");
    if (asegundos < 10) lcd.print("0");  // si el valor de segundo esta por debajo de 9 (unidad) antepone un cero
    lcd.print(asegundos);                // Sin este codigo, se muestra asi: H:M:S  (H:M:1)

    if (segundostotal == 0)  //Si finaliza el tiempo
    {
      digitalWrite(13,HIGH);
      while (1)                       //Bucle infinito mostrando mensaje y haciendo parpadear un led
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("RETIRAR PANTALLA");
        lcd.setCursor(1, 1);
        lcd.print(" *FIN PROCESO");

        //digitalWrite(buzzer, HIGH);
   //      tone(9,293.66,200);
 // delay(200);
  //tone(9,293.66,100);
  //delay(100);
  //tone(9,293.66,200);
  //delay(200);
  //tone(9,293.66,100);
 // delay(100);
  //tone(9,293.66,200);
  //delay(200);
  //tone(9,293.66,100);
 // delay(100);
 // tone(9,293.66,100);
 // delay(100);
 // tone(9,293.66,100);
  //delay(100);
  //tone(9,293.66,100);
  //delay(100);
  //tone(9,293.66,200);
  //delay(200);
  //tone(9,293.66,100);
  //delay(100);
  //tone(9,293.66,200);
  //delay(200);
  //tone(9,293.66,100);
  //delay(100);
  //tone(9,293.66,200);
  //delay(200);
  //tone(9,293.66,100);
  //delay(100);
  //tone(9,293.66,100);
  //delay(100);
  //tone(9,293.66,100);
  //delay(100);
 // tone(9,293.66,100);
 // delay(100);
 // tone(9,293.66,200);
  //delay(200);
  //tone(9,293.66,100);
  //delay(100);
  //tone(9,293.66,200);
  //delay(200);
  //tone(9,293.66,100);
  //delay(100);
  tone(9,293.66,200);
  delay(200);
  tone(9,293.66,100);
  delay(100);
  tone(9,293.66,100);
  delay(100);
  tone(9,440,100);
  delay(100);
  tone(9,523.25,100);
  delay(100);
  tone(9,587.33,100);
  delay(200);
  tone(9,587.33,100);
  delay(200);
  tone(9,587.33,100);
  delay(100);
  tone(9,659.25,100);
  delay(100);
  tone(9,698.45,100);
  delay(200);
  tone(9,698.45,100);
  delay(200);
  tone(9,698.45,100);
  delay(100);
  tone(9,783.99,100);
  delay(100);
  tone(9,659.25,100);
  delay(200);
  tone(9,659.25,100);
  delay(200);
  tone(9,587.33,100);
  delay(100);
  tone(9,523.25,100);
  delay(100);
  tone(9,523.25,100);
  delay(100);
  tone(9,587.33,100);
  delay(300);
  tone(9,440,100);
  delay(100);
  tone(9,523.25,100);
  delay(100);
  tone(9,587.33,100);
  delay(200);
  tone(9,587.33,100);
  delay(200);
  tone(9,587.33,100);
  delay(100);
  tone(9,659.25,100);
  delay(100);
  tone(9,698.45,100);
  delay(200);
  tone(9,698.45,100);
  delay(200);
  tone(9,698.45,100);
  delay(100);
  tone(9,783.99,100);
  delay(100);
  tone(9,659.25,100);
  delay(200);
  tone(9,659.25,100);
  delay(200);
  tone(9,587.33,100);
  delay(100);
  tone(9,523.25,100);
  delay(100);
  tone(9,587.33,100);
  delay(400);
  tone(9,440,100);
  delay(100);
  tone(9,523.25,100);
  delay(100);
  tone(9,587.33,100);
  delay(200);
  tone(9,587.33,100);
  delay(200);
  tone(9,587.33,100);
  delay(100);
  tone(9,698.45,100);
  delay(100);
  tone(9,783.99,100);
  delay(200);
  tone(9,783.99,100);
  delay(200);
  tone(9,783.99,100);
  delay(100);
  tone(9,880,100);
  delay(100);
  tone(9,932.33,100);
  delay(200);
  tone(9,932.33,100);
  delay(200);
  tone(9,880,100);
  delay(100);
  tone(9,783.99,100);
  delay(100);
  tone(9,880,100);
  delay(100);
  tone(9,587.33,100);
  delay(300);
  tone(9,587.33,100);
  delay(100);
  tone(9,659.25,100);
  delay(100);
  tone(9,698.45,100);
  delay(200);
  tone(9,698.45,100);
  delay(200);
  tone(9,783.99,100);
  delay(200);
  tone(9,880,100);
  delay(100);
  tone(9,587.33,100);
  delay(300);
  tone(9,587.33,100);
  delay(100);
  tone(9,698.45,100);
  delay(100);
  tone(9,659.25,100);
  delay(200);
  tone(9,659.25,100);
  delay(200);
  tone(9,698.45,100);
  delay(100);
  tone(9,587.33,100);
  delay(100);
  tone(9,659.25,100);
  delay(400);
  tone(9,880,100);
  delay(100);
  tone(9,1046.50,100);
  delay(100);
  tone(9,1174.66,100);
  delay(200);
  tone(9,1174.66,100);
  delay(200);
  tone(9,1174.66,100);
  delay(100);
  tone(9,1318.51,100);
  delay(100);
  tone(9,1396.91,100);
  delay(200);
  tone(9,1396.91,100);
  delay(200);
  tone(9,1396.91,100);
  delay(100);
  tone(9,1567.98,100);
  delay(100);
  tone(9,1318.51,100);
  delay(200);
  tone(9,1318.51,100);
  delay(200);
  tone(9,1174.66,100);
  delay(100);
  tone(9,1046.50,100);
  delay(100);
  tone(9,1046.50,100);
  delay(100);
  tone(9,1174.66,100);
  delay(300);
  tone(9,880,100);
  delay(100);
  tone(9,1046.50,100);
  delay(100);
  tone(9,1174.66,100);
  delay(200);
  tone(9,1174.66,100);
  delay(200);
  tone(9,1174.66,100);
  delay(100);
  tone(9,1318.51,100);
  delay(100);
  tone(9,1396.91,100);
  delay(200);
  tone(9,1396.91,100);
  delay(200);
  tone(9,1396.91,100);
  delay(100);
  tone(9,1567.98,100);
  delay(100);
  tone(9,1318.51,100);
  delay(200);
  tone(9,1318.51,100);
  delay(200);
  tone(9,1174.66,100);
  delay(100);
  tone(9,1046.50,100);
  delay(100);
  tone(9,1174.66,100);
  delay(400);
  tone(9,880,100);
  delay(100);
  tone(9,1046.50,100);
  delay(100);
  tone(9,1174.66,100);
  delay(200);
  tone(9,1174.66,100);
  delay(200);
  tone(9,1174.66,100);
  delay(100);
  tone(9,1396.91,100);
  delay(100);
  tone(9,1567.98,100);
  delay(200);
  tone(9,1567.98,100);
  delay(200);
  tone(9,1567.98,100);
  delay(100);
  tone(9,1760,100);
  delay(100);
  tone(9,1864.66,100);
  delay(200);
  tone(9,1864.66,100);
  delay(200);
  tone(9,1760,100);
  delay(100);
  tone(9,1567.98,100);
  delay(100);
  tone(9,1760,100);
  delay(100);
  tone(9,1174.66,100);
  delay(300);
  tone(9,1174.66,100);
  delay(100);
  tone(9,1318.51,100);
  delay(100);
  tone(9,1396.91,100);
  delay(200);
  tone(9,1396.91,100);
  delay(200);
  tone(9,1567.98,100);
  delay(200);
  tone(9,1760,100);
  delay(100);
  tone(9,1174.66,100);
  delay(300);
  tone(9,1174.66,100);
  delay(100);
  tone(9,1396.91,100);
  delay(100);
  tone(9,1318.51,100);
  delay(200);
  tone(9,1318.51,100);
  delay(200);
  tone(9,1174.66,100);
  delay(100);
  tone(9,1108.73,100);
  delay(100);
  tone(9,1174.66,100);
  delay(200);
  tone(9,1174.66,100);
  delay(200);
  tone(9,1318.51,100);
  delay(200);
  tone(9,1396.91,100);
  delay(200);
  tone(9,1396.91,100);
  delay(100);
  tone(9,1396.91,100);
  delay(100);
  tone(9,1567.98,100);
  delay(200);
  tone(9,1760,300);
  delay(400);
  tone(9,1396.91,100);
  delay(100);
  tone(9,1174.66,100);
  delay(100);
  tone(9,880,300);
  delay(600);
  tone(9,1864.66,300);
  delay(400);
  tone(9,1396.91,100);
  delay(100);
  tone(9,1174.66,100);
  delay(100);
  tone(9,932.33,300);
  delay(600);
  tone(9,587.33,100);
  delay(100);
  tone(9,440,100);
  delay(200);
  tone(9,587.33,100);
  delay(300);
  tone(9,554.36,100);
  delay(400);
  tone(9,1567.98,100);
  delay(100);
  tone(9,1567.98,100);
  delay(100);
        //sonarTono(TONO_ERROR, TIME_INTERVAL);
        //delay(500);
        //digitalWrite(buzzer, LOW);
        //delay(200);
      }
    }
  }
}

  //------------------------------------
  // SONAR TONO
  //------------------------------------
  void sonarTono(int tono, int duracion)
  {
    tone(buzzer, tono, duracion);
    delay(duracion);
  }
