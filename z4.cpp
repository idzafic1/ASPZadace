#include <iostream>
#include <stdexcept>
using namespace std;

template<typename TipKljuca, typename TipVrijednosti>
class Mapa{
  public:
    Mapa () {}
    virtual ~Mapa(){}
    virtual TipVrijednosti operator[](const TipKljuca kljuc) const = 0;
    virtual TipVrijednosti& operator[](const TipKljuca kljuc) = 0;
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
    //Konstruktor bez parametara
    NizMapa(){
        vel = 1000000;
        brojEl = 0;
        niz = new std::pair<TipKljuca, TipVrijednosti>* [vel];
    }
    //Destruktor
    ~NizMapa(){
        obrisi();
        delete[] niz;
    }
    //Konstruktor kopije
    NizMapa(const NizMapa<TipKljuca, TipVrijednosti> &drugi){
        vel = drugi.vel;
        brojEl = drugi.brojEl;
        niz = new std::pair<TipKljuca, TipVrijednosti>* [vel];
        for(int i = 0; i < brojEl; i++){
            niz[i] = new std::pair<TipKljuca, TipVrijednosti> (*(drugi.niz[i]));
        }
    }
    //Operator dodjele
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
    
    //Operatori uglaste
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

    //Broj el    
    int brojElemenata()const {return brojEl;}

    //Brisanje po kljucu
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
    
    //Brisanje cijelog niza
    void obrisi(){
        for(int i = 0; i < brojEl; i++){
            delete niz[i];
        }
        brojEl = 0;
    }
};





 
template<typename TipKljuca, typename TipVrijednosti>
class  AVLStabloMapa : public Mapa<TipKljuca, TipVrijednosti> {
    public:
    class AVLCvor {
    public:
        TipKljuca kljuc;
        TipVrijednosti vrijednost;
        int balans=0;
        AVLCvor* lijevo=nullptr;
        AVLCvor* desno=nullptr;
        AVLCvor* roditelj=nullptr;
 
        AVLCvor(const TipKljuca& kljuc1, const TipVrijednosti& vrijednost1) : 
        kljuc(kljuc1), vrijednost(vrijednost1) {}
    };
    
    void Preorder() {
        PreorderHelper(korijenStabla);
        std::cout << std::endl;
    }
    private : 
 
    AVLCvor* korijenStabla;
    int brElem; 
 
    void PreorderHelper(AVLCvor* cvor) {
        if(!cvor) return;
        std::cout << cvor->kljuc << ",";
        PreorderHelper(cvor->lijevo);
        PreorderHelper(cvor->desno);
    }
 
    void funkcijaZaBalans(AVLCvor *&pom) {
        if(pom->roditelj==nullptr) return;
        bool a=true;
        if(!(pom->lijevo && pom->desno && pom->balans==0)) {
            if(pom->roditelj->lijevo==pom)
            pom->roditelj->balans++;
            else pom->roditelj->balans--;
        }
 
        AVLCvor *pok1 = pom->roditelj;
        AVLCvor *pok2 = pom;
        if(pom->balans>0 && pom->roditelj->balans <-1) {
            rotacijaUDesno(pok2);
            rotacijaULijevo(pok1);
            a=false;
        } else if(pom->balans <0 && pom->roditelj->balans >1) {
            rotacijaULijevo(pok2);
            rotacijaUDesno(pok1);
            a=false;
        } else if(pom->roditelj->balans <-1) {
            rotacijaULijevo(pok1);
            a=false;
        } else if(pom->roditelj->balans>1) {
            rotacijaUDesno(pok1);
            a=false;
        }
        if(a==true) funkcijaZaBalans(pom->roditelj);   
        }
 
 
        void rotacijaUDesno(AVLCvor *&pom) {
            bool a = false;
            if(pom->roditelj && pom->roditelj->lijevo==pom)
            pom->roditelj->lijevo=pom->lijevo;
            else if(pom->roditelj)
            pom->roditelj->desno=pom->lijevo;
            AVLCvor* pok1 = pom->lijevo;
            if(pom->lijevo->desno) {
                pom->lijevo=pok1->desno;
                pok1->desno->roditelj=pom;
                pok1->desno=nullptr;
                a=true;
            }
            pok1->roditelj=pom->roditelj;
            if(pom->roditelj)
            pom->roditelj=pok1;
            else {
            pom->roditelj=pok1;
            korijenStabla=pok1;
            }
            pok1->desno=pom;
            pok1->balans=0;
            pom->balans=0;
            if(!a) pom->lijevo=nullptr;
 
        }
 
 
 
         void rotacijaULijevo(AVLCvor *&pom) {
            bool a = false;
            if(pom->roditelj && pom->roditelj->lijevo==pom)
            pom->roditelj->lijevo=pom->desno;
            else if(pom->roditelj)
            pom->roditelj->desno=pom->desno;
            AVLCvor* pok1 = pom->desno;
            if(pom->desno->lijevo) {
                pom->desno=pok1->lijevo;
                pok1->lijevo->roditelj=pom;
                pok1->lijevo=nullptr;
                a=true;
            }
            pok1->roditelj=pom->roditelj;
            if(pom->roditelj)
            pom->roditelj=pok1;
            else {
            pom->roditelj=pok1;
            korijenStabla=pok1;
            }
            pok1->lijevo=pom;
            pok1->balans=0;
            pom->balans=0;
            if(!a) pom->desno=nullptr;
 
        }
 
    void dodajPomocna(AVLCvor *&pom) {
        AVLCvor *pok1=korijenStabla;
        AVLCvor *pok2=nullptr;
        bool a=false;
        while(pok1!=nullptr) {
            pok2=pok1;
            if(pom->kljuc < pok1->kljuc)
            pok1=pok1->lijevo;
            else pok1 = pok1->desno;
        }
 
        if(pok2==nullptr) {
            korijenStabla=pom;
            brElem++;
            a=true;
        } else {
            if(pom->kljuc < pok2->kljuc) {
                pok2->lijevo=pom;
                pom->roditelj=pok2;
                brElem++;
                if(pok2->desno) {
                    a=true;
                    pok2->balans=0;
                }
            } else {
                pok2->desno=pom;
                pom->roditelj=pok2;
                brElem++;
                if(pok2->lijevo) {
                    a=true;
                    pok2->balans=0;
                }
            }
        }
        if(!a)
        funkcijaZaBalans(pom);
 
    }
 
    TipVrijednosti& dodajCvor(TipKljuca kljuc) {
        AVLCvor* pom=new AVLCvor (kljuc,TipVrijednosti{});
        this->dodajPomocna(pom);
        return pom->vrijednost;
 
    }
 
    void pomocnaFunkcijaZaBrisanje(AVLCvor *cvor) {
        if(!cvor) return;
        pomocnaFunkcijaZaBrisanje(cvor->lijevo);
        pomocnaFunkcijaZaBrisanje(cvor->desno);
        delete cvor;
        brElem=brElem-1;
    }
 
    void pomocnaFunkcijaZaKopiranje(AVLCvor *drugi) {
        if(drugi==nullptr) return;
        this->dodajCvor(drugi->kljuc)=drugi->vrijednost;
        pomocnaFunkcijaZaKopiranje(drugi->lijevo);
        pomocnaFunkcijaZaKopiranje(drugi->desno);
 
    }
 
    void pomocnaFunkcijaZaBrisanje1(const TipKljuca&kljuc1) {
        AVLCvor *pom = korijenStabla;
        AVLCvor *pom1 = 0;
        AVLCvor *pom2 = 0;
        AVLCvor *pom3= 0;
        AVLCvor *pom4 = 0;
 
        while(pom != 0 && kljuc1 != pom->kljuc) {
            pom1=pom;
            if(kljuc1<pom->kljuc)
            pom=pom->lijevo;
            else pom=pom->desno;
        }
 
        if(pom==0) throw("Greska");
        if(pom->lijevo==0) 
        pom4=pom->desno;
        else if(pom->desno==0)
        pom4=pom->lijevo;
        else {
            pom3=pom;
            pom4=pom->lijevo;
            pom2=pom4->desno;
            while(pom2 != 0) {
                pom3=pom4;
                pom4=pom2;
                pom2=pom4->desno;
            }
            if(pom3 != pom) {
                pom3->desno = pom4->lijevo;
                pom4->lijevo=pom->lijevo;
                pom3->roditelj=pom4;
                pom4->roditelj=pom1;
                if(pom3->desno)
                pom3->desno->roditelj=pom3;
            }
            pom4->desno=pom->desno;
            pom->desno->roditelj=pom4;
        }
        if(pom1==0) {
            korijenStabla=pom4;
            if(korijenStabla) 
            korijenStabla->roditelj=nullptr;
        } else if(pom==pom1->lijevo) {
            pom1->lijevo=pom4;
            if(pom4)
            pom4->roditelj=pom1;
        } else {
            pom1->desno=pom4;
            if(pom4)
            pom4->roditelj=pom1;
        }
        if(pom->roditelj && pom->roditelj->lijevo == pom)
        pom->roditelj->balans--;
        else if(pom->roditelj) pom->roditelj->balans++;
        delete pom;
        brElem=brElem-1;
    }
 
 
    public:
    AVLStabloMapa() {
        korijenStabla=nullptr;
        brElem=0;
    }
    ~AVLStabloMapa() {
        pomocnaFunkcijaZaBrisanje(korijenStabla);
 
 
    }
    AVLStabloMapa(const AVLStabloMapa<TipKljuca,TipVrijednosti> &mapa) {
        korijenStabla=nullptr;
        brElem=0;
        AVLCvor *pom=mapa.korijenStabla;
        pomocnaFunkcijaZaKopiranje(pom);
 
    }
    AVLStabloMapa<TipKljuca,TipVrijednosti> & operator =(const AVLStabloMapa<TipKljuca,TipVrijednosti> &mapa) {
        if(this==&mapa) return *this;
        pomocnaFunkcijaZaBrisanje(korijenStabla);
         korijenStabla=nullptr;
        AVLCvor *pom=mapa.korijenStabla;
        pomocnaFunkcijaZaKopiranje(pom);
        return *this;
 
    }
 
    TipVrijednosti& operator [] (TipKljuca kljuc1) {
        auto pom=korijenStabla;
        while(pom!=0 && kljuc1 != pom->kljuc) {
            if(kljuc1<pom->kljuc)
            pom=pom->lijevo;
            else pom=pom->desno;
        }
        if(pom!=nullptr) {
            return pom->vrijednost;
        }
        AVLCvor *pok1 = new AVLCvor (kljuc1,TipVrijednosti{});
        this->dodajPomocna(pok1);
        return pok1->vrijednost;
    } 
 
    TipVrijednosti operator [] (TipKljuca kljuc1) const {
        auto pom=korijenStabla;
        while(pom!=0 && kljuc1 != pom->kljuc) {
            if(kljuc1<pom->kljuc)
            pom=pom->lijevo;
            else pom=pom->desno;
        }
        if(pom!=nullptr) {
            return pom->vrijednost;
        }
        TipVrijednosti rez{};
        return rez;
    } 
 
 
    int brojElemenata() const {
        return brElem;
    }
 
    void obrisi() {
        pomocnaFunkcijaZaBrisanje(korijenStabla);
        korijenStabla=nullptr;
        brElem=0;
    }
 
    void obrisi(const TipKljuca& kljuc) {
 
        pomocnaFunkcijaZaBrisanje1(kljuc);
    }
};
template<typename TipKljuca, typename TipVrijednosti>
class BinStabloMapa : public Mapa<TipKljuca, TipVrijednosti>
{
public:
    class Cvor
    {
    public:
        TipKljuca kljuc;
        TipVrijednosti vrijednost;
        Cvor* lijevo;
        Cvor* desno;
        Cvor* roditelj;
        Cvor()
        {
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

    TipVrijednosti& Umetni (Cvor* &novi)
    {
        Cvor *p = start;
        Cvor *q = nullptr;

        if(start == nullptr) {
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

    TipVrijednosti& dodaj(TipKljuca k)
    {
        Cvor* t = new Cvor(k, TipVrijednosti{});
        this->Umetni(t);
        return t->vrijednost;
    }

    void Preorder (Cvor* c)
    {
        if(c) {
            this->dodaj(c->kljuc) = c->vrijednost;
            if(c != nullptr) Preorder(c->lijevo);
            if(c != nullptr) Preorder(c->desno);
        }
    }

    void brisanje(Cvor* c)
    {
        if(c) {
            brisanje(c->lijevo);
            brisanje(c->desno);
            delete c;
        }
        start = nullptr;
    }

public:
    //Konstruktor bez parametara
    BinStabloMapa()
    {
        start = nullptr;
        broj_Elemenata = 0;
    }

    BinStabloMapa(const BinStabloMapa &nova)
    {
        start = nullptr;
        broj_Elemenata = 0;
        Cvor* t = nova.start;
        Preorder(t);
    }

    BinStabloMapa& operator =(const BinStabloMapa &nova)
    {
        if(this == &nova) return *this;
        this->obrisi();
        start = nullptr;
        Cvor* t = nova.start;
        Preorder(t);
        return *this;
    }

    //Destruktor
    ~BinStabloMapa()
    {
        brisanje(start);
    }
    //Operatori uglaste
    TipVrijednosti operator[] (const TipKljuca &kljuc) const
    {
        auto cc =  start;
        while(1) {
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

    TipVrijednosti& operator[] (const TipKljuca &kljuc)
    {
        auto cc =  start;
        while(1) {
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
    //Broj elemenata
    int brojElemenata()const
    {
        return broj_Elemenata;
    }
    //Brisanje po kljucu
    void obrisi(const TipKljuca& kljuc)
    {
        Cvor *p = start, *q = 0;
        Cvor* tmp = 0, *pp = 0, *rp = 0;
        while(p != 0 && kljuc != p->kljuc) {
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
        else {
            pp = p;
            rp = p->lijevo;
            tmp = rp->desno;
            while(tmp != 0) {
                pp = rp;
                rp = tmp;
                tmp = rp->desno;
            }
            if(pp != p) {
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
    //Brisanje svih elemenata
    void obrisi()
    {
        brisanje(start);
        broj_Elemenata = 0;
    }


};


int main()
{
    std::cout << "Zadaća 4, Zadatak 1";
    return 0;
}
