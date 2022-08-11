#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>



#define MAXN 1000000

int n; 							// ilosc pracownikow
int ile_posrednich[MAXN]; 		//w tej tablicy wrzucamy ilosc WSZYSTKICH podwładnych danego pracownika
int bezp_podw[MAXN]; 			// jesli bezposredni bezp_podw (jezeli tylko jeden) to np. bezp_podw[1]=3. bezp_podw[3]=0.  jeśli mamy więcej niz 1 to np. podwaldny[6]=-1 i podwaldny[5]=-1 
int przelozony[MAXN];
int pensja[MAXN]; 
int ilosc_bezp_pod[MAXN]; 		//ile pracownik ma podwaładnych BEZPOŚREDNICH (tych zaraz pod nim);
int prac_konc[MAXN]; 			//do tej tablicy wrzucamy indeksy pracownikow ktorzy sa na samym koncu (nie mają szefów)
int szefowie_poczatkowi[MAXN]; //pracownik z ustalona pensja, bezposrednio po ktorym sa nieustalone.
int indeks_pensji[MAXN]; 		//wrzucanie do tablicy pracownikow przy czym indeksy wskazują na ich pensje
				  				//w skrocie jesli chcemy sprawdzic kto ma pensje = 10 to robimy indeks_pensji[10] i wiemy ze indeks_pensji[10] = 2.



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
void indeksuj_pensje() 
{
	
    for (int i = 1; i < n; ++i)
    {
        if (pensja[i] != 0)
        {
        	indeks_pensji[pensja[i]] = i; //wrzucanie do tablicy pracownikow przy czym indeksy wskazują na ich pensje
		   					  	  //w skrocie jesli chcemy sprawdzic kto ma pensje = 10 to robimy indeks_pensji[10] i wiemy ze indeks_pensji[10] = 2.
			
		}
        else if (bezp_podw[przelozony[i]] == 0) //na poczatku kazdy podwdwladny[] = 0
        {
        	bezp_podw[przelozony[i]] = i; //tablica do ktorej wrzucamy	podwaldnych np. podwaldny[1]=3. 
        	
		}     
        else 
        {	
        	if(bezp_podw[przelozony[i]]>0)
        		bezp_podw[przelozony[i]]=1; 
        	
			bezp_podw[przelozony[i]] -= 2; //system detekcji więcej niż 1 podwładnego, jeśli mamy więcej niz 1 to podwaldny[6]=-1 i podwaldny[5]=-1 
    	}
		
	}
        	
}

void usun_podwladnych(int i)
{
	for(int j=1; j <= ile_posrednich[bezp_podw[i]]; j++)
	{					
		if(przelozony[indeks_pensji[pensja[bezp_podw[i]]-j]] == n+1 ) //or indeks_pensji{pensja... <0
			ile_posrednich[bezp_podw[i]]++;	
		else
			indeks_pensji[pensja[bezp_podw[i]]-j] =- 1;						
	}		
}

int main() 
{
	
	
		scanf("%d", &n);
    for (int i = 1; i <= n; ++i) 
	{
        scanf("%d %d", przelozony + i, pensja + i);
        if (przelozony[i] == i) pensja[i] = n;
        if (pensja[i]) przelozony[i] = n+1;
    }
		
 	n++;
    przelozony[n+1] = n; //dodawanie fikcyjnego dyrektora dyrektora
    pensja[n] = n; //nadanie mu najwyzszej pensji
    
    indeksuj_pracownikow();
    indeksuj_pensje();
    
    //kod do obliczania ilosci pracownikow
    n--;
    
    
    //wstępny pomysl jest taki ze numerki ktorych w indeks_pensji[] ktore nas nie obchodza maja warosc -1 a te wazne maja wartosc odpowiednia.
    //przy czym zaczynamy od korzeni mających najwyzsze wartości szefow poczatkowych.
    
    int cntr=0;
    
    
    for(int i=1; i <= n; i++)
    {
    	if(przelozony[i] == (n+1) && pensja[i] !=0 && ile_posrednich[i] != 0)
    	{
	    	szefowie_poczatkowi[cntr]=pensja[i];
	    	cntr++;
    	}
	}
	
	qsort(szefowie_poczatkowi, cntr, sizeof(int), compare); 
	
	//uzyskujemy posortowaną listę początkowych zarobkow
	
	int i=0;
	int usun=0;
	int pensja_minus=0;
    
    while(cntr>0)
    {
    	
    	
    	i=(indeks_pensji[szefowie_poczatkowi[cntr-1]]); //iterujemy po szedfach poczatkowych od najwiecej zarabiajacego do najmniej
    	if(i <= 0)
    	{
    	//printf("no, %d\n", szefowie_poczatkowi[cntr-1]);	
		}
    	
    	cntr--;	
    	
    	if(przelozony[i] == (n+1) && pensja[i] !=0 && ile_posrednich[i] != 0) //dodatkowy warunek w razie bledu [POTEM USUN]
    	{
	    	
		    if(bezp_podw[i]>0)	
			{
					
				pensja_minus=1; //iterator zeby schodzic po pensjach nizej w przypadku mozliwosci poznania pensji pracownika nizej
				
				while(bezp_podw[i] > 0)	
				{	
				if(bezp_podw[i])
					
					if(indeks_pensji[pensja[i]-pensja_minus] == -1)
					{	
						for(int j=1; j <= ile_posrednich[i]; j++)
						{
							
							if(przelozony[indeks_pensji[pensja[i]-1-j]] == n+1 ) //or indeks_pensji{pensja... <0
								ile_posrednich[i]++;	
							else
								indeks_pensji[pensja[i]-1-j]=-1;
							
						}	
						break;
					}
					if(indeks_pensji[pensja[i]-pensja_minus] == 0) 
					{
						indeks_pensji[pensja[i]-pensja_minus] = bezp_podw[i];
						
						if(bezp_podw[bezp_podw[i]]<0)
							usun_podwladnych(i);
							
						bezp_podw[i]=bezp_podw[bezp_podw[i]];
					}
					else if(przelozony[indeks_pensji[pensja[i]-pensja_minus]] != (n+1) )
					{	
						usun = (ile_posrednich[indeks_pensji[pensja[i]-pensja_minus]]+1);
						for(int j=1; j <= usun ; j++)
						{
							if(przelozony[indeks_pensji[pensja[i]-j]] == (n+1))
							{
								usun++;	
							}
							else
								indeks_pensji[pensja[i]-j] = -1; 
						}
						for(int j=1; j <= ile_posrednich[i]; j++) 
						{
							if(przelozony[indeks_pensji[pensja[i]-1-j]] == n+1) //or indeks_pensji{pensja... <0
								ile_posrednich[i]++;	
							else
								indeks_pensji[pensja[i]-1-j]=-1;
						}
						
					}
					
				pensja_minus++;	
				}
			}
			if(bezp_podw[i]<0)	
			{
			    for(int j=0 ; j <= (-((bezp_podw[i]-1)/2)); j++)
				{	
					while(indeks_pensji[pensja[i]] != 0)
					{
						pensja[i]--;	
					}														
					indeks_pensji[pensja[i]] = -1;	
				} 
			}
		}	
	}
   
for (int i = 1; i < n; ++i)
{
	if(indeks_pensji[i]<0)
		indeks_pensji[i]=0;
}

for (int i = 1; i <= n; i++)
{
	if(indeks_pensji[i] != 0)
	{
		pensja[indeks_pensji[i]]=i;
	}	
}
 
for (int i = 1; i <= n; i++)  
    printf("%d \n", pensja[i]); 
   
        return 0;
}
    
    
	

	
	
   
    
    
    
    