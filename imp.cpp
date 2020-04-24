#include <iostream>
#include <iterator>
#include <unordered_map>
#include <map>
#include <vector>
#include <algorithm>
#include <iterator>

using namespace std;


template<typename V, typename container = unordered_map<int,V> >
class sparse_matrix
{
public:
	typedef V data;
	
	sparse_matrix() : r_(0), c_(0), mat_(nullptr), nnz_(0) {}

	template<typename X>
	sparse_matrix(X arr, int r, int c, data z) : 
	r_(r), c_(c), z_(z), mat_(new container[r]), nnz_(0)
	{	
		if(arr !=nullptr) {
			for(int i = 0; i < r; ++i)
			{
				for(int j = 0; j < c; ++j)
				{
					if(arr[i][j] != z)
					{
						mat_[i][j] = arr[i][j];
						++nnz_;
					}
				}
			}
		}
	}

	template <typename T>
	sparse_matrix (T inp,int z) :
	z_(z), nnz_(0)
	{
		int r = distance(inp.begin(),inp.end());
		r_ = r;
		mat_ = new container[r];
		typename T::iterator x = inp.begin();
		for(int i = 0;x!=inp.end();++x,++i)
		{
			auto y = (*x).begin();
			for(int j = 0;y!=(*x).end();++y,++j)
			{
				if(*y != z)
				{
					mat_[i][j] = *y;
					++nnz_;
				}
				c_ = j;
			}
		}
	}
	class Iterator : public iterator<forward_iterator_tag, data>
	{
	private:
		int cr_; //current row of iterator
		typename container::iterator curr_;
		const sparse_matrix* sm_;

	public:

		friend sparse_matrix;
		Iterator(const sparse_matrix *sp) : sm_(sp), curr_(sp->mat_[sp->r_-1].end()) {}
		Iterator(const sparse_matrix *sp, int r) : cr_(r), sm_(sp), curr_(sp->mat_[r].begin()) {}
		bool operator==(const Iterator &rhs) const
		{
			return curr_ == rhs.curr_;
		}
		bool operator!=(const Iterator &rhs) const
		{
			return !(*this==rhs);
		}
		data& operator*() const
		{
			return curr_->second;
		}
		Iterator& operator++()
		{
			typename container::iterator temp = curr_;
			
			if(++temp != sm_->mat_[cr_].end())
			{
				++curr_;
				return *this;
			}
			else
			{
				while(temp == sm_->mat_[cr_].end())
				{
					if(cr_ == sm_->r_ -1)
						break;
					temp = sm_->mat_[++cr_].begin();
				}
				curr_ = temp;
				return *this;
			}
		}
		Iterator operator++(int)
		{
			Iterator temp(*this);
            ++*this;
            return temp;
		}

	};

	Iterator begin() const
	{
		int i = 0;
		while(i<r_ && mat_[i].begin() ==  mat_[i].end())
			++i;
		if (i == r_)
			return Iterator(this);
		return Iterator(this,i);
	}

	Iterator end() const
	{
		return Iterator(this);
	}

	class row {
		private:
			sparse_matrix *sp_;
			int mat_row_;
		public:
		row(sparse_matrix *sp,int r) :mat_row_(r),sp_(sp){}
		data operator[](int col_index){
			if(sp_->mat_[mat_row_].find(col_index)!=sp_->mat_[mat_row_].end()) {
				return sp_->mat_[mat_row_][col_index];
			}
			return sp_->z_;
		} 
	};

	row operator[] (int row_index) {
		return row(this,row_index);
	}

	void insert_at(int row, int col, data value) {
		if(row < r_ && col < c_ && value!=z_)
			mat_[row][col] = value;
	}
	
	
	sparse_matrix<data,container>& add(const sparse_matrix<data,container>& rhs){ 
		if(rhs.row_size() == r_ && rhs.col_size()==c_) {
			cout<<"Flag\n";
			data **m = nullptr;
			sparse_matrix<data,container> *result = new sparse_matrix<data,container>(m,r_,c_,z_);
			for(int i = 0; i < r_; ++i)
			{
				auto j = mat_[i].begin();
				auto j_rhs = rhs.mat_[i].begin();
				while(j!=mat_[i].end()){
					result->insert_at(i, j->first, j->second + rhs.mat_[i][j->first]);	
					++j;
				}
				while(j_rhs!=rhs.mat_[i].end()){
					result->insert_at(i, j_rhs->first, j_rhs->second + mat_[i][j_rhs->first]);	
					++j_rhs;
				}
			}
			return *result;
		}
		return *(new sparse_matrix<data,container>());
	}

	bool is_empty () {
		return (mat_ == nullptr);
	}

	int row_size() const
	{
		return r_;
	}
	int col_size() const
	{
		return c_;
	}

	void disp() const
	{
		for(int i = 0; i < r_; ++i)
		{
			auto j = mat_[i].begin();
			while(j!=mat_[i].end())
			{
				cout<<"("<<i<<","<<j->first<<") : "<<j->second<<"\n";
				++j;
			}
		}
	}

	Iterator& find(data ele) {
		for(int i = 0; i < r_; ++i)
		{
			auto j = mat_[i].begin();
			while(j!=mat_[i].end())
			{
				if(j->second == ele) {  
					Iterator *ret = new Iterator(this,i);
					ret->curr_=j;
					return *ret;
				}
				++j;
			}
		}
		Iterator *ret = new Iterator(this->end());
		return *ret;
	}

	void transpose(){
		container * new_mat = new container[c_];
		for(int i = 0; i < r_; ++i)
		{
			auto j = mat_[i].begin();
			while(j!=mat_[i].end())
			{
				new_mat[j->first][i] = j->second;
				++j;
			}
		}
		int temp = c_;
		c_ = r_;
		r_ = temp;
		delete[] mat_;
		mat_ = new_mat;
	}

private:
	int r_;
	int c_;
	data z_;
	int nnz_;
	container* mat_;

};

int main()
{
	cout<<"vector\n";
	vector<vector<int>> twod;
	twod.push_back({0,0,3,0});
	twod.push_back({0,0,0,2});
	twod.push_back({1,0,0,0});
	twod.push_back({0,4,0,0});
	sparse_matrix <int,unordered_map<int,int>> sp(twod,0);
	cout<<"print non-zeros\n";
	sp.disp();
	cout<<endl;
	auto it = sp.begin();
	cout<<"== operator\n";
	auto it2 = sp.begin();
	cout<<boolalpha<<(it == it2);
	cout<<"\npost inc " << *it2++ <<"\n";
	*it2 = 7;
	cout<<"\nsequential iteration of non zeros \n";
	while(it != sp.end())
	{
		cout<<*it<<endl;
		++it;
	}
	cout<<"Find : \n";
	cout<<*(find(sp.begin(),sp.end(),3))<<endl;
	cout<<"---------\n";


	int m[2][2];
	for(int i=0;i<4;++i)
		for(int j=0;j<4;++j)
			m[i][j] = 0;
	m[0][2] = 1;
	m[1][1] = 2;
	sparse_matrix <int,map<int,int>> sp2(m,2,2,0);
	sp2.disp();

	cout<<"\n2D array\n";
	int **arr1;

    arr1 = new int *[2];

    for (int i =0; i < 2;i++) {

        arr1[i] = new int[2];

        for(int j=0;j<2;j++) {

            arr1[i][j]=0;

        }

    }	
    arr1[1][1]=6;
    sparse_matrix<int,map<int,int>> obj3(arr1,2,2,0);    
	obj3.insert_at(1,1,1);
	obj3.insert_at(1,0,6);
	cout<<"Testing [1][1] -> "<<obj3[1][1]<<endl;
	obj3.disp();
	cout<<"Addition : \n";
	sparse_matrix<int,map<int,int>> res = obj3.add(sp2);
	res.disp();
	res.transpose();
	cout<<"Transpose Result : "<<endl;
	res.disp();
	for(int i=0;i<2;i++){
		for(int j=0;j<2;j++) {
			cout<<res[i][j]<<" "; 
		}
		cout<<endl;
	}
	cout<<"Find : \n";
	auto iter = res.find(7);
	*iter= 8;
	res.disp(); 
}