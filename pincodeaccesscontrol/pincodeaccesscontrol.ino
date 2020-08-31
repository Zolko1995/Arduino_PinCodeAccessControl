#include <Keypad.h> //A 4x4-es billentyűzethez használt könyvtár beillesztése
#include<Wire.h> //Wire könyvtár beillesztése az I2C busz használatához
#include <LiquidCrystal_I2C.h> //Az I2C Folyékony kristályos LCD kijelző kezelő könyvtára
LiquidCrystal_I2C lcd(0x27, 16, 2); //Az általunk használt kijelző karakterkészlete 16 karakter és 2 sor

#define Password_Lenght 7 //A jelszó hosszának defiálása 6 karakter + NULL karakter

char Data[Password_Lenght]; // 6 számot tartalmazhat + NULL karakter így 7 karakter hosszú
char Master[Password_Lenght] = "123456"; //A Master jelszó
byte data_count = 0, master_count = 0;
bool Pass_is_good; //Bináris típusu változó mely a jelszó helyességét igazolja
char customKey; //A felhasználó által lenyomott billentyűnek megfelelő ASCII kodot tartalmazó változó
const byte ROWS = 4; //Konstans byte típusú változó mely a billentyűzet sorait számozza
const byte COLS = 4; //Konstans byte típusú változó mely a billentyűzet oszlopait számozza

char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'}, //Két dimenziós a karaktereket tartalmazó tömb
  {'4', '5', '6', 'B'}, //Két dimenziós a karaktereket tartalmazó tömb
  {'7', '8', '9', 'C'}, //Két dimenziós a karaktereket tartalmazó tömb
  {'*', '0', '#', 'D'}  //Két dimenziós a karaktereket tartalmazó tömb
};

bool kapu = true; //A kapu nyitását igazoló bináris típusú változó
int zar = 12; //Az elektromos zár pinjét tartalmazó változó
byte rowPins[ROWS] = {2, 3, 4, 5}; //A pinek melyek a sorokra csatlakoznak
byte colPins[COLS] = {6, 7, 8, 9}; //A pinek melyek az oszlopokra csatlakoznak

Keypad customKeypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS); //initialize an instance of class NewKeypad

void setup()
{
  pinMode(zar, OUTPUT); //A zárra csatlakoztatott pin kimenetté alakítása
  lcd.init(); //Az LCD kijelző inicializálása
  lcd.backlight(); //Az LCD kijelző háttérvilágításának bekapcsolása
  lcd.setCursor(2, 0); //Kurzor pozicionálás ez esetben 0. karakter a 0. sorban
  lcd.print("UDVOZOLJUK!"); //Megadott karakterlánc kiíratása
  delay(3000); //Várakozás 3 másodpercig
  lcd.clear(); //Az LCD kijelző tartalmának a törlése
}

void loop() //ciklus
{
  if (kapu == 0) //Feltétel vizsgálat:
  {
    customKey = customKeypad.getKey(); //Karakterek beolvasása
    if (customKey == '#') //Feltétel vizsgálat:
    {
      lcd.clear(); //Az LCD kijelző tartalmának a törlése
      digitalWrite(zar, LOW); //A zár kikapcsolt állapotának megőrzése
      lcd.print("KAPU ZARVA"); //Megadott karakterlánc kiíratása
      delay(3000); //Várakozás 3 másodpercig
      lcd.clear(); //Az LCD kijelző tartalmának a törlése
      kapu = 1;    //A kapu változó értékének IGAZRA vagy 1-re változtatása
    }
  }
  else nyitas(); //Egyébként, nyitas funkció meghívása
}

void clearData() //Adattörlés funkció az adatokat tartalmaző tömb törlésére
{
  while (data_count != 0) //Előtesztelő ciklus amíg: data_count nem egyenlő 0-val addig:
  {
    Data[data_count--] = 0; //A tömb elemeinek törlése
  }
  return; //Kilépés a ciklus magjából
}

void nyitas() //Nyitas funkció
{
  lcd.setCursor(0, 0); //Kurzor pozicionálás ez esetben 0. karakter a 0. sorban
  lcd.print("A JELSZO:"); //Megadott karakterlánc kiíratása
  customKey = customKeypad.getKey(); //Karakterek beolvasása
  if (customKey) //A billentyű lenyomásának vizsgálata megfelel ennek (customKey != NO_KEY)
  {
    Data[data_count] = customKey; //A karakterek tömbben tárolása
    lcd.setCursor(data_count, 1); //Kurzor pozicionálás a tömbre az 1. sorban
    lcd.print(Data[data_count]); //Karakter kiíratása
    data_count++; //A jelszót tartalmazó tömb hosszának inkrementálása 1 elemmel
  }
  if (data_count == Password_Lenght - 1) //Feltétel vizsgálat miszerint, ha a megadott jelszó hossza megfelelő akkor hasonlitás a Master-hez
  {
    if (!strcmp(Data, Master)) //String Comparison
    {
      lcd.clear(); //Az LCD kijelző tartalmának a törlése
      delay(500); //Várakozás 500 milliszekundum ideig
      digitalWrite(zar, HIGH); //A zár nyitása
      lcd.print("KAPU NYITVA"); //Megadott karakterlánc kiíratása
      kapu = 0; //A kapu változó értékének HAMISA vagy 0-ra változtatása
    }
    else
    {
      lcd.clear(); //Az LCD kijelző tartalmának a törlése
      delay(500); //Várakozás 500 milliszekundum ideig
      digitalWrite(13, LOW); //A zár zárása
      lcd.print("HELYTELEN JELSZO"); //Megadott karakterlánc kiíratása
      delay(1000); //Várakozás 1 másodpercig
      kapu = 1; //A kapu változó értékének IGAZRA vagy 1-re változtatása
    }
    clearData(); //Adattörlés funkció hívása
  }
}
