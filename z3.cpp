#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int getMax(vector<int> &v)
{
    int n = v.size();
    int pov = v[0];
    for(int i = 1; i < n; i++)
        if(v[i]>pov)
            pov = v[i];
    return pov;
}

void countSort(vector<int> &v, int exp)
{
    int n = v.size();
    int *output = new int[n];
    int count[10] = {0};
    for(int i = 0; i < n; i++)
        count[(v[i]/exp)%10]++;
    for(int i = 1; i < 10; i++)
        count[i] += count[i-1];
    for(int i = n-1; i >= 0; i--) {
        output[count[(v[i]/exp)%10]-1] = v[i];
        count[(v[i]/exp)%10]--;
    }
    for(int i = 0; i < n; i++)
        v[i] = output[i];
    delete[] output;
}

void radixSort(vector<int> &v){
    int s = getMax(v);
    for(int exp = 1; s/exp>0;exp*=10)
    countSort(v,exp);
}

void popraviDolje(int i, vector<int> &v, int vel)
{
    while(!(i >= vel/2) && (i < vel)) {
        int veci = 2*i+1;
        int dd = 2*i+2;
        if((dd<vel) && v[dd] > v[veci])
            veci = dd;
        if(v[i] > v[veci])
            return;
        int tmp = v[i];
        v[i] = v[veci];
        v[veci] = tmp;
        i = veci;
    }
}

void stvoriGomilu(vector<int> &v)
{
    int vel = v.size();
    for(int i=vel/2; i>=0; i--)
        popraviDolje(i,v,v.size());
}


void popraviGore(int i, vector<int> &v)
{
    int t,r;
    while((i!=0)&&(v[i]>v[(i-1)/2])) {
        r=(i-1)/2;
        t=v[i];
        v[i]=v[r];
        v[r]=t;
        i=r;
    }
}

void umetniUGomilu(vector<int> &v, int umetnuti, int &vel)
{
    int kap=v.size()-1;
    if(vel>kap)
        v.resize(v.size()+1);
    v[vel++] = umetnuti;
    popraviGore(vel-1,v);
}

int izbaciPrvi(vector<int> &v, int &vel)
{
    if(vel==0) throw "Prazna gomila!\n";
    int tmp = v[0];
    v[0] = v[--vel];
    v[vel]=tmp;
    if(vel!=0)
        popraviDolje(0,v,vel);
    return v[vel];
}

void gomilaSort(vector<int> &v)
{
    stvoriGomilu(v);
    int br=1;
    int vel =v.size();
    for(int i=vel-1; i>0; i--) {
        int tmp=v[0];
        v[0]=v[i];
        v[i]=tmp;
        int velicina = v.size()-br++;
        popraviDolje(0,v,velicina);
    }
}
int main()
{
    vector<int> v;
    int velicina = 0;
    umetniUGomilu(v,546,velicina);
    umetniUGomilu(v,4354,velicina);
    umetniUGomilu(v,12,velicina);
    umetniUGomilu(v,506,velicina);
    umetniUGomilu(v,675,velicina);
    umetniUGomilu(v,33,velicina);
    umetniUGomilu(v,55,velicina);
    umetniUGomilu(v,1,velicina);
    for(int i(0);i<v.size();i++) cout<<v[i]<<" ";
    cout<<endl;
    radixSort(v);
    for(int i=0;i<v.size();i++) cout<<v[i]<<" ";
    
    return 0;
}
