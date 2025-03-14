#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

// Clasa de bază pentru bilete
class Bilet
{
protected:
    int numarBilet;
    int oraStart, oraFinal;
    string zona; // "urban" sau "interurban"

public:
    Bilet(int numar, int oraStart, int oraFinal, string zona)
        : numarBilet(numar), oraStart(oraStart), oraFinal(oraFinal), zona(zona) {}

    int calculDurata()
    {
        return oraFinal - oraStart;
    }

    virtual double calculCost() = 0; // Metodă pur virtuală

    void afisare()
    {
        cout << "Bilet #" << numarBilet << ", Durata: " << calculDurata()
             << " ore, Cost: " << calculCost() << " lei, Zona: " << zona << "\n";
    }

    virtual ~Bilet() {}
};

// Clasa pentru Bilet Standard (2 lei/oră în urban, 3 lei/oră în interurban)
class BiletStandard : public Bilet
{
public:
    BiletStandard(int numar, int oraStart, int oraFinal, string zona)
        : Bilet(numar, oraStart, oraFinal, zona) {}

    double calculCost() override
    {
        int durata = calculDurata();
        return (zona == "urban") ? durata * 2 : durata * 3;
    }
};

// Clasa pentru Abonament (50 lei/lună, dacă este inactiv: 1 leu/oră)
class Abonament : public Bilet
{
    bool activ;

public:
    Abonament(int numar, int oraStart, int oraFinal, string zona, bool activ)
        : Bilet(numar, oraStart, oraFinal, zona), activ(activ) {}

    double calculCost() override
    {
        return activ ? 0 : calculDurata() * 1;
    }
};

// Clasa pentru Bilet cu Reducere (1 leu/oră, dar se poate extinde pentru diferite reduceri)
class BiletReducere : public Bilet
{
    string tipReducere; // student, pensionar, etc.
public:
    BiletReducere(int numar, int oraStart, int oraFinal, string zona, string tipReducere)
        : Bilet(numar, oraStart, oraFinal, zona), tipReducere(tipReducere) {}

    double calculCost() override
    {
        return calculDurata() * 1; // Poți adăuga logica pentru reduceri diferite
    }
};

// Funcție pentru citirea biletelor din fișier
void citesteBilete(vector<Bilet *> &bilete)
{
    ifstream fisier("bilete.txt");
    if (!fisier)
    {
        cerr << "Eroare la deschiderea fișierului!\n";
        return;
    }

    int numar, oraStart, oraFinal;
    string tip, zona;

    while (fisier >> tip >> numar >> oraStart >> oraFinal >> zona)
    {
        if (tip == "standard")
        {
            bilete.push_back(new BiletStandard(numar, oraStart, oraFinal, zona));
        }
        else if (tip == "abonament")
        {
            bool activ;
            fisier >> activ; // Citim dacă abonamentul e activ (1 = da, 0 = nu)
            bilete.push_back(new Abonament(numar, oraStart, oraFinal, zona, activ));
        }
        else if (tip == "reducere")
        {
            string tipReducere;
            fisier >> tipReducere; // Citim tipul reducerii (ex: student, pensionar)
            bilete.push_back(new BiletReducere(numar, oraStart, oraFinal, zona, tipReducere));
        }
    }

    fisier.close();
}

// Funcție pentru afișarea și eliberarea memoriei
void afiseazaSiStergeBilete(vector<Bilet *> &bilete)
{
    for (auto bilet : bilete)
    {
        bilet->afisare();
        delete bilet;
    }
    bilete.clear();
}

int main()
{
    vector<Bilet *> bilete;
    citesteBilete(bilete);
    afiseazaSiStergeBilete(bilete);
    return 0;
}
