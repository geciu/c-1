#include <iostream>
#include <iomanip>
#include <string>
#include <cstdio>
#include <cmath>
#include <time.h>
#include <stdio.h>
#include <string.h>

using namespace std;

const bool bTest = true;

const long DATA_PP = 20170201;				// data początkowa projektu w formacie rrrrmmdd
const int MX_I_ZAD = 50;					// maksymalna ilość zadań
const int MX_I_CZAS_ZAD = 40;				// maksymalny czas trwania zadania
const int MX_I_ZAD_POP = 3;					// maksymalna ilość zadań poprzedzających
const int MX_C_NAZ_PROJ = 20;				// maksymalna długość nazwy projektu
const int MX_C_NAZ_ZAD = 10;				// maksymalna długość nazwy zadania

long db = 0;								// data rozpoczęcia ostatniego zaplanowanego zadania

struct zadanie {
	int		nr;								// nr zadania
	char	nazwa[MX_C_NAZ_ZAD + 1];		// nazwa zadania
	int		dni;							// czas trwania zadania
	int		zp1;							// nr zadania poprzedzającego nr 1
	int		zp2;							// nr zadania poprzedzającego nr 2
	long 	rmdp;							// start zadania rrrrmmdd
	long	rmdk;							// koniec zadania rrrrmmdd
	int	z;									// znacznik: czy zadanie włączone do harmonogramu	
};

struct projekt {
	char	nazwa[MX_C_NAZ_PROJ + 1];		// nazwa projektu
	int		ilzad;							// ilość zadań projektu
	long	czasp;							// data rozpoczęcia projektu
	long	czask;							// data zakończenia projektu - do wyliczenia
};

void ustaw_date(time_t * t1, int prk, int pmc, int pdn) {
	struct tm * d;
	d = localtime(t1);
	d->tm_year = prk - 1900;
	d->tm_mon = pmc - 1;
	d->tm_mday = pdn;
	*t1 = mktime(d);
	return;
}

long itod(int prk, int pmc, int pdn) {				// utworzenie daty w formacie rrrrmmdd
	return 10000 * prk + 100 * pmc + pdn;
}

int ddn(long d2) {									// wyodrębnienie dni z daty w formacie rrrrmmdd
	return d2 % 100;
}

int dmc(long d2) {									// wyodrębnienie miesiąca z daty w formacie rrrrmmdd
	long w;
	w = floor(d2 / 100);
	return w % 100;
}

int drk(long d2) {									// wyodrębnienie roku z daty w formacie rrrrmmdd
	return floor(d2 / 10000);
}

long dodaj_dni2(long d2, int dn) {					//  dodanie dni do daty w formacie rrrrmmdd
	time_t	c;
	struct tm * d;
	time(&c);
	ustaw_date(&c, drk(d2), dmc(d2), ddn(d2));
	d = localtime(&c);
	c = mktime(d) + 3600 * 24 * dn;
	d = localtime(&c);
	return (d->tm_year + 1900) * 10000 + (d->tm_mon + 1) * 100 + d->tm_mday;
}

void dodaj_dni(time_t * t1, time_t * t2, int pdni) {
	*t2 = *t1 + pdni * 3600 * 24;
}

void wypisz_zadanie(zadanie *wskz) {
	cout << setw(4) << wskz->nr << setw(12) << wskz->nazwa << setw(6) << wskz->dni << setw(5) << wskz->zp1 << setw(5) << wskz->zp2 << setw(12) << wskz->rmdp << setw(12) << wskz->rmdk << setw(3) << wskz->z << endl;
	return;
}

void wypisz_nagz() {								// wypisz nagłówek zadania
	cout << setw(4) << "[Nr] " << setw(12) << "  [Nazwa] " << setw(6) << "[Dni] " << setw(5) << "[Zp1] " << setw(5) << "[Zp2]" << setw(12) << "[Start]   " << setw(12) << "[Koniec]  " << endl;
	return;
}

void wypisz_zadania(zadanie *wskz, int n) {			// wypisz wszystkie zadania projektu
	wypisz_nagz();
	for (zadanie *z = wskz; z < wskz + n; z++) {
		if (z->nr >= 0 && z->nr < 50 && z->rmdk != 0)
			wypisz_zadanie(z);
	}
	return;
}

void zeruj_z(zadanie *wskz, int n) {				// wypisz wszystkie zadania projektu
	for (zadanie *z = wskz; z < wskz + n; z++) {
		if (z->nr >= 0 && z->nr < 50)
			z->z = 0;
	}
	return;
}

void wpr_proj_nazwa(projekt P[], int n) {
	char np[MX_C_NAZ_PROJ + 1];
	cout << endl << "Nazwa projektu: ";
	cin >> np;
	if (np != ".") strcpy(P->nazwa, np);
	return;
}
void wpr_proj_ilzad(projekt P[], int n) {
	cout << endl << "Ilosc zadan   : ";
	cin >> P->ilzad;
	if ((P->ilzad > 0) && (P->ilzad < MX_I_ZAD)) db = DATA_PP;
	return;
}
void wypisz_projekt(projekt P[], int n) {
	cout << endl << "Nazwa projektu                  : " << P->nazwa;
	cout << endl << "Planowana ilosc zadan           : " << P->ilzad;
	cout << endl << "Planowana data rozpoczecia      : " << P->czasp;
	cout << endl << "Data zakonczenia wg harmonogramu: " << P->czask;
	P->czask = db;
}

void generuj_zadanie(zadanie Z[], int i) {				// wprowadź parametry zadania
	Z[i].nr = i;
	strcpy(Z[i].nazwa, "Zad.");
	Z[i].dni=Z[i].zp1=Z[i].zp2=0;
	Z[i].rmdp = Z[i].rmdk = db;
	Z[i].z = 0;
}

void generuj_zadania(zadanie Z[], int n, projekt P[]) {
	for (int i = 0; i<P->ilzad; i++) {
		generuj_zadanie(Z, i);
		}
}

void wpr_zadanie(zadanie Z[], int n) {					// wprowadź parametry zadania
	int i;
	char nz[MX_C_NAZ_ZAD + 1];
	cout << "Wprowadzanie parametrow zadania" << endl;
	cout << "Numer (0-49): ";
	cin >> i;
	if (i > 49) {
		cout << endl << "Wprowadzono bledny numer zadania";
		system("pause");
		return;
	}

	Z[i].nr = i;
	cout << endl << "Nazwa zadania: ";
	cin >> nz;
	strcpy(Z[i].nazwa, nz);

	cout << endl << "Pracochlonnosc zadania [dni]: ";
	cin >> Z[i].dni;
	cout << endl << "Nr zad. poprzedzajacego 1   : ";
	cin >> Z[i].zp1;
	cout << endl << "Nr zad. poprzedzajacego 2   : ";
	cin >> Z[i].zp2;
	Z[i].z = false;
}

void zaplanuj_zadanie(zadanie Z[], int n) {								// jeżeli zadania zp1 i zp2 są juz zaplanowane lub == 0 wpisz datę db, do db dodaj długość zadania tzn. ilosc dni w sekundach
	if ((Z[n].zp1 == 0) && (Z[n].zp2 == 0)) {
		Z[n].rmdp = DATA_PP;
	}
	if (((Z[n].zp1 != 0) && (Z[Z[n].zp1].z == 1)) || ((Z[n].zp2 != 0) && (Z[Z[n].zp2].z == 1))) {
		if (Z[Z[n].zp1].rmdk > Z[Z[n].zp2].rmdk) {
			Z[n].rmdp = dodaj_dni2(Z[Z[n].zp1].rmdk, 1);
		}
		else {
			Z[n].rmdp = dodaj_dni2(Z[Z[n].zp2].rmdk, 1);
		}
	}
	Z[n].rmdk = dodaj_dni2(Z[n].rmdp, Z[n].dni);
	Z[n].z = 1;
	if (Z[n].rmdk>db) db = Z[n].rmdk;
	return;
}

void generuj_harmonogram(zadanie Z[], int n) {
	for (int i = 0; i<n; i++) {
		if (Z[i].nr >= 0 && Z[i].nr < MX_I_ZAD) {
			zaplanuj_zadanie(Z, i);
		}
	}
	return;
}

void pisz_menu(int m)
{
	switch (m)
	{
	case 1: {
		cout << endl;
		cout << "MENU glowne" << endl;
		cout << "----------------------" << endl;
		cout << "1 - dane ogolne projektu " << endl;
		cout << "2 - wygenerowanie zadanej ilosci zadan " << endl;
		cout << "3 - dodawanie zadan do projektu " << endl;
		cout << "4 - wyswietlenie listy zadan " << endl;
		cout << "5 - zmiana parametrow zadania" << endl;
		cout << "6 - generowanie harmonogramu 1 zadania" << endl;
		cout << "7 - generowanie harmonogramu " << endl;
		cout << "0 - KONIEC pracy" << endl;
		cout << "> ";
		break;
	}
	case 2: {
		cout << endl;
		cout << "MENU dane ogolne projektu" << endl;
		cout << "----------------------" << endl;
		cout << "1 - wprowadz nazwe projektu " << endl;
		cout << "2 - wprowadz ilosc zadan projektu" << endl;
		cout << "0 - wyjscie";
		cout << "> ";
		break;
	}
	case 3: {
		cout << endl;
		cout << "MENU edycja zadania" << endl;
		cout << "----------------------" << endl;
		cout << "1 - zmiana numeru zadania " << endl;
		cout << "2 - zmiana nazwy zadania " << endl;
		cout << "3 - zmiana pracochlonnosci zadania " << endl;
		cout << "4 - zmiana numeru zadania poprzedzajacego nr 1 " << endl;
		cout << "5 - zmiana numeru zadania poprzedzajacego nr 2 " << endl;
		cout << "0 - wyjscie";
		cout << "> ";
		break;
	}
	case 10: {
		cout << endl;
		cout << "MENU glowne" << endl;
		cout << "----------------------" << endl;
		cout << "1 - dane ogolne projektu " << endl;
		cout << "0 - KONIEC pracy " << endl;
		break;

	}
	}
	
}

void menu_projekt(projekt P[], int n)
{
	int wybor;
	do {
		wypisz_projekt(P, 1);
		cout << endl;
		pisz_menu(2);
		wybor = 10;
		cin >> wybor;
		switch (wybor)
		{
		case 1: wpr_proj_nazwa(P, 1);
			break;
		case 2: wpr_proj_ilzad(P, 1);
			break;
		}
	} while (wybor != 0);
}

void menu_zadanie(zadanie Z[], int n)
{
	char nz[MX_C_NAZ_ZAD + 1];
	int wybor;
	do {
		cout << endl;
		wypisz_nagz();
		wypisz_zadanie(&Z[n]);
		cout << endl;
		pisz_menu(3);
		cin >> wybor;
		switch (wybor)
		{
		case 1: ;
			break;
		case 2: cout << endl << "Nazwa zadania     : ";
			cin >> nz;
			strcpy(Z[n].nazwa, nz);
			break; 
		case 3: 
			cout << "Pracochlonnosc zadania [dni]  : ";
			cin >> Z[n].dni;
			break;
		case 4: 
			cout << "Nr zadania poprzedzajacego nr 1: ";
			cin >> Z[n].zp1;
			break;
		case 5: 
			cout << "Nr zadania poprzedzajacego nr 2: ";
			cin >> Z[n].zp2;
			break;
		}
	} while (wybor != 0);
}

void menu_main(zadanie Z[], int n, projekt *P)
{
	int wybor;
	int zwybor, mwybor;

	do {
		switch (db)
		{
		case 0: pisz_menu(10);
			break;
		default: pisz_menu(1);
			break;
		}
		cin >> wybor;
		switch (wybor)
		{
		case 1: menu_projekt(P, 1);
			break;
		case 2: if (db > 0) generuj_zadania(Z, MX_I_ZAD, P);
			break;
		case 3: if (db > 0) wpr_zadanie(Z, MX_I_ZAD);
			break;
		case 4: if (db > 0) wypisz_zadania(Z, MX_I_ZAD);
			break;
		case 5: if (db > 0){
			cout << endl << "Numer zadania: ";
			cin >> zwybor;
			cout << endl << zwybor;
			menu_zadanie(Z, zwybor);
			break;
			}
		case 6:
			if (db > 0) {
			cin >> mwybor;
			zaplanuj_zadanie(Z, mwybor);
			}
			break;
		case 7: 
			zeruj_z(Z, MX_I_ZAD);
			generuj_harmonogram(Z, MX_I_ZAD);
			break;
		}
	} while (wybor != 0);
}

int main()
{
	projekt P;
	zadanie *Z;

	strcpy(P.nazwa, "");
	P.czasp = P.czask = DATA_PP;
	P.ilzad = 0;

	Z = new zadanie[MX_I_ZAD];
	
	menu_main(Z, MX_I_ZAD, &P);

	return 0;
}
