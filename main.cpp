#include <iostream>
#include <cstring>
#include <vector>
#include <memory>

using namespace std;

const int trenutnaGodina(2019);

class Racun {
    char ime[20], prezime[20];
    unsigned int indeks;
    double stanjeRacuna;
    void IspravnostDizanja (double iznos);
public:
    Racun (const char ime[], const char prezime[], int indeks, int iznos);
    double dajStanje () const { return stanjeRacuna;};
    const char *DajIme () const { return ime;}
    const char *DajPrezime () const { return prezime;}
    Racun &Uplata (double iznos);
    Racun &Isplata (double iznos);
    int dajIndeks () const { return indeks; }
    void IspisiStanje () const;
};


class Datum {
    int dan, mjesec, godina;
public:
    Datum (int d, int m, int g);
    double dajGodinu () const { return godina;};
    void Ispisi() const { cout << dan << "/" << mjesec << "/" <<godina;};
};

class Let {
    char polazak[20], odrediste[20], ime[20], prezime[20];
    Datum dRodjenja;
    int mjesto;
    double cijenaLeta;
public:
    Let (const char odakle[], const char gdje[], const char ime[], const char prezime[], int d, int m, int g, int brojSjedista);
    const char *DajIme () const { return ime;}
    const char *DajPrezime () const { return prezime;}
    Let &PostaviCijenu (double iznos) { cijenaLeta = iznos; return *this;};
    int dajMjesto () { return mjesto;};
    double dajCijenu () const { return cijenaLeta;};
    int dajGodine () const { return (trenutnaGodina - dRodjenja.dajGodinu());}
    void Ispisnica () const;
};

class Avion {
    vector<shared_ptr<Let> > putnici;
    vector<shared_ptr<Racun> > racuni;
    friend void Dobrodoslica();
public:
    Avion () : putnici(0), racuni(0) {};
    Avion (const Avion &a);
    Avion (Avion &&a) = default;
    Avion &operator=(const Avion &a);
    Avion &operator=(Avion &&a) = default;
    Avion &PrijavaRacuna(int indeks);
    Avion &ZakaziLet ();
    Avion &OtkaziLet ();
    Avion &OdbijLet (shared_ptr<Let> osoba);
    Avion &OtvoriRacun();
    Avion &UplatinaRacun (int indeks);
    void PlatiCijenu (int indeks, shared_ptr<Let> osoba);
    void VratiPare (int indeks, shared_ptr<Let> osoba);
    void InformacijeRacuna (int indeks);
    void ObracunajCijenu (shared_ptr<Let> osoba);
    void SlobodnaMjesta ();
    double BrojMjesta() const { return putnici.size(); };
    void ZauzetaMjesta ();
    void IspisiPutnike ();
    void DajImeKorisnika(int indeks) const { cout << racuni[indeks-1]->DajIme();}
    void DajPrezimeKorisnika (int indeks) const { cout << racuni[indeks-1]->DajPrezime();}
};

void Racun::IspravnostDizanja(double iznos) {
    if (iznos < 0)
        throw range_error ("Iznos mora biti veci od 0!");
    if (iznos > stanjeRacuna)
        throw range_error ("Iznos mora biti manji od stanja racuna!");
return;
}

void Racun::IspisiStanje() const {
    cout << prezime << " " << ime << " vase stanje iznosi: " << stanjeRacuna;
}

Racun::Racun(const char ime[], const char prezime[], int transakcioniBroj, int iznos) : stanjeRacuna(0), indeks(transakcioniBroj) {
    if (iznos < 0)
        throw range_error ("Iznos mora biti veci od 0!");
    stanjeRacuna = iznos;
    strcpy(this->ime, ime);
    strcpy(this->prezime, prezime);
}

Racun& Racun::Isplata(double iznos) {
    IspravnostDizanja(iznos);
    cout << "\nSa vaseg racuna uspjesno je skinuto: " << iznos;
    stanjeRacuna-=iznos;

return *this;
}

Racun& Racun::Uplata(double iznos) {
    if (iznos < 0)
        throw range_error ("Uplata mora biti veca od 0 KM");
stanjeRacuna+=iznos;
    cout << "Uspjesno vam je uplaceno: " << iznos;
return *this;
}

Datum::Datum(int d, int m, int g) {
    int brojDana[12]{31,28,31,30,31,30,31,31,30,31,30,31};
    if (g % 4 == 0 && g % 100 != 0 || g % 400 == 0)
        brojDana[1]++;
    if (g < 1 || d < 1 || m < 1 || m > 12 || d > brojDana[m-1] || g > trenutnaGodina)
        throw::range_error("Neispravan datum!");
    dan = d; mjesec = m; godina = g;
}

Let::Let(const char odakle[], const char gdje[], const char ime[], const char prezime[], int d, int m, int g, int brojSjedista) : dRodjenja(d,m,g), mjesto(brojSjedista) {
    if (strlen(odakle) > 20 || strlen(gdje) > 20 || strlen(ime) > 20 || strlen(prezime) > 20)
        throw range_error ("Svi podaci moraju biti ispod 20 znakova!");
    strcpy(polazak, odakle);
    strcpy(odrediste, gdje);
    strcpy(this->ime, ime);
    strcpy(this->prezime, prezime);
}

void Let::Ispisnica() const {
    cout << "\nGospodin: " <<prezime << " " <<ime << " rodjen "; dRodjenja.Ispisi();
    cout << " zakazao je let sa " <<polazak << " na lokaciju " << odrediste << "!";
    cout <<endl << "Mjesto u avionu je broj: " << mjesto;
 }

bool LegitimnaDuzina (const char rec[]) {
return (strlen(rec) < 20);
}

void Dobrodoslica () {
    cout << "Dobrodosli na BiH airlanes!" <<endl;
    cout << "U nastavku molimo vas da popunite sljedece informacije(informacije ne smiju biti duze od 20 slova)!" <<endl;
    cout << "Hvala i sretan put!" <<endl;
}

Avion::Avion(const Avion &a) : putnici(a.putnici.size()) {
    for (int i=0; i<putnici.size(); i++)
        putnici[i] = make_shared<Let>(*a.putnici[i]);
}

void Avion::InformacijeRacuna(int indeks) {
    if (indeks-1 >= racuni.size())
        throw range_error ("Racun ne postoji!");
    racuni[indeks-1]->IspisiStanje();
}

Avion& Avion::OtvoriRacun() {
    system("cls");
    char ime[20], prezime[20];
    unsigned long int brojRacuna;
    int uplata;
        cout << "Unesite ime: ";
         cin >> ime;
        cout << "Unesite prezime: ";
         cin >> prezime;
        cout << "Pocetna uplata na racun: ";
         cin >> uplata;
        for (int i=0; i<racuni.size(); i++) {
            if (strcmp(racuni[i]->DajIme(), ime) == 0 && strcmp(racuni[i]->DajPrezime(), prezime) == 0)
                throw range_error  ("Racun vec postoji!");
        }
racuni.push_back(make_shared<Racun>(ime, prezime, racuni.size(), uplata));
    cout << "Uspjesno ste otvorili racun!";
    cout << "\nIndeks vaseg racuna je: " << racuni.size();
return *this;
}

Avion& Avion::PrijavaRacuna(int indeks) {
    if (indeks-1 >= racuni.size())
        throw range_error ("Racun nije pronadjen!");
    cout << "Uspjesno ste se prijavili " << racuni[indeks-1]->DajIme() << " " << racuni[indeks-1]->DajPrezime();
}

Avion& Avion::ZakaziLet() {
    bool pogresan(true);
    char ime[20], prezime[20], mjestoBoravka[20], destinacija[20];
        int zeljenoMjesto, dan, mjesec, godina;
    while (pogresan) {
        try {
            system("cls");
        Dobrodoslica();
        cout << "\nUslov za rezervaciju leta jeste kreiran nas racun, koji mozete napraviti na ovoj aplikaciji!\n";
        cout <<endl;
        cout << "Unesite vase ime: ";
         cin >> ime;
        cout << "Unesite vase prezime: ";
         cin >> prezime;
         if (strlen(ime) < 3 || strlen(prezime) < 3)
            throw range_error ("Ime i prezime mora biti duze od 3 slova!");
        cout << "Unesite vas datum rodjenja: ";
         cin >> dan >> mjesec >> godina;
         if (!cin)
            throw range_error ("Molimo vas, unesite validan datum!");
        cout << "Unesite polaziste: ";
         cin >> mjestoBoravka;
        cout << "Unesite destinaciju koju zelite posjetiti: ";
         cin >> destinacija;
         do {
        cout << "Izbor mjesta u avionu: ";
         cin >> zeljenoMjesto;
            }while(zeljenoMjesto < 0 || zeljenoMjesto > 30);
        if (!LegitimnaDuzina(ime) || !LegitimnaDuzina(prezime) || !LegitimnaDuzina(mjestoBoravka) || !LegitimnaDuzina(destinacija))
            throw range_error ("Podaci nisu validni!");
        for (int i=0; i<putnici.size(); i++)
            if (zeljenoMjesto == putnici[i]->dajMjesto())
                throw range_error ("Mjesto je vec zauzeto!");
        pogresan = false;
        }
        catch (range_error greska) {
        cout << greska.what ();
        system("pause");
        cin.clear(); cin.ignore(10000, '\n');
        }
    }
    putnici.push_back(make_shared<Let>(mjestoBoravka, destinacija, ime, prezime, dan, mjesec, godina, zeljenoMjesto));
    ObracunajCijenu(putnici[putnici.size()-1]);
return *this;
}

Avion& Avion::OdbijLet(shared_ptr<Let> osoba) {
    vector<shared_ptr<Let>> pomocni;
    for (int i=0; i<putnici.size(); i++) {
        if (strcmp(osoba->DajIme(), putnici[i]->DajIme()) == 0 && strcmp(osoba->DajPrezime(), putnici[i]->DajPrezime()) == 0 && putnici[i]->dajMjesto() == osoba->dajMjesto()) {
            putnici[i] = nullptr;
        cout << "Let odbijen!!";
        }
    }
    for (int i=0; i<putnici.size(); i++)
        if (putnici[i] != nullptr)
        pomocni.push_back(make_shared<Let>(*putnici[i]));
        putnici = move(pomocni);

return *this;
}

void Avion::PlatiCijenu (int indeks, shared_ptr<Let> osoba) {
    if (racuni.size() == 0)
        throw range_error ("Morate napraviti racun!");
    if (racuni[indeks-1] == nullptr)
        throw range_error ("Racun ne postoji!");
    if (racuni[indeks-1]->dajStanje() < osoba->dajCijenu()) {
        cout << "Nemate dovoljno sredstava na racunu!";
        OdbijLet(osoba);
        }
    else
        racuni[indeks-1]->Isplata(osoba->dajCijenu());

}
void Avion::ObracunajCijenu(shared_ptr<Let> osoba) {
    int odgovor;
    bool duze(true);
        cout << "Da li je putovanje duze od 1000(km)(1-DA): ";
         cin >> odgovor;
        if (odgovor != 1)
            duze = false;
        if (osoba->dajGodine() < 15 || osoba->dajGodine() > 65) {
            if (duze) {
                osoba->PostaviCijenu(500 - 500 * 0.25);
                cout << "Uspjeli ste ostvariti popust na osnovu vasih godina!\n";
                cout <<endl << "Cijena vaseg leta iznosi: " << osoba->dajCijenu() <<endl;
                int index;
            cout << "Indeks vaseg racuna: ";
             cin >> index;
             try {
        PlatiCijenu(index, osoba);
            }
            catch(range_error iz) {
            cout << iz.what();
            return;
            }
            return;
            }
            else {
                osoba->PostaviCijenu (300 - 300 * 0.25);
                cout << "Uspjeli ste ostvariti popust na osnovu vasih godina!\n";
                cout <<endl << "Cijena vaseg leta iznosi: " << osoba->dajCijenu() <<endl;
                int index;
            cout << "Indeks vaseg racuna: ";
             cin >> index;
             try {
        PlatiCijenu(index, osoba);
            }
            catch(range_error iz) {
            cout << iz.what();
            return;
            }
                return;
            }
        }
    if (duze)
        osoba->PostaviCijenu(500);
    else
        osoba->PostaviCijenu(300);
    int rac;
    cout <<endl << "Nazalost, niste zadovoljili uslove za popust!\n";
        int index;
            cout << "Indeks vaseg racuna: ";
             cin >> index;
             try {
        PlatiCijenu(index, osoba);
            }
            catch(range_error iz) {
            cout << iz.what();
            return;
            }
}

Avion& Avion::UplatinaRacun(int indeks) {
        int iznos;
        system("cls");
        cout << "Unesite novac koji zelite uplatiti na racun: ";
         cin >> iznos;
         try {
    racuni[indeks-1]->Uplata(iznos);
         }
         catch(range_error izuzetak) {
         cout << izuzetak.what();
         }
}

void Avion::ZauzetaMjesta() {
    if (putnici.size() == 0) {
        cout << "Nema putnika za ovaj let!";
        return;
    }
        cout << "Zauzeta su vam sljedeca mjesta: ";
    for (int i=0; i<putnici.size(); i++)
        cout << putnici[i]->dajMjesto() << " ";

}

Avion& Avion::OtkaziLet() {
    vector<shared_ptr<Let> > pomocni;
    shared_ptr<Let> osoba;
    bool pronadjen(false);
    char ime[20], prezime[20];
    int brojSjedista;
        cout << "Molimo, popunite sljedece podatke! \n";
         cout << "Unesite vase ime: ";
          cin >> ime;
        cout << "Unesite vase prezime: ";
         cin >> prezime;
        cout << "Broj sjedista: ";
         cin >> brojSjedista;
    for (int i=0; i<putnici.size(); i++) {
        if (strcmp(ime, putnici[i]->DajIme()) == 0 && strcmp(prezime, putnici[i]->DajPrezime()) == 0 && putnici[i]->dajMjesto() == brojSjedista) {
                int index;
            cout << "\nMolimo unesite, indeks racuna kako bi vam vratili polovinu novca: ";
                cin >> index;
            VratiPare(index, putnici[i]);
            putnici[i] = nullptr;
            cout << "Uspjesno ste otkazali let!";
            pronadjen = true;
        }
    }
    if (!pronadjen)
        throw range_error("Pogresni podaci leta!");
    for (int i=0; i<putnici.size(); i++) {
        if (putnici[i] != nullptr)
        pomocni.push_back(make_shared<Let>(*putnici[i]));
        putnici = move(pomocni);
        return *this;
    }
}

void Avion::VratiPare (int indeks, shared_ptr<Let> osoba) {
       if (racuni[indeks-1] == nullptr)
        throw range_error ("Racun ne postoji!");
        racuni[indeks-1]->Uplata(osoba->dajCijenu() / 2);
    cout << "\nVracena vam je polovina novca placenog za kartu!";
}

void Avion::IspisiPutnike() {
        if (putnici.size() == 0)
            cout << "Nema putnika na ovom letu!";
    for (int i=0; i<putnici.size(); i++)
        putnici[i]->Ispisnica();
}


int main() {
Avion boing;
        int indeks;
        int izbor, kraj;
    do {
        system ("cls");
    cout << "Dobrodosli u MENI!" <<endl;
    cout << "U nastavku, imate sljedece opcije:\n1-Otvaranje racuna\n2-Rezervacija leta\n3-Otkazivanje leta\n4-Zauzeta mjesta\n5-Spisak putnika\n6-Broj putnika\n7-Postavke racuna" <<endl;
    cout << "Vas izbor: ";
        cin >> izbor;
    switch(izbor) {
case 1:
    try {
    boing.OtvoriRacun();
    break;
    }
    catch(range_error izuzetak) {
    cout << izuzetak.what();
    }
case 2:
    try {
    boing.ZakaziLet();
    break;
    }
    catch(range_error izuzetak) {
        cout << izuzetak.what();
    }
case 3:
    try {
    boing.OtkaziLet();
    break;
    }
    catch (range_error izuzetak) {
        cout << izuzetak.what();
    }
case 4:
    boing.ZauzetaMjesta();
    break;
case 5:
    boing.IspisiPutnike();
    break;
case 6:
    cout << "Broj zauzetih mjesta: " << boing.BrojMjesta();
    break;
case 7:
        int indeks;
        cout << "Molimo prijavite se na racun, unesite indeks: ";
            cin >> indeks;
        try {
            int izbor2;
            system ("cls");
            boing.PrijavaRacuna(indeks);
            cout <<"\n"; boing.DajImeKorisnika(indeks); cout << " "; boing.DajPrezimeKorisnika(indeks);
            cout <<endl <<endl;
            cout << "Dostupne su sljedece postavke:\n1-Uplata na racun\n2-Informacije o racunu";
            cout << "\nVas izbor: ";
                cin >> izbor2;
            if (izbor2 == 1) {
                try {
                boing.UplatinaRacun(indeks);
                break;
                }
                catch(range_error izu) {
                cout << izu.what();
                break;
                }
            }
            else if (izbor2 == 2) {
                try {
                boing.InformacijeRacuna(indeks);
                }
                catch(range_error iz) {
                cout << iz.what();
                }
            break;
            }
         }
        catch(range_error izuzetak) {
            cout << izuzetak.what();
            break;
        }
default:
    cout << "Pogresna komanda!";
    break;
    }
     do {
    cout << "\n0-Izaci\n1-Main menu";
     cout << "\nVas izbor: ";
      cin >> kraj;
        }while(!(kraj == 1 || kraj == 0));
    }while(kraj == 1);
    return 0;
}
