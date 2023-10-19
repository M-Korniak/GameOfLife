/**
 * "Zycie"
 *
 * Program wykonuje symulacje świata komórek zgodnie z zasadami "Game of Life".
 *
 * Symulacja zaczyna się od pewnej generacji początkowej.
 * W następnej generacji komórka będzie żywa wtedy i tylko wtedy, gdy:
 * -w bieżącej generacji jest żywa i ma dokładnie dwóch lub trzech żywych sąsiadów,
 * -w bieżącej generacji jest martwa i ma dokładnie trzech żywych sąsiadów.
 *
 * Program wczytuje opis generacji początkowej.
 * W pętli, pokazuje fragment planszy, po czym czyta i wykonuje polecenie użytkownika.
 * Polecenia sterują liczeniem kolejnych generacji.
 * Określają też fragment planszy, nazywany oknem.
 *
 * Program rozpoznaje polecenia:
 * '.' - zakończenie pracy
 * N - liczba całkowita dodatnia - oblicza N - tą generacje
 * wiersz pusty - oblicza następną genrację
 * '0' - generuje opis aktualnej generacji.
 * 'w' 'k' - zmienia współrzędne lewego górnego rogu okna na wiersz w i kolumne k
 *
 * autor: Michał Korniak
 *
 * data: 30 grudnia 2022
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/**
 * Liczba wierszy okna
 */
#ifndef WIERSZE
#define WIERSZE 22
#endif

/**
 * Liczba kolumn okna
 */
#ifndef KOLUMNY
#define KOLUMNY 80
#endif

/**
 * Reprezentuje komórke
 *
 * 'stan' - aktualny stan komórki
 * 'ile_sasiadow'- liczba sasiadow komorki
 * 'kol' - numer kolumny w której jest komórka
 * 'poprz' - wskaźnik do poprzedniej komórki w wierszu
 * 'nast' - wskaźnik do następnej komórki w wierszu
 */
typedef struct Komorka_
{
    char stan;
    int ile_sasiadow;
    int kol;
    struct Komorka_* poprz;
    struct Komorka_* nast;
} Komorka;

/**
 * Reprezentuja wiersz planszy
 *
 * 'wiersz' - numer wiersza
 * 'k'- wskaźnik do pierwszej komórki w wierszu
 * 'poprz' - wskaznik do poprzedniego wiersza
 * 'nast' - wskaźnik do następnego wiersza
 */
typedef struct Plansza_
{
    int wiersz;
    Komorka* k;
    struct Plansza_* poprz;
    struct Plansza_* nast;
} Plansza;

/**
 * Reprezentuje komorke okna
 *
 * 'znak' - '0' lub '.', komorka opowiednio żywa lub martwa.
 * 'wiersz' - numer wiersza
 * 'kol' - numer kolumny
 */
typedef struct Okno_
{
    char znak;
    int wiersz;
    int kol;
} Okno;

/**
 * Usuwa ostatnią komórke 'k'  w wierszu
 *
 * Gdyby po usunięcia wiersz miałby być pusty wtedy nie usuwa
 */
void usun_komorke(Komorka* k) {
    if (k -> poprz != NULL)
        k -> poprz -> nast = NULL;
    free(k);
}

/**
 * Usuwa ostatni wiersz 'p'  w planszy
 *
 * Gdyby po usunięciu plansza miałaby być pusta wtedy zmienia stan ostatniej komórki
 */
void usun_wiersz(Plansza* p) {
    if (p -> poprz != NULL) {
        p -> poprz -> nast = NULL;
        free(p -> k);
        free(p);
    }
    else
        p -> k -> stan = 0;
}

/**
 * Tworzy i dodaje komórke na koniec wiersza bezpośrednio za 'k'
 *
 * Zwraca w wyniku adres nowej komórki
 *
 * k jest różne od NULL
 */
Komorka* dodaj_komorke(Komorka* k)
{
    assert(k != NULL);
    Komorka* k2 = (Komorka*) malloc (sizeof(Komorka));
    k -> nast = k2;
    k2 -> poprz = k;
    k2 -> nast = NULL;
    return k2;
}

/**
 * Tworzy i dodaje wiersz na koniec planszy bezpośrednio za 'p'
 *
 * Zwraca w wyniku adres nowego wiersza
 *
 * p jest różne od NULL
 */
Plansza* dodaj_wiersz(Plansza* p)
{
    assert(p != NULL);
    Plansza* p2 = (Plansza*) malloc (sizeof(Plansza));
    p -> nast = p2;
    p2 -> poprz = p;
    p2 -> nast = NULL;
    return p2;
}

/**
 * Czyta, zapisuje i inicjuje komórki dane na wejściu w wierszu, w którym jest 'pom2'
 *
 * pom2 jest różne od NULL
 */
void czytaj_wiersz(Komorka* pom2)
{
    assert(pom2 != NULL);
    int x;
    int y;
    int ok2 = 1;
    while (ok2) {
        x = getchar();
        if (x == EOF || x == '\n')
            ok2 = 0;
        else {
            scanf("%d", &y);
            pom2 -> kol = y;
            pom2 -> stan = '0';
            pom2 -> ile_sasiadow = 0;
            pom2 = dodaj_komorke(pom2);
        }
    }
    usun_komorke(pom2);
}

/**
 * Czyta, zapisuje i inicjuje komórki dane na wejściu w całej planszy 'p'
 *
 * p jest różne od NULL
 */
void czytaj_plansze(Plansza* p)
{
    assert(p != NULL);
    int x;
    int y;
    int ok = 1;
    Plansza* pom = p;
    Komorka* pom2 = p -> k;
    while (ok) {
        x = getchar();
        x = getchar();
        if (x == EOF || x == '\n')
            ok = 0;
        else {
            ungetc(x, stdin);
            scanf("%d", &y);
            pom -> wiersz = y;
            czytaj_wiersz(pom2);
            pom = dodaj_wiersz(pom);
            pom -> k = (Komorka*) malloc (sizeof(Komorka));
            pom2 = pom -> k;
            pom2 -> poprz = NULL;
            pom2 -> nast = NULL;
        }
    }
    usun_wiersz(pom);
}

/**
 * Sprawdza czy plansza jest pusta
 * Przyjmujemy, że gdy stan ostaniej komórki to 0 w ASCII to plansza jest pusta
 *
 * 'p' jest różne od NULL
 */
int czy_plansza_pusta(Plansza* p)
{
    assert(p != NULL);
    if ((int) p -> k -> stan == 0)
        return 1;
    else
        return 0;
}

/**
 * Tworzy wiersz oraz komórke w nim takie, że
 * -numer wiersza jest równy numerowi wiersza 'pom' + 'w' oraz
 * -numer kolumny komórki jest równy numerowi komórki 'pom2' + 'n'
 *
 * 'pom' i 'pom2' są różne od NULL, w = {-1, 1},  'n' = {-1, 0, 1}
 */
void tworz_wiersz(Plansza* pom, Komorka* pom2, int w, int n)
{
    assert(pom != NULL);
    assert(pom2 != NULL);
    assert(w == -1 || w == 1);
    assert(n >= -1 && n <= 1);
    Plansza* pomw = (Plansza*) malloc (sizeof(Plansza));
    if (w == -1) {
        if (pom -> poprz != NULL)
            pom -> poprz -> nast = pomw;
        pomw -> poprz = pom -> poprz;
        pomw -> nast = pom;
        pom -> poprz = pomw;
    }
    else {
        if (pom -> nast != NULL)
            pom -> nast -> poprz = pomw;
        pomw -> nast = pom -> nast;
        pomw -> poprz = pom;
        pom -> nast = pomw;
    }
    pomw -> wiersz = pom -> wiersz + w;
    pomw -> k = (Komorka*) malloc (sizeof(Komorka));
    pomw -> k -> stan = '.';
    pomw -> k -> ile_sasiadow = 0;
    pomw -> k -> kol = pom2 -> kol + n;
    pomw -> k -> poprz = NULL;
    pomw -> k -> nast = NULL;
}

/**
 * Tworzy komórke w wierszu 'pomw' przed komórką 'pom3', taką że
 * numer jej wiersza jest równy numerowi wiersza 'pom2' + 'n'
 *
 * 'pomw' jest rozne od NULL, 'n' = {-1, 0, 1}, 'pom3', 'pom2' są rozne od NULL
 */
void tworz_komorke(Plansza* pomw, Komorka* pom3, Komorka* pom2, int n)
{
    assert(pomw != NULL);
    assert(n >= -1 && n <= 1);
    assert(pom3 != NULL);
    assert(pom2 != NULL);
    Komorka* k2 = (Komorka*) malloc (sizeof(Komorka));
    if (pom3 -> poprz != NULL)
        pom3 -> poprz -> nast = k2;
    k2 -> poprz = pom3 -> poprz;
    k2 -> nast = pom3;
    pom3 -> poprz = k2;
    k2 -> stan = '.';
    k2 -> ile_sasiadow = 0;
    k2 -> kol = pom2 -> kol + n;
    if (k2 -> poprz == NULL) {
        pomw -> k = k2;
    }
}

/**
 * Tworzy wszystkich sąsiadów każdej żywej komórki w wierszu,
 * w którym jest 'pom2' począwszy od 'pom2' takich, że:
 * -znajdują się w wierszu 'pomw' oraz
 * -numer kolumny sąsiada różni się od numeru komórki o 'n'
 *
 * 'pomw' jest różne od NULL, n = {-1, 0 , 1}
 */
void tworz_sasiadow_pom_pom(Plansza* pomw, Komorka* pom2, int n)
{
    assert(pomw != NULL);
    assert(n >= -1 && n <= 1);
    Komorka* pom3 = pomw -> k;
    Komorka* pom4 = NULL;
    while (pom2 != NULL) {
        if (pom2 -> stan == '0') {
            if (pom3 == NULL) {
                pom3 = dodaj_komorke(pom4);
                pom3 -> stan = '.';
                pom3 -> ile_sasiadow = 0;
                pom3 -> kol = pom2 -> kol + n;
            }
            else if (pom2 -> kol + n < pom3 -> kol) {
                tworz_komorke(pomw, pom3, pom2, n);
                pom2 = pom2 -> nast;
            }
            else if (pom2 -> kol + n == pom3 -> kol) {
                pom2 = pom2 -> nast;
                pom4 = pom3;
                pom3 = pom3 -> nast;
            }
            else if (pom2 -> kol + n > pom3 -> kol) {
                pom4 = pom3;
                pom3 = pom3 -> nast;
            }
        }
        else
            pom2 = pom2 -> nast;
    }
}

/**
 * Tworzy wszystkich sąsiadów każdej żywej komórki w wierszu 'pom' takich, że:
 * -znajdują się w wierszu o numerze 'pom' -> kol  +  'w' oraz
 * -numer kolumny sąsiada różni się od numeru komórki o 'n'
 *
 * pom jest różne od NULL, n = {-1, 0 , 1}, w = {-1, 1}
 */
void tworz_sasiadow_pom(Plansza* pom, int w, int n)
{
    assert(pom != NULL);
    assert(w == -1 || w == 1);
    assert(n >= -1 && n <= 1);
    Komorka* pom2 = pom -> k;
    Plansza* pomw;
    if (w == -1)
        pomw = pom -> poprz;
    else
        pomw = pom -> nast;
    int ok = 1;
    while (pom2 != NULL && ok) {
        if (pom2 -> stan == '0' && (pomw == NULL || pom -> wiersz + w != pomw -> wiersz)) {
            tworz_wiersz(pom, pom2, w, n);
            ok = 0;
        }
        else if (pom2 -> stan == '0') {
            ok = 0;
            break;
        }
        pom2 = pom2 -> nast;
    }
    if (w == -1)
        pomw = pom -> poprz;
    else
        pomw = pom -> nast;
    if (ok == 0) {
        tworz_sasiadow_pom_pom(pomw, pom2, n);
    }
}

/**
 * Tworzy wszystkich sąsiadów każdej żywej komórki w wierszu 'pom' takich, że:
 * -znajdują się w wierszu 'pom' oraz
 * -numer kolumny sąsiada różni się od numeru komórki o 'n'
 *
 * pom jest różne od NULL, n = -1 lub n = 1
 */
void tworz_sasiadow_pom2(Plansza* pom, int n)
{
    assert(pom != NULL);
    assert(n == -1 || n == 1);
    Komorka* pom2 = pom -> k;
    while (pom2 != NULL) {
        if (pom2 -> stan == '0') {
            if (n == -1) {
                if (pom2 -> poprz == NULL || pom2 -> kol - 1 != pom2 -> poprz -> kol) {
                    tworz_komorke(pom, pom2, pom2, -1);
                }
            }
            else {
                if (pom2 -> nast == NULL || pom2 -> kol + 1 != pom2 -> nast -> kol) {
                    Komorka* k2 = (Komorka*) malloc (sizeof(Komorka));
                    if (pom2 -> nast != NULL)
                        pom2 -> nast -> poprz = k2;
                    k2 -> nast = pom2 -> nast;
                    k2 -> poprz = pom2;
                    pom2 -> nast = k2;
                    k2 -> stan = '.';
                    k2 -> ile_sasiadow = 0;
                    k2 -> kol = pom2 -> kol + 1;
                }
            }
        }
        pom2 = pom2 -> nast;
    }
}

/**
 * Tworzy wszystkich sąsiadów każdej żywej komórki w wierszu 'pom'
 *
 * pom jest różne od NULL
 */
void tworz_sasiadow_wiersz(Plansza* pom)
{
    assert(pom != NULL);
    tworz_sasiadow_pom(pom, -1, -1);
    tworz_sasiadow_pom(pom, -1, 0);
    tworz_sasiadow_pom(pom, -1, 1);
    tworz_sasiadow_pom2(pom, -1);
    tworz_sasiadow_pom2(pom, 1);
    tworz_sasiadow_pom(pom, 1, -1);
    tworz_sasiadow_pom(pom, 1, 0);
    tworz_sasiadow_pom(pom, 1, 1);
}

/**
 * Tworzy wszystkich sąsiadów każdej żywej komórki na planszy 'p'
 *
 * 'p' może ulec zmianie
 */
void tworz_sasiadow(Plansza** p)
{
    Plansza* pom = *p;
    while (pom != NULL) {
        tworz_sasiadow_wiersz(pom);
        pom = pom -> nast;
    }
    if ((*p) != NULL && (*p) -> poprz != NULL)
        (*p) = (*p) -> poprz;
}

/**
 * Inicjuje okno 'o' poprzez wpisane w wartości 'znak' całego okna znaku '.'
 *
 * 'w' i 'k' oznaczają współrzędne lewego górnego rogu okna
 *
 * 'o' ma wymiary [WIERSZE][KOLUMNY]
 */
void inicjuj_okno(Okno o[][KOLUMNY], int w, int k)
{
    for (int i = 0; i < WIERSZE; i++)
        for (int j = 0; j < KOLUMNY; j++) {
            o[i][j].znak = '.';
            o[i][j].wiersz = i + w;
            o[i][j].kol = j + k;
        }
}

/**
 * Wpisuje w okno 'o' komórki planszy 'p'
 *
 * Komórki w oknie są żywe wtw gdy są żywe na planszy 'p'
 *
 * 'w' i 'k' oznaczają współrzędne lewego górnego rogu okna
 *
 * 'o' ma wymiary [WIERSZE][KOLUMNY]
 */
void wpisz_w_okno(Plansza* p, Okno o[][KOLUMNY], int w, int k)
{
    Plansza* pom = p;
    while (pom != NULL) {
        Komorka* pom2 = pom -> k;
        while (pom2 != NULL) {
            if (w <= pom -> wiersz && pom -> wiersz < w + WIERSZE &&
                k <= pom2 -> kol && pom2 -> kol < k + KOLUMNY && pom2 -> stan == '0') {
                o[pom -> wiersz - w][pom2 -> kol - k].znak = '0';
            }
            pom2 = pom2 -> nast;
        }
        pom = pom -> nast;
    }
}

/**
 * Pisze wartości 'znak' okna 'o'
 *
 * 'o' ma wymiary [WIERZE][KOLUMNY]
 */
void pisz_okno(Okno o[][KOLUMNY])
{
    for (int i = 0; i < WIERSZE; i++) {
        for (int j = 0; j < KOLUMNY; j++)
            printf("%c", o[i][j].znak);
        printf("\n");
    }
}

/**
 * Wyświetla dolną ramkę okna dlugości KOLUMNY
 */
void pisz_ramke(void)
{
    for (int i = 0; i < KOLUMNY; i++)
        printf("=");
    printf("\n");
}

/**
 * Wyświetla okno planszy na której rozgrywa się gra
 *
 * Komórki w oknie są żywe wtw gdy są żywe na planszy 'p'
 *
 * 'w' i 'k' oznaczają współrzędne lewego górnego rogu okna
 */
void wyswietl_okno(Plansza* p, int w, int k)
{
    Okno o[WIERSZE][KOLUMNY];
    inicjuj_okno(o, w, k);
    wpisz_w_okno(p, o, w, k);
    pisz_okno(o);
    pisz_ramke();
}

/**
 * Zlicza liczbę sąsiadów każdej komórki w wierszu 'pom', takich że:
 * -znajdują się także w wierszu 'pom' -> kol + w oraz
 * -numer kolumny sąsiada różni się od numeru komórki o 'n'
 *
 * n = {-1, 0, 1} oraz w = {-1, 1}
 */
void zlicz_sasiadow_pom(Plansza* pom, int w, int n)
{
    assert(n >= -1 && n <= 1);
    assert(w == -1 || w == 1);
    int ok = 0;
    Komorka* pom2 = pom -> k;
    Komorka* pom3 = NULL;
    if (w == -1 && pom -> poprz != NULL && pom -> poprz -> wiersz + 1 == pom -> wiersz) {
        ok = 1;
        pom3 = pom -> poprz -> k;
    }
    else if (w == 1 && pom -> nast != NULL && pom -> nast -> wiersz - 1 == pom -> wiersz) {
        ok = 1;
        pom3 = pom -> nast -> k;
    }
    if (ok == 1) {
        while (pom2 != NULL && pom3 != NULL) {
            if (pom2 -> kol + n == pom3 -> kol) {
                if (pom3 -> stan == '0')
                    pom2 -> ile_sasiadow++;
                pom3 = pom3 -> nast;
                pom2 = pom2 -> nast;
            }
            else if (pom2 -> kol + n < pom3 -> kol){
                pom2 = pom2 -> nast;
            }
            else {
                pom3 = pom3 -> nast;
            }
        }
    }
}

/**
 * Zlicza liczbę sąsiadów każdej komórki w wierszu 'pom', takich że:
 * -znajdują się także w wierszu 'pom' oraz
 * -numer kolumny sąsiada różni się od numeru komórki o 'n'
 *
 * n = -1 lub n = 1
 */
void zlicz_sasiadow_pom2(Plansza* pom, int n)
{
    assert(n == -1 || n == 1);
    Komorka* pom2 = pom -> k;
    if (n == -1) {
        while (pom2 != NULL) {
            if (pom2 -> poprz != NULL && pom2 -> poprz -> kol == pom2 -> kol - 1 &&
                pom2 -> poprz -> stan == '0')
                pom2 -> ile_sasiadow++;
            pom2 = pom2 -> nast;
        }
    }
    else {
        while (pom2 != NULL) {
            if (pom2 -> nast != NULL && pom2 -> nast -> kol == pom2 -> kol + 1 &&
                pom2 -> nast -> stan == '0')
                pom2 -> ile_sasiadow++;
            pom2 = pom2 -> nast;
        }
    }
}

/**
 * Zlicza liczbę sąsiadów każdej komórki w wierszu 'pom'
 *
 * Wyniki zapisuje w ile_sasiadow każdej komórki
 */
void zlicz_sasiadow_wiersz(Plansza* pom)
{
    zlicz_sasiadow_pom(pom, -1, -1);
    zlicz_sasiadow_pom(pom, -1, 0);
    zlicz_sasiadow_pom(pom, -1, 1);
    zlicz_sasiadow_pom2(pom, -1);
    zlicz_sasiadow_pom2(pom, 1);
    zlicz_sasiadow_pom(pom, 1, -1);
    zlicz_sasiadow_pom(pom, 1, 0);
    zlicz_sasiadow_pom(pom, 1, 1);
}

/**
 * Zlicza liczbę sąsiadów każdej komórki na planszy 'p'
 *
 * Wyniki zapisuje w ile_sasiadow każdej komórki
 *
 * ile_sasiadow każdej komórki musi być równe 0
 */
void zlicz_sasiadow(Plansza* p)
{
    Plansza* pom = p;
    while (pom != NULL) {
        Komorka* pom2 = pom -> k;
        while (pom2 != NULL) {
            assert(pom2 -> ile_sasiadow == 0);
            pom2 = pom2 -> nast;
        }
        pom = pom -> nast;
    }
    pom = p;
    while (pom != NULL) {
        zlicz_sasiadow_wiersz(pom);
        pom = pom -> nast;
    }
}

/**
 * Aktualizuje stan wszystkich komórek planszy 'p' oraz zeruje liczbę sąsiadów
 *
 * W następnej generacji komórka będzie żywa wtedy i tylko wtedy, gdy:
 * -w bieżącej generacji jest żywa i ma dokładnie dwóch lub trzech żywych sąsiadów,
 * -w bieżącej generacji jest martwa i ma dokładnie trzech żywych sąsiadów.
 */
void aktualizuj_stan(Plansza* p)
{
    Plansza* pom = p;
    while (pom != NULL) {
        Komorka* pom2 = pom -> k;
        while (pom2 != NULL) {
            if (pom2 -> stan == '0' && (pom2 -> ile_sasiadow == 2 || pom2 -> ile_sasiadow == 3))
                pom2 -> stan = '0';
            else if (pom2 -> stan == '.' && pom2 -> ile_sasiadow == 3)
                pom2 -> stan = '0';
            else
                pom2 -> stan = '.';
            pom2 -> ile_sasiadow = 0;
            pom2 = pom2 -> nast;
        }
        pom = pom -> nast;
    }
}

/**
 * Usuwa komórki na planszy 'p', gdy:
 * -są martwe oraz
 * -wszyscy sąsiedzi danej komórki sa martwi
 */
void usun_martwe_pom(Plansza* p)
{
    Plansza* pom = p;
    while (pom != NULL) {
        Komorka* pom2 = pom -> k;
        while (pom2 != NULL) {
            if (pom2 -> stan == '.' && pom2 -> ile_sasiadow == 0) {
                if (pom2 -> poprz == NULL && pom2 -> nast == NULL) {
                    pom -> k = NULL;
                    free(pom2);
                    pom2 = NULL;
                }
                else if (pom2 -> poprz == NULL) {
                    Komorka* pom3 = pom2 -> nast;
                    pom3 -> poprz = NULL;
                    free(pom2);
                    pom -> k = pom3;
                    pom2 = pom3;
                }
                else if (pom2 -> nast == NULL) {
                    pom2 -> poprz -> nast = NULL;
                    free(pom2);
                    pom2 = NULL;
                }
                else {
                    Komorka* pom3 = pom2 -> poprz;
                    pom3 -> nast = pom2 -> nast;
                    pom2 -> nast -> poprz = pom3;
                    free(pom2);
                    pom2 = pom3 -> nast;
                }
            }
            else {
                pom2 = pom2 -> nast;
            }
        }
        pom = pom -> nast;
    }
}

/**
 * Usuwa wiersze planszy 'p', w których nie ma komórek
 *
 * 'p' może ulec zmianie
 */
void usun_puste_wiersze(Plansza** p)
{
    Plansza* pom = (*p);
    while (pom != NULL) {
        if (pom -> k == NULL) {
            if (pom -> poprz == NULL && pom -> nast == NULL) {
                (*p) = NULL;
                free(pom);
                pom = NULL;
            }
            else if (pom -> poprz == NULL) {
                Plansza* pom3 = pom -> nast;
                pom3 -> poprz = NULL;
                free(pom);
                pom = pom3;
                *p = pom3;
            }
            else if (pom -> nast == NULL) {
                pom -> poprz -> nast = NULL;
                free(pom);
                pom = NULL;
            }
            else {
                Plansza* pom3 = pom -> poprz;
                pom3 -> nast = pom -> nast;
                pom -> nast -> poprz = pom3;
                free(pom);
                pom = pom3 -> nast;
            }
        }
        else
            pom = pom -> nast;
    }
}

/**
 * Usuwa komórki na planszy 'p', gdy:
 * -są martwe oraz
 * -wszyscy sąsiedzi danej komórki sa martwi
 *
 * Usuwa puste wiersze
 *
 * 'p' może ulec zmianie
 */
void usun_martwe(Plansza** p)
{
    usun_martwe_pom(*p);
    usun_puste_wiersze(p);
}

/**
 * Zeruje liczbe sąsiadów każdej komórki na planszy 'p'
 */
void zeruj_sasiadow(Plansza* p)
{
    Plansza* pom = p;
    while (pom != NULL) {
        Komorka* pom2 = pom -> k;
        while (pom2 != NULL) {
            pom2 -> ile_sasiadow = 0;
            pom2 = pom2 -> nast;
        }
        pom = pom -> nast;
    }
}

/**
 * Oblicza następną generacje komórek planszy 'p'
 *
 * Zapisuje następną generację na planszy 'p'
 *
 * 'p' może ulec zmianie
 */
void nastepna_generacja(Plansza** p)
{
    zlicz_sasiadow(*p);
    aktualizuj_stan(*p);
    zlicz_sasiadow(*p);
    usun_martwe(p);
    zeruj_sasiadow(*p);
    tworz_sasiadow(p);
}

/**
 * Pisze zrzut stanu planszy 'p'
 */
void zrzuc_stan(Plansza* p)
{
    Plansza* pom = p;
    while (pom != NULL) {
        Komorka* pom2 = pom -> k;
        while (pom2 != NULL && pom2 -> stan == '.') {
            pom2 = pom2 -> nast;
        }
        if (pom2 != NULL) {
            printf("/%d %d", pom -> wiersz, pom2 -> kol);
            pom2 = pom2 -> nast;
            while (pom2 != NULL) {
                if (pom2 -> stan == '0') {
                    printf(" %d", pom2 -> kol);
                }
                pom2 = pom2 -> nast;
            }
            printf("\n");
        }
        pom = pom -> nast;
    }
    printf("/\n");
}

/**
 * Usuwa wszystkie komórki i wiersze planszy 'p'
 */
void czysc_plansze(Plansza* p)
{
    Plansza* pom = p;
    while (pom != NULL) {
        Komorka* pom2 = pom -> k;
        while (pom2 != NULL) {
            Komorka* pom3 = pom2 -> nast;
            free(pom2);
            pom2 = pom3;
        }
        Plansza* pom4 = pom -> nast;
        free(pom);
        pom = pom4;
    }
}

/**
 * Rozgrywa grę na planszy 'p'.
 *
 * Czyta początkowy stan planszy oraz polecenia użytkownika
 *
 * 'p' jest różne od NULL
 */
void rozgrywaj(Plansza* p)
{
    assert(p != NULL);
    int w = 1;
    int k = 1;
    czytaj_plansze(p);
    if (czy_plansza_pusta(p)) {
        free(p -> k);
        free(p);
        p = NULL;
    }
    tworz_sasiadow(&p);
    int x;
    while (1) {
        wyswietl_okno(p, w, k);
        x = getchar();
        if ((char) x == '.') {
            czysc_plansze(p);
            break;
        }
        else if ((char) x == '\n' || (char) x == EOF)
            nastepna_generacja(&p);
        else {
            ungetc(x, stdin);
            int y;
            scanf("%d", &y);
            x = getchar();
            if ((char) x == ' ') {
                w = y;
                scanf("%d", &y);
                k = y;
                x = getchar();
            }
            else {
                if (y == 0) {
                    zrzuc_stan(p);
                }
                else {
                    for (int i = 0; i < y; i++)
                        nastepna_generacja(&p);
                }
            }
        }
    }
}

/**
 * Inicjuje plansze 'p' tworząć w niej komórke
 *
 * Ustawia wszystkie wskażniki na NULL
 */
void inicjuj_plansze(Plansza** p)
{
    
    (*p) -> poprz = NULL;
    (*p) -> nast = NULL;
    (*p) -> k = (Komorka*) malloc (sizeof(Komorka));
    (*p) -> k -> poprz = NULL;
    (*p) -> k -> nast = NULL;
}

/**
 * Uruchamia program i tworzy plansze
 */
int main(void) {
    Plansza* p = (Plansza*) malloc (sizeof(Plansza));
    inicjuj_plansze(&p);
    rozgrywaj(p);
    return 0;
}
