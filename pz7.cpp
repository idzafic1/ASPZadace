#include <iostream>
#include <stdexcept>
#include <ctime>
#include <random>

template<typename TipKljuca, typename TipVrijednosti>
class Mapa{
  public:
    virtual ~Mapa(){}
    virtual TipVrijednosti operator[](const TipKljuca& kljuc) const = 0;
    virtual TipVrijednosti& operator[](const TipKljuca& kljuc) = 0;
    virtual int brojElemenata() const = 0;
    virtual void obrisi() = 0;
    virtual void obrisi(const TipKljuca& kljuc) = 0;
};

template<typename TipKljuca, typename TipVrijednosti>
class NizMapa : public Mapa<TipKljuca, TipVrijednosti>{
private:
    int vel;
    int brojEl;
    std::pair<TipKljuca, TipVrijednosti>** niz;
    
public:

    NizMapa(){
        vel = 1000000;
        brojEl = 0;
        niz = new std::pair<TipKljuca, TipVrijednosti>* [vel];
    }

    ~NizMapa(){
        obrisi();
        delete[] niz;
    }

    NizMapa(const NizMapa<TipKljuca, TipVrijednosti> &drugi){
        vel = drugi.vel;
        brojEl = drugi.brojEl;
        niz = new std::pair<TipKljuca, TipVrijednosti>* [vel];
        for(int i = 0; i < brojEl; i++){
            niz[i] = new std::pair<TipKljuca, TipVrijednosti> (*(drugi.niz[i]));
        }
    }

    NizMapa &operator= (const NizMapa<TipKljuca, TipVrijednosti> &drugi){
        if(this == &drugi) return *this;
       obrisi();
       vel = drugi.vel;
       brojEl = drugi.brojEl;
        for(int i = 0; i < brojEl; i++){
            niz[i] = new std::pair<TipKljuca, TipVrijednosti> (*(drugi.niz[i]));
        }
        return *this;
    }
    
    TipVrijednosti operator[] (const TipKljuca &kljuc) const{
        for(int i = 0; i < brojEl; i++){
            if(niz[i]->first == kljuc){
                return niz[i]->second;
            }
        }
        TipVrijednosti a = {};
        return a;
    }
    
    TipVrijednosti& operator[] (const TipKljuca &kljuc){
        for(int i = 0; i < brojEl; i++){
            if(niz[i]->first == kljuc){
                return niz[i]->second;
            }
        }
        niz[brojEl++] = new std::pair<TipKljuca, TipVrijednosti>(kljuc, TipVrijednosti());
        return niz[brojEl - 1]->second;
    }

    int brojElemenata()const {return brojEl;}

    void obrisi(const TipKljuca& kljuc){
        if(brojEl == 0)
            throw "Nema elemenata za obrisati!\n";
        int trigger = 0;
        for(int i = 0; i < brojEl; i++){
            if(niz[i]->first == kljuc){
                trigger = 1;
                delete niz[i];
                for(int j = i; j < brojEl-1; j++){
                    niz[j] = niz[j+1];
                }  
                break;
            }
        }
        if(trigger)
            brojEl--;
        
    }
    
    void obrisi(){
        for(int i = 0; i < brojEl; i++){
            delete niz[i];
        }
        brojEl = 0;
    }
};


template<typename TipKljuca, typename TipVrijednosti>
class BinStabloMapa : public Mapa<TipKljuca, TipVrijednosti>{
public:
    class Cvor{
    public:
        TipKljuca kljuc;
        TipVrijednosti vrijednost;
        Cvor* lijevo;
        Cvor* desno;
        Cvor* roditelj;
        Cvor(){
            lijevo = nullptr;
            desno = nullptr;
            roditelj = nullptr;
        }
        
        Cvor(const TipKljuca& kljuc, const TipVrijednosti& vrijednost,
            Cvor* lijevo = nullptr, Cvor* desno = nullptr, Cvor* roditelj = nullptr) :
            kljuc(kljuc), vrijednost(vrijednost), lijevo(lijevo), desno(desno), roditelj(roditelj) {} 
        
    };

    Cvor* start;
    int broj_Elemenata;
    
    TipVrijednosti& Umetni (Cvor* &novi){
        Cvor *p = start;
        Cvor *q = nullptr;
        
        if(start == nullptr){
            start = novi;
            broj_Elemenata++;
            return start->vrijednost;
        }
        
        while (p) {
            q = p;
            if(novi->kljuc > p->kljuc) p = p->desno;
            else p = p->lijevo;
        }
        
        p = novi;
        p->roditelj = q;
        if(p->kljuc > q->kljuc) q->desno = p;
        else q->lijevo = p;
        broj_Elemenata++;
        
        return p->vrijednost;
        
    }

    TipVrijednosti& dodaj(TipKljuca k){
        Cvor* t = new Cvor(k, TipVrijednosti{});
        this->Umetni(t);
        return t->vrijednost;
    }
    
    void Preorder (Cvor* c){
        if(c){
            this->dodaj(c->kljuc) = c->vrijednost;
            if(c != nullptr) Preorder(c->lijevo);
            if(c != nullptr) Preorder(c->desno);
        }
    }
    
    void brisanje(Cvor* c){
        if(c){
            brisanje(c->lijevo);
            brisanje(c->desno);
            delete c;
        }
        start = nullptr;
    }
    
public:

    BinStabloMapa(){
        start = nullptr;
        broj_Elemenata = 0;
    }
    
    BinStabloMapa(const BinStabloMapa &nova){
        start = nullptr;
        broj_Elemenata = 0;
        Cvor* t = nova.start;
        Preorder(t);
    }
    
    BinStabloMapa& operator =(const BinStabloMapa &nova){
        if(this == &nova) return *this;
        this->obrisi();
        start = nullptr;
        Cvor* t = nova.start;
        Preorder(t);
        return *this;
    }
    
    ~BinStabloMapa(){
        brisanje(start);
    }
 
    TipVrijednosti operator[] (const TipKljuca &kljuc) const{
        auto cc =  start;
        while(1){
            if(cc == 0 || kljuc == cc->kljuc)
                break;
            if(kljuc < cc->kljuc)
                cc = cc->lijevo;
            else
                cc = cc->desno;
        }
        if(cc != nullptr)
            return cc->vrijednost;
        TipVrijednosti povratni{};
        return povratni;
    }
    
    TipVrijednosti& operator[] (const TipKljuca &kljuc){
        auto cc =  start;
        while(1){
            if(cc == 0 || kljuc == cc->kljuc)
                break;
            if(kljuc < cc->kljuc)
                cc = cc->lijevo;
            else
                cc = cc->desno;
        }
        if(cc != nullptr)
            return cc->vrijednost;
        Cvor* temp =  new Cvor(kljuc, TipVrijednosti{});
        this->Umetni(temp);
        return temp->vrijednost;
    }
    
    int brojElemenata()const {return broj_Elemenata;}
    
    void obrisi(const TipKljuca& kljuc){ 
        Cvor *p = start, *q = 0;
        Cvor* tmp = 0, *pp = 0, *rp = 0;
        while(p != 0 && kljuc != p->kljuc){
            q = p;
            if(kljuc < p->kljuc)
                p = p->lijevo;
            else
                p = p->desno;
        }
        if(p == 0)
            throw std::logic_error("Nije pronadjen");
        if(p->lijevo == 0)
            rp = p->desno;
        else if(p->desno == 0)
            rp = p->lijevo;
        else{
            pp = p;
            rp = p->lijevo;
            tmp = rp->desno;
            while(tmp != 0){
                pp = rp;
                rp = tmp;
                tmp = rp->desno;
            }
            if(pp != p){
                pp->desno = rp->lijevo;
                rp->lijevo = p->lijevo;
                pp->roditelj = rp;
                rp->roditelj = q;
                if(pp->desno)
                    pp->desno->roditelj = pp;
            }
            rp->desno = p->desno;
        }
        if(q == 0)
            start = rp;
        else if(p == q->lijevo)
            q->lijevo = rp;
        else
            q->desno = rp;
        delete p;
        broj_Elemenata--;
    }
    
    void obrisi(){
       brisanje(start);
       broj_Elemenata = 0;
    }
    
    
};

using namespace std;
int main() {

    BinStabloMapa<int, int> bsm;
    NizMapa<int, int> nm;
    int time_1 = clock();
    bsm[0] = 12345;
    int time_2 = clock();
    int total = (time_2 - time_1) / (CLOCKS_PER_SEC/100000);
    std::cout<< "Vrijeme dodavanja malog broja elemenata u binarno stablo mapu iznosi: " << total << " ms/100." << std::endl; 
    time_1 = clock();
    nm[0] = 12345;
    time_2 = clock();
    total = (time_2 - time_1) / (CLOCKS_PER_SEC/100000);
    std::cout<< "Vrijeme dodavanja malog broja elementa u niz mapu iznosi: " << total << " ms/100." << std::endl; 
    
    int random_1;
    time_1  = clock();
    for(int i = 0; i < 10000; i++){
       random_1 = rand() % 1000;
       bsm[random_1] = 1;
    }
    time_2 = clock();
    total = (time_2 - time_1) / (CLOCKS_PER_SEC/100000);
    std::cout<< "Vrijeme dodavanja velikog broja elemenata u binarno stablo mapu iznosi: " << total << " ms/100." << std::endl; 
    
    time_1  = clock();
    for(int i = 0; i < 10000; i++){
       random_1 = rand() % 1000;
       nm[random_1] = 1;
    }
    time_2 = clock();
    total = (time_2 - time_1) / (CLOCKS_PER_SEC/100000);
    std::cout<< "Vrijeme dodavanja velikog broja elementa u niz mapu iznosi: " << total << " ms/100." << std::endl; 
    
    time_1 = clock();
    int drugi_broj = bsm[25655];
    time_2 = clock();
    total = (time_2 - time_1) / (CLOCKS_PER_SEC/100000);
    std::cout<< "Vrijeme pristupa postojecem elementu za binarno stablo mapu iznosi: " << total << " ms/100." << std::endl; 
    
    time_1 = clock();
    int prvi_broj = nm[25655];
    time_2 = clock();
    total = (time_2 - time_1) / (CLOCKS_PER_SEC/100000);
    std::cout<< "Vrijeme pristupa postojecem elementu za niz mapu iznosi: " << total << " ms/100." << std::endl; 
    
    if(prvi_broj == drugi_broj)
        std::cout<< "TestDobar!" << std::endl;
    return 0;
}

