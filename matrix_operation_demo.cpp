#include <algorithm> 
#include <chrono> 
#include <iostream> 
#include <algorithm>
#include <vector>
using namespace std; 
using namespace std::chrono; 
#include "sparse_matrix.h"
//#include "sparse_matrix.h"
#define R 100
#define C 100
#define NE 1200
#define TOG 1
#define DIAx 0

int main()
{
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
  /*  cout << "2D array : "
         << duration.count()/1000 << " milliseconds" << endl; 
    */for(int i = 0; i < R; ++i)
		for(int j = 0; j < C; ++j)
		{
			if(res[i][j] != 0) 
				++count;
		}
	cout << "non zeros in array : "<<count<<endl;
    sparse_matrix<int,void> Res(R,C,0);

    auto Bxt = Bxsp.transpose();
    
    //cout<<"Bx transpose\n";
    //Bxt.disp();

    start = high_resolution_clock::now(); 
	
	int op = multiply(Axsp,Bxt,Res);

    stop = high_resolution_clock::now(); 

    cout<< "non zeros in matrix : "<<Res.count_non_zeros()<<endl;
    duration = duration_cast<microseconds>(stop - start); 
  
/*    cout << "Time taken by sparse_matrix container: "
         << duration.count()/1000 << " milliseconds" << endl;
*/
    cout<<"OPERATIONS : "<<op<<"\n";

    cout<<endl;

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
   /* cout << "Axddition 2D array: "
         << duration.count() << " microseconds" << endl; 
    */for(int i = 0; i < R; ++i)
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
	
	int opa = add(Axsp,Bxsp,Resadd);

    stop = high_resolution_clock::now(); 

    cout<< "non zeros in res : "<<Resadd.count_non_zeros()<<endl;
    duration = duration_cast<microseconds>(stop - start); 
  
    cout << "Addition sparse_matrix container: ";
        // << duration.count()<< " microseconds" << endl;
		  cout<<"\nOPERATIONS : "<<opa<<"\n";
    

    
   /* cout <<"Ax\n";
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
	Res.disp();*/
    




}