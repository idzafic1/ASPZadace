#include <iostream>
#include <vector>

template <typename TipOznake>
class Cvor;
template <typename TipOznake>
class Grana;
template <typename TipOznake>
class GranaIterator;
template <typename TipOznake>
class MatricaGraf;

template <typename TipOznake>
class UsmjereniGraf
{
public:
    UsmjereniGraf() = default;
    virtual ~UsmjereniGraf() {}
    virtual int dajBrojCvorova() = 0;
    virtual void postaviBrojCvorova(int br) = 0;
    virtual void dodajGranu(int pol, int dol, float tez) = 0;
    virtual void obrisiGranu(int pol, int dol) = 0;
    virtual void postaviTezinuGrane(int pol, int dol, float tez) = 0;
    virtual void dajTezinuGrane(int pol, int dol) = 0;
    virtual bool postojiGrana(int pol, int dol) = 0;
    virtual void postaviOznakuCvora(int br, TipOznake oznaka) = 0;
    virtual TipOznake dajOznakuCvora(int cvor) = 0;
    virtual void postaviOznakuGrane(int pol, int dol, TipOznake oznaka) = 0;
    virtual TipOznake dajOznakuGrane(int pol, int dol) = 0;
    virtual Grana<TipOznake> dajGranu(int pol, int dol) = 0;
    virtual Cvor<TipOznake> dajCvor(int br) = 0;
    virtual GranaIterator<TipOznake> dajGranePocetak() = 0;
    virtual GranaIterator<TipOznake> dajGraneKraj() = 0;
};

template <typename TipOznake>
class Cvor
{
    TipOznake oznaka;
    int redniBroj;
    UsmjereniGraf<TipOznake> *pokNaGraf;
public:
    Cvor(TipOznake oznaka, int red, UsmjereniGraf<TipOznake> *pokNaGraf) : pokNaGraf(pokNaGraf)
    {
        this->oznaka = oznaka;
        this->redniBroj = red;
    }
    Cvor() : oznaka(TipOznake{}), redniBroj(0), pokNaGraf(nullptr) {}
    TipOznake dajOznaku() {
        return oznaka;
    }
    void postaviOznaku(TipOznake o)
    {
        pokNaGraf->postaviOznakuCvora(redniBroj, o);
    }
    void postaviOznaku2(TipOznake o)
    {
        oznaka = o;
    }
    int dajRedniBroj()
    {
        return redniBroj;
    }
    friend class Grana<TipOznake>;
};

template <typename TipOznake>
class Grana
{
    float tez;
    TipOznake oznaka;
    Cvor<TipOznake> *pol, *dol;
public:
    Grana()
    {
        tez = 0;
        oznaka = TipOznake{};
        pol = nullptr;
        dol = nullptr;
    }
    Grana(float tez, TipOznake oznaka, Cvor<TipOznake> *polaz, Cvor<TipOznake> *dolaz) :
        pol(polaz), dol(dolaz)
    {
        this->tez = tez;
        this->oznaka = oznaka;
    }
    float dajTezinu()
    {
        return tez;
    }
    void postaviTezinu(float t)
    {
        pol->pokNaGraf->postaviTezinuGrane(pol->dajRedniBroj(), dol->dajRedniBroj(), t);
    }
    void postaviTezinu2(float t)
    {
        tez = t;
    }
    TipOznake dajOznaku()
    {
        return oznaka;
    }
    void postaviOznaku(TipOznake oz)
    {
        pol->pokNaGraf->postaviOznakuGrane(pol->dajRedniBroj(), dol->dajRedniBroj(), oz);
    }
    void postaviOznaku2(TipOznake oz)
    {
        oznaka = oz;
    }
    Cvor<TipOznake> dajPolazniCvor(){
        return *pol;
    }
    Cvor<TipOznake> dajDolazniCvor(){
        return *dol;
    }

};

template <typename TipOznake>
class MatricaGraf : public UsmjereniGraf<TipOznake>
{
public:
    std::vector<std::vector<Grana<TipOznake>*>> mat;
    std::vector<Cvor<TipOznake>*> cvorovi;
    int brP = 0;

public:
    MatricaGraf(int brCvorova)
    {
        mat.resize(brCvorova);
        int br = brCvorova;
        for(int i = 0; i < br; i++) mat[i].resize(brCvorova, nullptr);
        cvorovi.resize(brCvorova, nullptr);
        for(int i = 0; i < br; i++) cvorovi[i] = new Cvor<TipOznake>(TipOznake{}, i, this);
    }

    ~MatricaGraf()
    {
        int vel = mat.size();
        for(int i = 0; i < vel; i++) {
            for(int j = 0; j < vel; j++) delete mat[i][j];
        }
        int vel2 = cvorovi.size();
        for(int i = 0; i< vel2; i++) delete cvorovi[i];
    }

    int dajBrojCvorova()
    {
        return cvorovi.size();
    }

    void postaviBrojCvorova(int brCvorova)
    {
        if(brCvorova < cvorovi.size()) throw ("Nedozvoljeno smanjivanje!");
        mat.resize(brCvorova);
        int vel = brCvorova;
        for(int i = 0; i < vel; i++) mat[i].resize(brCvorova, nullptr);
        int old = cvorovi.size();
        cvorovi.resize(brCvorova, nullptr);
        for(int i = old; i< vel; i++) cvorovi[i] = new Cvor<TipOznake>(TipOznake{}, i, this);
    }

    void dodajGranu(int pol, int dol, float tez)
    {
        if(mat[pol][dol] != nullptr) throw "Nedozvoljeno!";
        if(pol > cvorovi.size())     throw "Nedozvoljeno!";
        if(dol > cvorovi.size())     throw "Nedozvoljeno!";
        mat[pol][dol] = new Grana<TipOznake>(tez, TipOznake{}, cvorovi[pol], cvorovi[dol]);
    }

    void obrisiGranu(int pol, int dol)
    {
        if(mat[pol][dol] == nullptr) throw "Nepostojeca grana!";
        if(pol > cvorovi.size())     throw "pol>cvorovi.size()!";
        if(dol > cvorovi.size())     throw "dol>cvorovi.size()!";
        delete mat[pol][dol];
        mat[pol][dol] = nullptr;
    }

    void postaviTezinuGrane(int pol, int dol, float tez)
    {
        if(mat[pol][dol] == nullptr) throw "Nepostojeca grana!";
        if(pol > cvorovi.size())     throw "pol>cvorovi.size()!";
        if(dol > cvorovi.size())     throw "dol>cvorovi.size()!";
        mat[pol][dol]->postaviTezinu2(tez);
    }

    void dajTezinuGrane(int pol, int dol)
    {
        if(mat[pol][dol] == nullptr) throw "Nepostojeca grana!";
        if(pol > cvorovi.size())     throw "pol>cvorovi.size()!";
        if(dol > cvorovi.size())     throw "dol>cvorovi.size()!";
        mat[pol][dol]->dajTezinu();
    }

    bool postojiGrana(int pol, int dol)
    {
        if(pol > cvorovi.size() ||dol > cvorovi.size()) return false;
        return mat[pol][dol];
    }

    TipOznake dajOznakuCvora(int br)
    {
        if(cvorovi[br] == nullptr) throw "Nepostojeci cvor!";
        return cvorovi[br]->dajOznaku();
    }

    void postaviOznakuCvora(int br, TipOznake oznaka)
    {
        if(cvorovi[br] == nullptr) throw "Nepostojeci cvor!";
        cvorovi[br]->postaviOznaku2(oznaka);
    }

    TipOznake dajOznakuGrane(int pol, int dol)
    {
        if(mat[pol][dol] == nullptr) throw "Nepostojeca grana!";
        if(pol > cvorovi.size()) throw "pol>cvorovi.size()!";
        if(dol > cvorovi.size())     throw "dol>cvorovi.size()!";
        return mat[pol][dol]->dajOznaku();
    }

    Grana<TipOznake> dajGranu(int pol, int dol)
    {
        if(mat[pol][dol] == nullptr) throw "Nepostojeca grana!";
        if(pol > cvorovi.size()) throw "pol>cvorovi.size()!";
        if(dol > cvorovi.size())     throw "dol>cvorovi.size()!";
        return *mat[pol][dol];
    }

    void postaviOznakuGrane(int pol, int dol, TipOznake oznaka)
    {
        if(mat[pol][dol] == nullptr) throw "Nepostojeca grana!";
        if(pol > cvorovi.size()) throw "pol>cvorovi.size()!";
        if(dol > cvorovi.size())     throw "dol>cvorovi.size()!";
        mat[pol][dol]->postaviOznaku2(oznaka);
    }

    Cvor<TipOznake> dajCvor(int br)
    {
        if(cvorovi[br] == nullptr) throw "Ne postoji cvor!";
        return *cvorovi[br];
    }

    virtual GranaIterator<TipOznake> dajGranePocetak()
    {
        int vel = mat.size();
        for(int i = 0; i < vel; i++)
            for(int j = 0; j < vel; j++)
                if(mat[i][j] != nullptr)
                    return GranaIterator<TipOznake> (this, i, j);
        return GranaIterator<TipOznake>(this, 0,0);
    }

    virtual GranaIterator<TipOznake> dajGraneKraj()
    {
        for(int i = mat.size() - 1; i > 0; i--)
            for(int j = mat.size() - 1; j > 0; j--)
                if(mat[i][j] != nullptr) {
                    if(j < mat.size() - 1) return GranaIterator<TipOznake>(this, i, j+1);
                    else return GranaIterator<TipOznake> (this, i+1, 0);
                }
        return GranaIterator<TipOznake> (this, mat.size() - 1, mat.size() - 1);
    }

    friend class GranaIterator<TipOznake>;

    template<typename TipOznake1>
    friend void bfs(UsmjereniGraf<TipOznake1> *graf, std::vector<Cvor<bool>> &obilazak, Cvor<TipOznake1> cvor, int sljedeci);
};

template<typename TipOznake>
void dfs(UsmjereniGraf<TipOznake> *&g, std::vector<Cvor<TipOznake>> &obilazak, Cvor<TipOznake> cvor)
{
    static std::vector<bool> pos(g->dajBrojCvorova(), false);
    pos[cvor.dajRedniBroj()] = true;
    obilazak.push_back(cvor);
    int redni = cvor.dajRedniBroj();
    int vel = g->dajBrojCvorova();
    for(int i = 0; i < vel; i++)
        if(g->postojiGrana(redni, i) && !pos[i])
            dfs(g, obilazak, g->dajCvor(i));

}

template<typename TipOznake1>
void bfs(UsmjereniGraf<TipOznake1> *g, std::vector<Cvor<bool>> &obilazak, Cvor<TipOznake1> cvor, int sljedeci = 0)
{
    MatricaGraf<TipOznake1> *mat2 = dynamic_cast<MatricaGraf<TipOznake1>*>(g);
    int vel1 = mat2->brP;
    int vel2 = mat2->dajBrojCvorova();
    if(vel1 == vel2) {
        mat2->brP = 0;
        return;
    }
    auto pos = TipOznake1(1);
    if(mat2->cvorovi[cvor.dajRedniBroj()]->dajOznaku() != pos) {
        mat2->cvorovi[cvor.dajRedniBroj()]->postaviOznaku(pos);
        obilazak.push_back(cvor);
        mat2->brP++;
    }
    for(int i = 0; i < mat2->dajBrojCvorova(); i++) {
        if(mat2->mat[cvor.dajRedniBroj()][i] != nullptr && mat2->mat[cvor.dajRedniBroj()][i]->dajDolazniCvor().dajOznaku() != pos) {
            obilazak.push_back(mat2->mat[cvor.dajRedniBroj()][i]->dajDolazniCvor());
            mat2->cvorovi[i]->postaviOznaku(pos);
            mat2->brP++;
        }
    }

    bfs(g,obilazak, obilazak[++sljedeci], sljedeci);
}


template <typename TipOznake>
class GranaIterator
{
    MatricaGraf<TipOznake> *mat;
    int pol, dol;
public:
    GranaIterator(MatricaGraf<TipOznake> *pok, int pol, int dol): mat(pok), pol(pol), dol(dol) {}
    Grana<TipOznake> operator *()
    {
        if(mat->mat[pol][dol]) return *(mat->mat[pol][dol]);
        throw "Nemoguce";
    }

    bool operator ==(const GranaIterator &iter) const
    {
        return (mat == iter.mat && pol == iter.pol && dol == iter.dol);
    }

    bool operator !=(const GranaIterator &iter) const
    {
        return !(mat == iter.mat && pol == iter.pol && dol == iter.dol);
    }

   GranaIterator& operator++(){
        if(dol == mat->mat.size() - 1 && pol == mat->mat.size() - 1)
            throw "End";
        GranaIterator<TipOznake> tmp = mat->dajGraneKraj();
        int x(tmp.pol);
        int y(tmp.dol);
        bool breakaj = false;
        int vel = mat->mat.size();
        for(int i = 0; i < vel; i++){
            for(int j = 0; j < vel; j++){
                if(i == x && j == y){
                    pol = x;
                    dol = y;
                    return *this;
                }
                else if(((i == pol && j > dol) || i > pol) && mat->mat[i][j] != nullptr){
                    pol = i;
                    dol = j;
                    breakaj = true;
                    break;
                }
            }
            if(breakaj) break;
        }
        return *this;
    }
    
    GranaIterator& operator++(int){
        GranaIterator<TipOznake> old(mat, pol, dol);
        if(dol == mat->mat.size() - 1 && pol == mat->mat.size() - 1)
            throw "End";
        GranaIterator<TipOznake> tmp = mat->dajGraneKraj();
        int x(tmp.pol);
        int y(tmp.dol);
        bool breakaj = false;
        int vel = mat->mat.size();
        for(int i = 0; i < vel; i++){
            for(int j = 0; j < vel; j++){
                if(i == x && j == y){
                    old.pol = x;
                    old.dol = y;
                    return old;
                }
                else if(((i == pol && j > dol) || i > pol) && mat->mat[i][j] != nullptr){
                    pol = i;
                    dol = j;
                    breakaj = true;
                    break;
                }
            }
            if(breakaj) break;
        }
        return old;
    }
};


using namespace std;

int main()
{
    //DFS test
    UsmjereniGraf<bool> *g = new MatricaGraf<bool>(6);
    g->dodajGranu(0, 1, 2.5);
    g->dodajGranu(1, 2, 1.2);
    g->dodajGranu(1, 3, 0.1);
    g->dodajGranu(2, 4, 3.14);
    g->dodajGranu(2, 3, 2.73);
    g->dodajGranu(3, 5, 1);
    g->dodajGranu(5, 2, -5.0);
    std::vector<Cvor<bool>> dfs_obilazak;
    dfs(g,dfs_obilazak,g->dajCvor(0));
    for(int i = 0; i < dfs_obilazak.size(); i++){
        if(i == dfs_obilazak.size() - 1) cout<<dfs_obilazak[i].dajRedniBroj();
        else cout<<dfs_obilazak[i].dajRedniBroj()<<",";
    } 
    delete g;
    cout<<"\n";
    
    //BFS test
    
 UsmjereniGraf<bool> *a = new MatricaGraf<bool>(6);
    a->dodajGranu(0, 1, 2.5);
    a->dodajGranu(1, 2, 1.2);
    a->dodajGranu(1, 3, 0.1);
    a->dodajGranu(2, 4, 3.14);
    a->dodajGranu(2, 3, 2.73);
    a->dodajGranu(3, 5, 1);
    a->dodajGranu(5, 2, -5.0);
    std::vector<Cvor<bool> > bfs_obilazak;
    bfs(a, bfs_obilazak, a->dajCvor(0));
    for(int i = 0; i < bfs_obilazak.size(); i++){
        if(i == bfs_obilazak.size() - 1) cout<<bfs_obilazak[i].dajRedniBroj();
        else cout<<bfs_obilazak[i].dajRedniBroj()<<",";
    } 
  delete a;
    return 0;
}
