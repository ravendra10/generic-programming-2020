#include <algorithm> 
#include <chrono> 
#include <iostream> 
#include <algorithm>
#include <vector>
#include <iterator>
#include <list>
using namespace std; 
using namespace std::chrono; 
#include "sparse_matrix.h"
#define R 4
#define C 4
#define NE 10
#define TOG 0
#define DIAx 0

int main()
{
    cout << "\033[1;36m\n2-D array to sparse matrix \033[0m\n";//yellow text
    int a[3][3];
    int b[3][3];
    for(int i = 0; i<3;++i)
    {
        for(int j = 0; j < 3; ++j)
        {
            a[i][j] = 0;
            b[i][j] = 0;
        }
    }
    a[0][1] = 1;
    a[1][1] = 2;
    a[1][2] = 3;
    a[2][2] = 4;

    b[0][0] = 1;
    b[0][2] = 1;
    b[2][1] = 1;
    b[2][0] = 2;
    b[1][1] = 3;

    sparse_matrix<int,map<int,int>> A(a,3,3,0);
    A.name_ = "A";
    A.disp();
    sparse_matrix<int,map<int,int>> B(b,3,3,0);
    B.name_ = "B";
    B.disp();

    cout << "\033[1;36m\nvector of vector to sparse_matrix \033[0m\n";
    
    vector<vector<int>> twod;
    twod.push_back({0,0,3,0});
    twod.push_back({0,0,0,2});
    twod.push_back({1,0,0,0});
    twod.push_back({0,4,0,0});
    sparse_matrix <int,map<int,int>> V(twod,0);
    V.name_ = "V";
    V.disp();
    
    cout << "\033[1;36m\nlist of vector to sparse_matrix \033[0m\n";
    
    list <vector<int>> L;
    L.push_back({0,0,1,0});
    L.push_back({0,0,0,0});
    L.push_back({3,0,0,4});
    sparse_matrix<int,map<int,int>> P(L,0);
    P.name_ = "P";
    P.disp();

    cout << "\033[1;36m\nlinear structure to row major sparse_matrix \033[0m\n";

    vector<int> q({0,0,1,0,2,0,0,0,3,0,0,0,0,4,0});
    sparse_matrix<int,map<int,int>> Q(q.begin(),q.end(),5,3,0);
    Q.name_ = "Q";
    Q.disp();

    /*cout<<"\nlinear structure to col major sparse_matrix\n";
    sparse_matrix<int,map<int,int>> Q(q.begin(),q.end(),5,3,0,0);
    Q2.name_ = "Q2";
    Q2.disp();*/

    if(can_add(A,B))
    {
        sparse_matrix<int,map<int,int>> D(A.row_size(),A.col_size(),0);
        add(A,B,D);
        D.name_ = "A+B";
        D.disp();
    }

    sparse_matrix<int,map<int,int>> Bt = B.transpose();
    Bt.name_ = "B transpose";
    Bt.disp();

    if(true)
    {
        sparse_matrix<int,map<int,int>> Cm(A.row_size(),B.col_size(),0);
        multiply(A,B,Cm);
        Cm.name_ = "A*B";
        Cm.disp();
    }

    int i,j,k;

    #if TOG
    int Ax[R][C];
    int Bx[R][C];
    int res[R][C];

    

    for(i = 0; i < R; ++i)
    {
        for(j = 0; j < C; ++j)
        {
            Ax[i][j] = 0;
            Bx[i][j] = 0;
            res[i][j] = 0;
        }
    }
    #if DIAx
    for(int i = 0; i<R; ++i)
    {
        Ax[i][i] = i;
        Bx[i][i] = i;
    }
    #endif

    #if !(DIAx)
    int nnz = 0;
    while(nnz<NE)
    {
        i = rand()%R;
        j = rand()%C;

        if(Ax[i][j] == 0)
        {
            Ax[i][j] = 1 + (rand()%10);
            ++nnz;
        }
    }

    nnz = 0;
    while(nnz<NE)
    {
        i = rand()%R;
        j = rand()%C;

        if(Bx[i][j] == 0)
        {
            Bx[i][j] = 1 + (rand()%10);
            ++nnz;
        }
    }
    #endif
    sparse_matrix<int,void> Axsp(Ax,R,C,0);

    sparse_matrix<int,void> Bxsp(Bx,R,C,0);

    #endif
    
    #if !(TOG)
    vector <vector<int>> Ax(R);
    vector <vector<int>> Bx(R);
    vector <vector<int>> res(R);
    for(int i = 0; i<R; ++i)
    {
        Ax[i] = vector<int> (C);
         for (int j = 0; j < C; j++) 
            Ax[i][j] = 0; 
        
    }

    for(int i = 0; i<R; ++i)
    {
        Bx[i] = vector<int> (C);
         for (int j = 0; j < C; j++) 
            Bx[i][j] = 0; 
    }

    for(int i = 0; i<R; ++i)
    {
        res[i] = vector<int> (C);
         for (int j = 0; j < C; j++) 
            res[i][j] = 0; 
    }

    #if DIAx
    for(int i = 0; i<R; ++i)
    {
        Ax[i][i] = i;
        Bx[i][i] = i;
    }
    #endif
    
    #if !(DIAx)
    int nnz = 0;
    while(nnz<NE)
    {
        i = rand()%R;
        j = rand()%C;

        if(Ax[i][j] == 0)
        {
            Ax[i][j] = 1 + (rand()%10);
            ++nnz;
        }
    }

    nnz = 0;
    while(nnz<NE)
    {
        i = rand()%R;
        j = rand()%C;

        if(Bx[i][j] == 0)
        {
            Bx[i][j] = 1 + (rand()%10);
            ++nnz;
        }
    }
    #endif

    sparse_matrix<int,void> Axsp(Ax,0);
    //cout<<"Ax r_ : "<<Axsp.row_size()<<" c_ : "<<Axsp.row_size()<< "nnz_"<<Axsp.count_non_zeros()<<endl;
    sparse_matrix<int,void> Bxsp(Bx,0);

    #endif
    
    auto start = high_resolution_clock::now();
     
    //general matrix multiplication algo
            
    for(i = 0; i < R; ++i)
    {
        for(j = 0; j < C; ++j)
        {
            for(k = 0; k < C; ++k)
            {
                res[i][j] += Ax[i][k] * Bx[k][j];
            }
        }
    }

    auto stop = high_resolution_clock::now(); 
    auto duration = duration_cast<microseconds>(stop - start); 
    int count = 0;
    cout << "Time taken by 2D array: "
         << duration.count()/1000 << " milliseconds" << endl; 
    for(int i = 0; i < R; ++i)
        for(int j = 0; j < C; ++j)
        {
            if(res[i][j] != 0) 
                ++count;
        }
    cout << "non zeros in res : "<<count<<endl;
    sparse_matrix<int,void> Res(R,C,0);

    auto Bxt = Bxsp.transpose();
    
    //cout<<"Bx transpose\n";
    //Bxt.disp();

    start = high_resolution_clock::now(); 
    
    multiply(Axsp,Bxt,Res);

    stop = high_resolution_clock::now(); 

    cout<< "non zeros in res : "<<Res.count_non_zeros()<<endl;
    duration = duration_cast<microseconds>(stop - start); 
  
    cout << "Time taken by sparse_matrix container: "
         << duration.count()/1000 << " milliseconds" << endl;

    

    start = high_resolution_clock::now();
     
    //general matrix multiplication algo
            
    for(i = 0; i < R; ++i)
    {
        for(j = 0; j < C; ++j)
        {
            res[i][j] = Ax[i][j] + Bx[i][j];
        }
    }

    stop = high_resolution_clock::now(); 
    duration = duration_cast<microseconds>(stop - start); 
    count = 0;
    cout << "Axddition 2D array: "
         << duration.count() << " microseconds" << endl; 
    for(int i = 0; i < R; ++i)
        for(int j = 0; j < C; ++j)
        {
            if(res[i][j] != 0) 
                ++count;
        }
    cout << "non zeros in res : "<<count<<endl;
    sparse_matrix<int,void> Resadd(R,C,0);

    //auto Bxt = Bxsp.transpose();

    //cout<<"Bx transpose\n";
    //Bxt.disp();

    start = high_resolution_clock::now(); 
    
    add(Axsp,Bxsp,Resadd);

    stop = high_resolution_clock::now(); 

    cout<< "non zeros in res : "<<Resadd.count_non_zeros()<<endl;
    duration = duration_cast<microseconds>(stop - start); 
  
    cout << "Axddition sparse_matrix container: "
         << duration.count()<< " microseconds" << endl;

    

    
    cout <<"Ax\n";
    for(i = 0; i < R; ++i)
    {
        for(j = 0; j < C; ++j)
        {
            cout <<Ax[i][j]<<"\t";
        }
        cout<<endl;
    }

    cout <<"Bx\n";
    for(i = 0; i < R; ++i)
    {
        for(j = 0; j < C; ++j)
        {
            cout <<Bx[i][j]<<"\t";
        }
        cout<<endl;
    }

    cout <<"res\n";
    for(i = 0; i < R; ++i)
    {
        for(j = 0; j < C; ++j)
        {
            cout <<res[i][j]<<"\t";
        }
        cout<<endl;
    }

    Axsp.disp();
    Bxsp.disp();
    Res.disp();


    cout<<"iterator test : \n";
    auto it = Res.begin();
    while(it != Res.end())
    {
        cout<<*it<<"  ";
        ++it;
    }

    cout<<"\nequality :"<<(it == Res.end())<<endl;
    
     //Find
    int ele=73;
    auto iter = find(Res.begin(),Res.end(),ele);
    cout<<"Find element "<<ele<<" : ";
    if (iter!=Res.end()) cout<<"Found"<<endl;
    else cout<<"Not Found"<<endl;
    ele=3;
    iter = find(Res.begin(),Res.end(),ele);
    cout<<"Find element "<<ele<<" : ";
    if (iter!=Res.end()) cout<<"Found"<<endl;
    else cout<<"Not Found"<<endl;
    cout<<"----------------------"<<endl;
    //Insertion
    int row = 0,col = 1;
    ele = 8;
    cout<<"Insert Element (B Transpose)"<<ele<<" at ("<<row<<","<<col<<")\n";
    Bt.insert_at(row,col,ele);
    Bt.disp();
    row = 0,col = 1;
    ele = 6;
    cout<<"Insert Element "<<ele<<" at ("<<row<<","<<col<<")\n";
    Bt.insert_at(row,col,ele);
    Bt.disp();
    
    //Indexing operator
    cout<<"Display Matrix using indexing operator : \n";
    for( int i=0;i<Bt.row_size();i++) {
        for(int j =0;j<Bt.col_size();j++){
            cout<<Bt[i][j]<<" ";
        }
        cout<<endl;
    }

    //L-value dereference iterator
    cout<<"Iterator L-value dereference-> replace first element: \n";
    auto s=Bt.begin();
    *s = 0;
    cout<<*s<<endl;
    Bt.disp();
    

     //delete index
    cout<<"Delete index : (1,2)"<<endl;
    Bt.delete_index(1,2);
    Bt.disp();

    //Compress
    cout<<"Compress Matrix:"<<endl;
    Bt.compress();
    Bt.disp();
    


    

}