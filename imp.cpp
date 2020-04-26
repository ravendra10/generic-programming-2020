#include<iostream>
#include<algorithm>
#include<list>
#include "sparse_matrix.h"

int main()
{
    cout<<"\n2-D array to sparse matrix\n";
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

    cout<<"\nvector of vector to sparse_matrix\n";
    vector<vector<int>> twod;
    twod.push_back({0,0,3,0});
    twod.push_back({0,0,0,2});
    twod.push_back({1,0,0,0});
    twod.push_back({0,4,0,0});
    sparse_matrix <int,map<int,int>> V(twod,0);
    V.disp();
    
    cout<<"\nlist of vector to sparse_matrix\n";
    list <vector<int>> L1;
    L1.push_back({0,0,1,0});
    L1.push_back({0,0,0,0});
    L1.push_back({3,0,0,4});
    sparse_matrix<int,map<int,int>> P1(L1,0);
    P1.name_ = "P1";
    P1.disp();

    cout<<"\nlinear structure to sparse_matrix\n";
    list <vector<int>> L2;
    L2.push_back({0,0,1,0});
    L2.push_back({0,0,0,0});
    L2.push_back({3,0,0,4});
    sparse_matrix<int,map<int,int>> P2(L2,0);
    P2.name_ = "P2";
    P2.disp();


    if(can_multiply(A,B))
    {
        sparse_matrix<int,map<int,int>> C = multiplication(A,B);
        C.name_ = "A*B";
        C.disp();
    }
    if(can_add(A,B))
    {
        sparse_matrix<int,map<int,int>> D = addition(A,B);
        D.name_ = "A+B";
        D.disp();
    }
    B.disp();
    sparse_matrix<int,map<int,int>> Bt = B.transpose();
    Bt.name_ = "B transpose";
    Bt.disp();
    
    //Find
    int ele=34;
    auto iter = find(Bt.begin(),Bt.end(),ele);
    cout<<"Find element "<<ele<<" in matrix B transpose : ";
    if (iter!=Bt.end()) cout<<"Found"<<endl;
    else cout<<"Not Found"<<endl;
    ele=3;
    iter = find(Bt.begin(),Bt.end(),ele);
    cout<<"Find element "<<ele<<" in matrix B transpose : ";
    if (iter!=Bt.end()) cout<<"Found"<<endl;
    else cout<<"Not Found"<<endl;
    
    //Insert Element, replace if already exists
    int row = 0,col = 1;
    ele = 8;
    cout<<"Insert Element "<<ele<<" at ("<<row<<","<<col<<")\n";
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
    //Iterator
    cout<<"Display non zero elements using iterator : \n";
    auto start = Bt.begin();
    auto end = Bt.end();
    while (start!=end){
        cout<<*start<<endl;
        start++;
    }

    //L-value dereference iterator
    cout<<"Iterator L-value dereference-> replace first element: \n";
    start = Bt.begin();
    *start = 0;
    cout<<*start<<endl;
    
    //delete index
    cout<<"Delete index : (1,1)"<<endl;
    Bt.delete_index(1,1);
    Bt.disp();

    //Compress
    cout<<"Compress Matrix:"<<endl;
    Bt.compress();
    Bt.disp();
    
}