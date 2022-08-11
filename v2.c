#include <stdio.h>
#include <stdlib.h>

#define MAXN 1000004

//komentarze na podstawie zadania wzorcowego z książeczek.

int n; 							// ilosc pracownikow
int ile_posrednich[MAXN]; 		//w tej tablicy wrzucamy ilosc WSZYSTKICH podwładnych danego pracownika
int bezp_podw[MAXN]; 			// jesli bezposredni bezp_podw (jezeli tylko jeden) to np. bezp_podw[1]=3. bezp_podw[3]=0.  jeśli mamy więcej niz 1 to np. podwaldny[6]=-1 i podwaldny[5]=-1 (przyklad wzorcowy)  
int przelozony[MAXN];
int pensja[MAXN]; 
int ilosc_bezp_pod[MAXN]; 		//ile pracownik ma podwaładnych BEZPOŚREDNICH (tych zaraz pod nim);
int prac_konc[MAXN]; 			//do tej tablicy wrzucamy indeksy pracownikow ktorzy sa na samym koncu (nie mają szefów)
int szefowie_poczatkowi[MAXN];  //pracownik z ustalona pensja, bezposrednio po ktorym sa nieustalone.
int indeks_pensji[MAXN]; 		//wrzucanie do tablicy pracownikow przy czym indeksy wskazują na ich pensje
				  				// jesli chcemy sprawdzic kto ma pensje = 10 to robimy indeks_pensji[10] i wiemy ze indeks_pensji[10] = 2. (przyklad wzorcowy)


int compare( const void* a, const void* b)
{
     int int_a = * ( (int*) a );
     int int_b = * ( (int*) b );

     if ( int_a == int_b ) return 0;
     else if ( int_a < int_b ) return -1;
     else return 1;
}

void indeksuj_pracownikow() 
{
	int indeks_startowy = 0;
	int indeks_koncowy = 0;
    for (int i = 1; i < n; ++i) 
	{
		ilosc_bezp_pod[i] = 0;	
	}
    for (int i = 1; i < n; ++i)
    {
    	ilosc_bezp_pod[przelozony[i]]++;
	}   									//zliczamy ile dany pracownik ma podwładnych (ale tylko bezposrednich(!)) np ilosc_bezp_pod[4]=1 a ilosc_bezp_pod 6=2 (przyklad wzorcowy)
        									//jesli przelozony[i]=11 to po prostu znaczy ze wyzej wyszystkie pensje są znane (przyklad wzorcowy)
        
    for (int i = 1; i < n; ++i)
    {
        if (ilosc_bezp_pod[i] == 0) 		//ile podwladnych jest bezposrednio pod danym numerem
        { 					 
        	prac_konc[indeks_koncowy] = i;	//jezli 0 to do tej tablicy wrzucamy indeksy pracownikow ktorzy sa na samym koncu 
        	indeks_koncowy++; 				//zliczamy tez ich ilosc
		}
 	}
    while (indeks_startowy < indeks_koncowy) 
	{
        int temp = prac_konc[indeks_startowy];//temp - aktywny pracownik (teraz rozpatrywany)
        indeks_startowy++;
        int przel = przelozony[temp]; 		//przelozozony aktywnego pracownika
        if (pensja[temp] == 0) 				//jesli pensja aktywnego pracownika = 0
		{	
			ilosc_bezp_pod[przel] -= 1; 	//stopien przelozonego aktpywnego numeru -1
            if (ilosc_bezp_pod[przel]==0) 
			{
			prac_konc[indeks_koncowy] = przel;//w dalszych komorkah kolejki wrzucamy szefow tych z pierwszych komorek
			indeks_koncowy++;
			}								  //w tej tablicy wrzucamy ilosc bezp_podwch jaka maja szefowie
            ile_posrednich[przel] += ile_posrednich[temp] + 1; 
        }									 //np. ile_posrednich[4]=3 (przyklad wzorcowy)
    }
}

void indeksuj_pensje() {
    for (int i = 1; i < n; ++i)
    {
        if (pensja[i] != 0)
        {
        	indeks_pensji[pensja[i]] = i; 		//po prostu osoby ktore maja pensje wszucamy do tablicy indeks_pensji na ich indeksach  	
		}   
        else if (bezp_podw[przelozony[i]] == 0) //na poczatku kazdy podwladny[] = 0
        {
        	bezp_podw[przelozony[i]] = i; 		//tablica do ktorej wrzucamy podwaldnych np. podwaldny[1]=3. (przyklad wzorcowy)
		}     
        else 
			bezp_podw[przelozony[i]] = -1; 		//system detekcji więcej niż 1 podwładnego, jeśli mamy więcej niz 1 to podwaldny[6]=-1 i podwaldny[5]=-1  (przyklad wzorcowy)
    }
}
int main() 
{
    scanf("%d", &n);
    for (int i = 1; i <= n; ++i) {
        scanf("%d %d", przelozony + i, pensja + i);
        if (przelozony[i] == i) pensja[i] = n;
        if (pensja[i]) przelozony[i] = n+1; //z rowziązania wzorcowego, potrzebne do przypadku gdy mamy 1 pracownika.
    }
    n++;
    przelozony[n+1] = n; 					//dodawanie fikcyjnego dyrektora dyrektora
    pensja[n] = n; 		 					//nadanie mu najwyzszej pensji
    
    indeksuj_pracownikow();
    indeksuj_pensje();
    
    int i = 0;
    int ile_wolnych = 0, ile_mozliwych = 0;
    while (i < n-1) 
	{
		
        while (i < n-1 && indeks_pensji[i+1] == 0) 	//jesli np i=1 i indeks_pensji[1]=0 (nie wiadomo kto ma pensje=1) (dane z drzewka wzorcowego)
		{
		 i++;																   
		 ile_wolnych++; 							// ++ ilosc wolnych slotow
		 ile_mozliwych++;
		}
        while (i < n-1 && indeks_pensji[i+1] != 0) 	//jesli np i=2 i indeks_pensji[2]=1 (pensje = 2 ma pracownik 1) (dane z drzewka wzorcowego)
		{
            i++; //if(i<n-1) break
			int temp = i;
			int akt = indeks_pensji[i];  			//aktywny pracownik 
            ile_wolnych -= ile_posrednich[akt]; 	//tu np. pracownik 1 ma 1 podwladnego wiec nie mamy dylematu ktoremu mozna okreslic pensje (dane z drzewka wzorcowego)
            if (ile_wolnych == 0) 					//jesli mozemy przypisac pensje podwladnemu
			{
				
                while (ile_mozliwych != 0 && bezp_podw[akt] > 0) 
				{
                    while (indeks_pensji[temp] != 0) 
					{
					temp--;							 //temp-- ustawia sie na pracownika kotry powinien zarabiac *(o 1 mniej)* niz pracownik *(chyba, że jest już pracownik któy zarabia 1 mniej, wtedy o 2 mniej.. itd.)*
					}
                     akt = bezp_podw[akt]; 			 //ustawia jaki aktywnego tego podwladnego nizej. (pojedynczego)
                     pensja[akt] = temp; 			 //ustawia pensje podwladnego; (definicja temp wyżej)
                     indeks_pensji[temp] = akt;		 //w tablicy indeks_pensji aktualizuje zarobki: np. ustawia indeks_pensji[1] na 3 (ustalono przed chwilą, że pracownik 3 zarabia 1 :))
                     ile_mozliwych--; 				 //odejmujemy mozliwosci, bo ustalilismy juz ile zarabia kazdy z podladnych (w tym przypadku 1)
                }
                ile_mozliwych = 0;
            }
            if (ile_posrednich[akt] != 0) 			//jesli 4, a co za tym idzie 6 pozostali jeszcze jacys podwladni, to nie mozemy im usalic pensji. (dane z drzewka wzorcowego)
            {
            	for(int j=0; j<n-1; j++)
				{
					
					ile_mozliwych = 0;
				}
			}
                
        }
    }
    for (int i = 1; i < n; ++i)
        printf("%d\n", pensja[i]); 
    return 0;
}