#include <iostream>
#include <string>
#include <fstream>
#include <limits>

using namespace std;

class Ksiazki {
    private:
        long kolejny_n;
        string sygnatura;
        string imie_autora;
        string nazwisko_autora;
        string tytul;
        string ISBN;
        string wydawnictwo;
        short rok_wydania;
    public:
        void setParam(long kolejny_n, string syg, string im_au, string naz_au, string tyt, string ISBN, string wyd, short rok);
        void dodajDane();
        void listowanie();
        void wyszukajDane();
        void usunDane();
        void wyswietlKsiazke();
        void pobierzDane();
        Ksiazki();

};

string pobierzStringa(string pole) {
    cout << "Podaj " << pole << ": " << endl;
    string s;
    cin >> s;
    return s;
}

void przerwa(){
    cout << "Nacisnij Enter, aby kontynuowac" << endl;
    getchar();
    cin.ignore(numeric_limits<streamsize>::max(),'\n'); //Ignorujemy wpisane przez uzytkownika wartosci jesli jakies wpisal w celu zniwelowania bledow
}

//Funkcja weryfikujaca czy podany ciag jest liczba (int)
int pobierzLiczbe(string pole) {
    int d;
    string z;
    bool isError;
    do {
        isError=false;
        cout << "Podaj " << pole << ": " << endl;
        try{
            getline(cin,z);
            d=stoi(z.c_str());
        } catch (invalid_argument const &e){
            cerr << "Nieprawidlowa wartosc! \n";
            isError=true;
        } catch (out_of_range const &e) {
            cerr << "Przekroczenie dopuszczalnej wartosci! \n";
            isError=true;
        }
    } while (isError);

    return d;
}

//Nadanie domyslnych wartosci ksiazce po utworzeniu obiektu
Ksiazki::Ksiazki(){
    this->kolejny_n = 0;
    this->sygnatura = "";
    this->imie_autora = "";
    this->nazwisko_autora = "";
    this->tytul = "";
    this->ISBN = "";
    this->wydawnictwo = "";
    this->rok_wydania = 2000;
}

void Ksiazki::pobierzDane(){
    this->kolejny_n = pobierzLiczbe("kolejny numer");
    this->sygnatura = pobierzStringa("sygnature");
    this->imie_autora = pobierzStringa("imie");
    this->nazwisko_autora = pobierzStringa("nazwisko");
    this->tytul = pobierzStringa("tytul");
    this->ISBN = pobierzStringa("ISBN");
    this->wydawnictwo = pobierzStringa("nazwe wydawnictwa");
    cin.ignore(numeric_limits<streamsize>::max(),'\n'); //Ignorujemy wpisane przez uzytkownika poprzednie wartosci w celu unikniecia bledow
    this->rok_wydania = pobierzLiczbe("rok wydania");
}

//Nadanie ksiazce parametrow czyli wpisanych przez uzytkownika tytulu,numeru itd.
void Ksiazki::setParam(long ko_numer, string syg, string im_au, string naz_au, string tyt, string ISBN, string wyd, short rok){
    this->kolejny_n = ko_numer;
    this->sygnatura = syg;
    this->imie_autora = im_au;
    this->nazwisko_autora = naz_au;
    this->tytul = tyt;
    this->ISBN = ISBN;
    this->wydawnictwo = wyd;
    this->rok_wydania = rok;
}

//Dodajemy nowa ksiazke do naszego pliku ksiazki.txt, wczesniej pobralismy od uzytkownika parametry
void Ksiazki::dodajDane(){
    string nazwa = "ksiazki.txt";
    fstream plik;
    plik.open(nazwa, ios::app);
    plik << this->kolejny_n << " " << this->sygnatura << " " << this->imie_autora <<
    " " << this->nazwisko_autora << " " << this->tytul << " " << this->ISBN << " " <<
    this->wydawnictwo << " " << this->rok_wydania << endl;
    plik.close();
}

//Funckja, ktora odczytuje z pliku i wyswietla wszystkie ksiazki w nim zawarte
void Ksiazki::listowanie(){
    string nazwa = "ksiazki.txt";
    fstream plik;
    string zdanie = "";
    plik.open(nazwa, ios::in);
    cout << "Zawartosc pliku: " << endl;
    do {
        getline(plik,zdanie);
        cout << zdanie << endl;
    } while(!plik.eof()); //Petla sie wykonuje, az do konca pliku
    plik.close();
}

//Funkcja umozliwiajaca wyszukanie ksiazki poprzez wpisanie frazy
void Ksiazki::wyszukajDane(){
    bool isFraza = false;
    string fraza = "";
    cout << "Prosze wpisac poszukiwana fraze:";
    getline(cin,fraza);
    fstream plik;
    string zdanie;
    plik.open("ksiazki.txt");
    size_t pos;
    while(plik.good()){
        getline(plik,zdanie);
        pos=zdanie.find(fraza); //Sprawdzamy czy w danym wierszu znajduje sie podana fraza
        if(pos!=string::npos) { //Jesli tak to przypisujemy dany wiersz do frazy i konczymy szukanie
            fraza = zdanie;
            isFraza = true;
            break;
        }
    }
    if(isFraza) {
        cout << "Znaleziono ksiazke z wyszukana fraza!" << endl;
        cout << fraza << endl; //Wyswietlamy nasz znaleziony wiersz
    } else {
        cout << "Nieznaleziono szukanej frazy!" << endl; //Wyswietlamy komunikat jesli nie znajdziemy wiersza z dana fraza
    }

    plik.close();
}

/* Funkcja usuwajaca ksiazke po znalezieniu wiersza z dana fraza
Dziala w takie sposob, ze wszystkie wiersze niezawierajace danej frazy kopiuje do pliku tmp.txt
A na stepnie usuwa plik ksiazki.txt i zamienia nazwe temp.txt na ksiazki.txt
Dzieki temu powstaje nam nowy plik z ksiazkami bez ksiazek z podana wczesniej fraza */
void Ksiazki::usunDane(){
    bool isFraza = false;
    string fraza = "";
    cout << "Prosze wpisac poszukiwana fraze:";
    getline(cin,fraza);
    fstream plik;
    ofstream temp;
    string zdanie;
    plik.open("ksiazki.txt");
    temp.open("temp.txt");
    size_t pos;
    while(getline(plik,zdanie)){
        pos=zdanie.find(fraza);
        if(pos==string::npos) {
            temp << zdanie << endl;
        }
    }
    cout << "Usunieto rekord/y z podana fraza (jesli nie znaleziono frazy to rekordy pozostaly bez zmian)" << endl;
    plik.close();
    temp.close();
    remove("ksiazki.txt");
    rename("temp.txt","ksiazki.txt");
}

int main(){
    Ksiazki ksiazka;
    while(true){
        int numer = 0;
        string opcja;
        bool inBlad;
        cout << "Wybierz opcje(1,2,3,4,5):" << endl;
        cout << "1.Dodawanie ksiazki" << endl;
        cout << "2.Listowanie ksiazek" << endl;
        cout << "3.Wyszukanie ksiazki" << endl;
        cout << "4.Usuwanie ksiazki" << endl;
        cout << "5.Zakoncz" << endl;

        //Sprawdzamy czy podany ciag jest liczba
        do {
            inBlad = false;
            cout << "Podaj numer opcji: ";
            getline(cin,opcja);
            for(char c: opcja){
                if(c >= '0' && c <= '9') numer = numer * 10 + (c - '0');
                else {
                    inBlad = true;
                    cerr << "Nieprawidlowa wartosc! Podaj wartosc z zakresu 1-5.\n";
                    numer = 0;
                    break;
                }
            }
        } while(inBlad || opcja=="");
        //Po pobraniu od uzytkownika opcji sprawdzamy czy istnieje i jesli tak to wykonujemy funkcje w niej zawarte
        switch(numer){
            case 1:
                cout << "Opcja nr 1\n";
                ksiazka.pobierzDane();
                ksiazka.dodajDane();
                przerwa();
                break;
            case 2:
                cout << "Opcja nr 2\n";
                ksiazka.listowanie();
                przerwa();
                break;
            case 3:
                cout << "Opcja nr 3\n";
                ksiazka.wyszukajDane();
                przerwa();
                break;
            case 4:
                cout << "Opcja nr 4\n";
                ksiazka.usunDane();
                przerwa();
                break;
            case 5:
                cout << "Zakonczono!";
                return 0;
            default:
                cout << "Nieprawidlowa wartosc! Podaj wartosc z zakresu 1-5.\n";
                przerwa();
                break;
        }

    }

    getchar();
    EXIT_SUCCESS;
}