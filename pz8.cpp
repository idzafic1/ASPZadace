#include <iostream>
#include <fstream>
#include <string>
using namespace std;
//bubble sort
template<typename Tip>
void bubble_sort(Tip *niz, int vel){
    for(int i=vel-1;i>0;i--){
        for(int j=1;j<=i;j++){
            if(niz[j-1]>niz[j]){
                Tip pom = niz[j-1];
                niz[j-1] = niz[j];
                niz[j] = pom;
            }
        }
    }
}

//selection sort
template<typename Tip>
void selection_sort(Tip* niz, int vel){
    for(int i=0;i<vel-1;i++){
        Tip min = niz[i];
        int pmin = i;
        for(int j=i+1; j<vel;j++){
            if(niz[j]<min){
                min=niz[j];
                pmin=j;
            }
        }
        niz[pmin] = niz[i];
        niz[i] = min;
    }
}


//quick sort
template<typename Tip>
void Zamijeni(Tip *niz, const int i, const int j){
    Tip pom = niz[i];
    niz[i] = niz[j];
    niz[j] = pom;
}

template<typename Tip>
int Particija(Tip *niz, const int p, const int z){
    Tip pivot = niz[p];
    int pom = p+1;
    while(pom<=z){
       if(niz[pom] <= pivot)
       pom++;
       else
       break;
    }
    for(int i = pom+1; i<=z; i++){
        if(niz[i]<pivot){
            Zamijeni(niz,pom,i);
            pom++;
        }
    }
    Zamijeni(niz,p,pom-1);
    return pom-1;
    
}


template<typename Tip>
void quick_sort2(Tip *niz, const int p, const int z){
    if(p<z) {
        int j = Particija(niz,p,z);
        quick_sort2(niz, p, j-1);
        quick_sort2(niz, j+1, z);
    }
}

template<typename Tip>
void quick_sort(Tip *niz, int vel){
    int p=0;
    int z=vel-1;
    quick_sort2(niz,p,z);
}


//merge 
template <typename Tip>
void merge(Tip* niz, const int l, const int p, const int q, const int u){
    int i(0), j(q-l), k(l);
    Tip *b = new Tip[u-l+1];
    for(int m = 0; m <= u - l; m++)
        b[m] = niz[l+m];
    while(i <= p - l && j <= u - l){
        if(b[i] < b[j])
            niz[k] = b[i++];
        else
            niz[k] = b[j++];
        k++;
    }
    while(i <= p - l)
        niz[k++] = b[i++];
    while(j <= u - l)
        niz[k++] = b[j++];
    delete [] b;
}


template <typename Tip>
void merge_sort2(Tip *niz, const int l, const int u){
    if(u>l){
        int p = (l+u-1) / 2;
        int q = p+1;
        merge_sort2(niz,l,p);
        merge_sort2(niz, q,u);
        merge(niz,l,p,q,u);
    }
}


template<typename Tip>
void merge_sort(Tip *niz, int vel){
    int l=0;
    int u = vel-1;
    merge_sort2(niz,l,u);
}

void ucitaj(std::string filename, int*& niz, int &vel){
    vel=0;
    std::ifstream ulaz(filename);
    int broj;
    char razmak;
    while(ulaz){
        ulaz>>broj>>razmak;
        vel++;
    }
    ulaz.close();
    niz = new int[vel];
    int i= 0;
    ulaz.open(filename);
    while(ulaz){
        ulaz>>broj>>razmak;
        niz[i++] = broj;
    }
    ulaz.close();
    
}


void generisi(std::string filename, int vel){
    std::ofstream izlaz (filename);
    for(int i=0; i<vel;i++){
        int broj= rand() % 100;
        izlaz<<broj<<" ";
    }
    izlaz.close();
}


void funkcija(){
    int *niz;
    int vel=0;
    clock_t vrijeme_prije = clock();
    generisi("test.txt", 3000);
    ucitaj("test.txt", niz, vel);
    std::cout<<"Odaberite vrstu algoritma za sortiranje: \n";
    std::cout<<"Unesite 1 za Bubble sort\n";
    std::cout<<"Unesite 2 za Selection sort\n";
    std::cout<<"Unesite 3 za Quick sort\n";
    std::cout<<"Unesite 4 za Merge sort\n";
    int n;
    std::cin>>n;
    if(n==1) bubble_sort(niz,vel);
    if(n==2) selection_sort(niz,vel);
    if(n==3) quick_sort(niz,vel);
    if(n==4) merge_sort(niz,vel);
    clock_t vrijeme_nakon=clock();
    int konacno = (vrijeme_nakon-vrijeme_prije)/(CLOCKS_PER_SEC/1000);
    std::cout<<"Ukupno vrijeme sortiranja: "<<konacno<<" milisekundi"<<std::endl;
    
    int trigger=1;
    for(int i=1;i<vel;i++){
        if(niz[i-1]>niz[i]){
            std::cout<<"Niz nije sortiran!\n";
            trigger = 0;
            break;
        }
    }
    if(trigger) std::cout<<"Niz je sortiran!\n";
    std::ofstream nova("nova.txt");
    for(int i=0;i<vel;i++) nova<<niz[i]<<' ';
    nova.close();
}


int main() {
    funkcija();
    return 0;
}
