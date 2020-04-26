#include <iostream>
#include <iterator>
#include <unordered_map>
#include <map>
#include <vector>
#include <algorithm>
#include <iterator>
#include <string>

using namespace std;

template<typename V, typename container = unordered_map<int,V> >
class sparse_matrix
{
public:
    typedef V data;
    
    string name_;

    sparse_matrix() : r_(0), c_(0), mat_(nullptr), nnz_(0) {} // null object

    sparse_matrix(int r, int c, data z) : 
    r_(r), c_(c), z_(z), mat_(new container[r]), nnz_(0) {} // 2d matrix with only zeros
    
    template<typename X>                                    // 2d array 
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
    template <typename T>                                   //list/vector/deque etc
    sparse_matrix (T inp,data z) :
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
    template<typename ptr_t>//row major
    sparse_matrix(ptr_t start, ptr_t last, data z, int r, int c) : z_(z), r_(r), c_(c), nnz_(0)
    {
        int i=0;
        mat_ = new container[r];
        for(;i<r;++i)
        {
            int j = 0;
            for(;j<c;++j,++start)
            {
                if(*start != z)
                    mat_[i][j] = *start;
            }
        }
    }

    template<typename ptr_t>//col major
    sparse_matrix(ptr_t start, ptr_t last, data z, int r, int c, int) : z_(z), r_(r), c_(c), nnz_(0)
    {
        int i=0;
        mat_ = new container[r];
        for(;i<c;++i)
        {
            int j = 0;
            for(;j<r;++j,++start)
            {
                if(*start != z)
                    mat_[j][i] = *start;
            }
        }
    }
    sparse_matrix(const sparse_matrix& old):                        //copy constructor
    nnz_(old.nnz_) , r_(old.r_) , c_(old.c_), mat_(new container[old.r_])
    {
        for(int i = 0; i<r_; ++i)
        {
            auto x = old.mat_[i].begin();

            while(x!=old.mat_[i].end())
            {
                mat_[i][x->first] = x->second;
                ++x;
            }
        }
    }

    sparse_matrix& operator=(const sparse_matrix& rhs)
    {
        if(this != &rhs)
        {
            delete [] mat_;
            r_ = rhs.r_;
            c_ = rhs.c_;
            for(int i = 0; i<r_; ++i)
            {
                auto x = rhs.mat_[i].begin();

                while(x!=rhs.mat_[i].end())
                {
                    mat_[i][x->first] = x->second;
                    ++x;
                }
            }
        }
        return *this;
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

    void insert_at(int row, int col, data value) 
    {
        if(row < r_ && col < c_ && value!=z_ )
        {
            if(mat_[row][col] == data())
                ++nnz_;
            mat_[row][col] = value;
        }

    }
    void delete_index(int row,int col) {
        if(row<r_ && col < c_) mat_[row].erase(col);
    }

    void compress(){
        
        for(int i = 0; i < r_; ++i)
        {
            vector<int> zeroes;
            auto j = mat_[i].begin();
            while(j!=mat_[i].end())
            {
                if(j->second == z_) zeroes.push_back(j->first);
                ++j;
            }
            for(int col : zeroes) {
                (*this).delete_index(i,col);
            }
        }
    }
    friend sparse_matrix<data,map<int,data>> addition(sparse_matrix<data,map<int,data>>& A,sparse_matrix<data,map<int,data>>& B)
    {
        sparse_matrix<data,map<int,data>> R(A.r_,B.c_,B.z_);

        for(int i = 0; i < A.r_; ++i)
            {
                auto j = A.mat_[i].begin();
                auto j_rhs = B.mat_[i].begin();
                while(j!=A.mat_[i].end()){
                    R.insert_at(i, j->first, j->second + B[i][j->first]);   
                    ++j;
                }
                while(j_rhs!=B.mat_[i].end()){
                    R.insert_at(i, j_rhs->first, j_rhs->second + A[i][j_rhs->first]);   
                    ++j_rhs;
                }
            }
       return R;
   }

   friend sparse_matrix<data,map<int,data>> multiplication(sparse_matrix<data,map<int,data>>& A,sparse_matrix<data,map<int,data>>& B)
   {
    sparse_matrix<data,map<int,data>> R(A.r_,B.c_,data());
    sparse_matrix<data,map<int,data>> Bt = B.transpose();

    for(int i = 0; i < A.r_; ++i)
    {
        for(int j = 0; j < B.r_;++j)
        {
            auto ptr1 = A.mat_[i].begin();
            auto ptr2 = Bt.mat_[j].begin();
            data res = data();
            while(ptr1 != A.mat_[i].end() && ptr2 != Bt.mat_[j].end())
            {
                if(ptr1->first < ptr2->first)
                    ++ptr1;
                else if(ptr1->first > ptr2->first)
                    ++ptr2;
                else
                {
                    res += (ptr1->second)*(ptr2->second);
                    ++ptr1;
                    ++ptr2;
                }
            }
            if(res != data())
                R.insert_at(i,j,res);
        }
    }
    return R;
}

bool is_empty () {
    return (nnz_ == 0);
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
    cout<<"mat "<<name_<<"\n";
    for(int i = 0; i < r_; ++i)
    {
        auto j = mat_[i].begin();
        while(j!=mat_[i].end())
        {
            cout<<"("<<i<<","<<j->first<<") : "<<j->second<<"\n";
            ++j;
        }
    }
    cout<<"---------------------\n";
}

sparse_matrix transpose()
{
    sparse_matrix<data,map<int,data>> temp(this->c_, this->r_, this->z_); 
    for(int i = 0; i < r_; ++i)
    {
        auto j = mat_[i].begin();
        while(j!=mat_[i].end())
        {
            temp.mat_[j->first][i] = j->second;
            ++j;
        }
    }
    temp.nnz_ = this->nnz_;
    return temp;
}
    template<typename d1, typename c1,typename d2, typename c2>
friend bool can_multiply(sparse_matrix<d1,c1>& A, sparse_matrix<d2,c2>& B)
{
    return (std::is_same<d1,d2>::value && std::is_same<c1,map<int,d1>>::value && std::is_same<c2,c1>::value && A.c_ == B.r_);
}
    template<typename d1, typename c1,typename d2, typename c2>
friend bool can_add(sparse_matrix<d1,c1>& A, sparse_matrix<d2,c2>& B)
{
    return (std::is_same<d1,d2>::value && std::is_same<c1,map<int,d1>>::value && std::is_same<c2,c1>::value && A.r_ == B.r_ && B.c_ == A.c_);
}

private:
    int r_;
    int c_;
    data z_;
    int nnz_;
    container* mat_;

};