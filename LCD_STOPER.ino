#include <LiquidCrystal.h>            //Załączenie biblioteki
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);  //Informacja o podłączeniu wyświelacza do danych pinów

#define start 13                      // Zdefiniowanie nazw przycisków
#define stop  12

int dziesiata_sekundy;    // Licznik 
int sekunda;
int minuta;

int tryb=2;               // Tryb pracy 
                          // 0 = Pomiar czasu
                          // 1 = Wynik
                          // 2 = Gotowy do pracy
                          
int kursor_minuta;                  // Położenie kursora dla sekund, minut
int kursor_sekunda;
int przycisk_wcisniety(int przycisk); // Funkcja obsługująca naciskanie przycisków
void pomiar_wyswietlanie(int minuty, int sekundy, int dziesiate_sekundy); // Funkcja obsługująca wyświetlanie drugiego wersa wyświetlacza


unsigned long aktualny_czas  = 0;
unsigned long poprzedni_czas = 0;
unsigned long roznica_czasu  = 0;

int wynik_minuta;
int wynik_sekunda;
int wynik_dziesiata_sekundy;
 
void setup() 
{
  pinMode(start,INPUT_PULLUP);  // Przyciski jako wejścia
  pinMode(stop,INPUT_PULLUP);
  
  lcd.begin(16, 2);     // Deklaracja typu wyświetlacza
  lcd.clear();          // Czyszczenie zawartości wyświetlacza
  lcd.setCursor(0,0);   // Ustawianie kursora
  lcd.print("STOPER");  // Napis powitalny
  delay (3000);
}
 
void loop()
{
  if (przycisk_wcisniety(start))tryb=0;
  if (przycisk_wcisniety(stop)) tryb++;
  if (tryb>2) tryb =1;
  aktualny_czas = millis();
  roznica_czasu = aktualny_czas - poprzedni_czas;

    if (roznica_czasu>=100UL) // Jeśli różnica czasu wynosi 1/10 sekundy wykonaj wszystkie instrukcje poniżej, dzięki temu wyświetlacz nie miga!
        {
         dziesiata_sekundy ++;
         if (dziesiata_sekundy>9){sekunda++;dziesiata_sekundy=0;}
         if (sekunda>59){minuta++;sekunda=0;}
         poprzedni_czas=aktualny_czas;
                      
              if (tryb==0) // Pomiar czasu
              {
               lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("Pomiar w trakcie");
                pomiar_wyswietlanie(minuta, sekunda, dziesiata_sekundy);
                wynik_minuta=minuta;
                wynik_sekunda=sekunda;
                wynik_dziesiata_sekundy=dziesiata_sekundy;
              }
  
               if (tryb==1) // Wynik
              {
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("Wynik");
                pomiar_wyswietlanie(wynik_minuta, wynik_sekunda, wynik_dziesiata_sekundy);
                poprzedni_czas=millis();               
                minuta=wynik_minuta;
                sekunda=wynik_sekunda;
                dziesiata_sekundy=wynik_dziesiata_sekundy;
              }
              
              if (tryb==2) //Gotowy do pracy
              { 
                minuta=sekunda=dziesiata_sekundy=0;
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("Gotowy do pracy");
                pomiar_wyswietlanie(minuta, sekunda, dziesiata_sekundy);
                
              }
        }                
}

int przycisk_wcisniety(int przycisk)
{
  if (digitalRead(przycisk)==0)
      {
        delay(100);
        if(digitalRead(przycisk)==0)return 1;
      }
  return 0;
}

void pomiar_wyswietlanie(int minuty, int sekundy, int dziesiate_sekundy)
{
      if (minuty>9) kursor_minuta=5; // Zmiana położenia kursora dla minut w momencie gdy wartosc staje się dwucyfrowa
          else    
          {kursor_minuta=6;          // Zapewnienie wyświetlania cyfr jednosci w formacie 01, 02, 03, etc.
           lcd.setCursor(5,1);
           lcd.print(0);} 

      
      if (sekundy>9) kursor_sekunda=8;   // Zmiana położenia kursora dla sekund w momencie gdy wartosc staje się dwucyfrowa
          else    
          {kursor_sekunda=9;            // Zapewnienie wyświetlania cyfr jednosci w formacie 01, 02, 03, etc.
           lcd.setCursor(8,1);
           lcd.print(0);}

      lcd.setCursor(0,1);               // Czas
      lcd.print("Czas:");
             
      lcd.setCursor(kursor_minuta,1);   // Minuty
      lcd.print(minuty);

      lcd.setCursor(7,1);               // Dwukropek
      lcd.print(":");
      
      lcd.setCursor(kursor_sekunda,1);  // Sekundy
      lcd.print(sekundy);
      
      lcd.setCursor(10,1);               // Dwukropek
      lcd.print(":");  
      
      lcd.setCursor(11,1);               // Dziesiętne części sekundy 
      lcd.print(dziesiate_sekundy);
}











