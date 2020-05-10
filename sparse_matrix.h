#ifndef __X_H_INCLUDED__   
#define __X_H_INCLUDED__
#include <iostream>
#include <iterator>
#include <unordered_map>
#include <map>
#include <vector>
#include <algorithm>
#include <iterator>
#include <string>

using namespace std;


template<typename V,typename containerType>
class sparse_matrix
{
public:
    typedef V data;
    typedef containerType container;
    
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
    sparse_matrix(ptr_t start, ptr_t last, int r, int c, data z) : z_(z), r_(r), c_(c), nnz_(0)
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
    sparse_matrix(ptr_t start, ptr_t last, int r, int c, data z, int) : z_(z), r_(r), c_(c), nnz_(0)
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
        auto begin()
        {
            return sp_->mat_[mat_row_].begin();
        }
        auto end()
        {
            return sp_->mat_[mat_row_].end();
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
    template <typename d,typename c1,typename c2, typename c3>
    friend void add(sparse_matrix<d,c1>& A,sparse_matrix<data,c2>& B, sparse_matrix<data,c3>& R)
    {
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
   }
   template<typename d>
   friend void multiply(sparse_matrix<d,map<int,d>>& A,sparse_matrix<d,map<int,d>>& Bt, sparse_matrix<d,map<int,d>>& R)
   {
    //auto Bt = B.transpose();
    for(int i = 0; i < A.r_; ++i)
    {
        auto a_start = A.mat_[i].begin();
        auto a_end = A.mat_[i].end();
        for(int j = 0; j < Bt.r_;++j)
        {
            auto ptr1 = a_start;
            auto ptr2 = Bt.mat_[j].begin();
            data res = data();
            auto b_end = Bt.mat_[j].end();
            while(ptr1 != a_end && ptr2 != b_end)
            {
                if(ptr1->first < ptr2->first)
                    ++ptr1;
                else if(ptr1->first > ptr2->first)
                    ++ptr2;
                else
                {
                    //cout<<"at A : "<<i<<","<<(ptr1->first)<<" and B : "<<j<<","<<(ptr1->first)<<"\n";
                    res += (ptr1->second)*(ptr2->second);
                    
                    ++ptr1;
                    ++ptr2;
                }
            }
            if(res != data())
            {
                R.mat_[i][j] = res;
                ++R.nnz_;
            }
        }
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
int count_non_zeros()
{
    return nnz_;
}
void disp() const
{
    cout<<"\033[1;32m"<<"key-value type mat "<<name_<<"\033[0m\n";;
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
    sparse_matrix<data,container> temp(this->c_, this->r_, this->z_); 
    for(int i = 0; i < r_; ++i)
    {
        auto j = mat_[i].begin();
        auto j_end = mat_[i].end();
        while(j != j_end)
        {
            temp.mat_[j->first][i] = j->second;
            ++j;
        }
    }
    temp.nnz_ = this->nnz_;
    return temp;
}
template <typename d1,typename c1,typename d2, typename c2>
friend bool can_multiply(sparse_matrix<d1,c1>& A, sparse_matrix<d2,c2>& B)
{
    return (std::is_same<d1,d2>::value && std::is_same<c1,map<int,d1>>::value && std::is_same<c2,map<int,d1>>::value && A.c_ == B.r_);
}
template <typename d1,typename c1,typename d2, typename c2>
friend bool can_add(sparse_matrix<d1,c1>& A, sparse_matrix<d2,c2>& B)
{
    return (std::is_same<d1,d2>::value && std::is_same<c1,map<int,d1>>::value && std::is_same<c2,map<int,d1>>::value && A.c_ == B.c_ && A.r_ == B.c_);
}

private:
    int r_;
    int c_;
    data z_;
    int nnz_;
    container* mat_;
};



































template<typename T>
class sparse_matrix<T,void>
{
public:

    typedef T data;
    string name_;

    sparse_matrix(int r, int c, data z) : r_(r), c_(c), nnz_(0), z_(z) {}

    template<typename X>                                    // 2d array 
    sparse_matrix(X arr, int r, int c, data z) : 
    r_(r), c_(c), z_(z), nnz_(0)
    {   
        if(arr != nullptr) {
            for(int i = 0; i < r; ++i)
            {
                for(int j = 0; j < c; ++j)
                {
                    if(arr[i][j] != z)
                    {
                        insert_seq(i,j,arr[i][j]);
                    }
                }
            }
        }
    }
    template <typename U>                                   //list/vector/deque etc
    sparse_matrix (U inp,data z) :
    z_(z), nnz_(0)
    {
        int r = distance(inp.begin(),inp.end());
        r_ = r;
        typename U::iterator x = inp.begin();
        for(int i = 0;x!=inp.end();++x,++i)
        {
            auto y = (*x).begin();
            for(int j = 0;y!=(*x).end();++y,++j)
            {
                if(*y != z)
                {
                    insert_seq(i,j,*y);
                }
                c_ = j;
            }
        }
    }
    template<typename ptr_t>//row major
    sparse_matrix(ptr_t start, ptr_t last, int r, int c, data z) : z_(z), r_(r), c_(c), nnz_(0)
    {
        int i=0;
        for(;i<r;++i)
        {
            int j = 0;
            for(;j<c;++j,++start)
            {
                if(*start != z)
                {
                    insert_seq(i,j,*start);
                    
                }
            }
        }
    }

    class Iterator : public iterator<forward_iterator_tag, data>
    {
    private:
        typename vector<T>::iterator curr_;
        

    public:
        Iterator(typename vector<T>::iterator it) :curr_(it) {}
        
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
            return *curr_;
        }
        Iterator& operator++()
        {
            ++curr_;
             return *this;

        }
        Iterator operator++(int)
        {
            Iterator temp(*this);
            ++*this;
            return temp;
        }

    };

    Iterator begin() 
    {
        return Iterator(val_.begin());
    }

    Iterator end() 
    {
        return Iterator(val_.end());
    }

    sparse_matrix transpose() 
    { 
        int col = c_;
        int row = r_;
        int len = nnz_;
        sparse_matrix result(col, row, z_); 

        //cout<<"new mat created\n";

        result.nnz_ = len; 

        result.row_ = vector<int> (len);
        for (int j = 0; j < len; j++) 
            result.row_[j] = 0; 

        result.col_ = vector<int> (len);
        for (int j = 0; j < len; j++) 
            result.col_[j] = 0; 

        result.val_ = vector<data> (len);
        for (int j = 0; j < len; j++) 
            result.val_[j] = z_; 

        //cout<<"memory allocated\n";



        int *count = new int[col + 1]; 

        count[0] = 0;

        for (int i = 1; i <= col; i++) 
            count[i] = 0; 

        for (int i = 0; i < len; i++) 
            count[col_[i]]++; 

        int *index = new int[col + 1]; 

        index[0] = 0; 

        for (int i = 1; i <= col; i++) 

            index[i] = index[i - 1] + count[i - 1]; 


        for (int i = 0; i < len; i++) 
        { 

            int rpos = index[col_[i]]++; 
            result.row_[rpos] = col_[i];
            result.col_[rpos] = row_[i]; 
            result.val_[rpos] = val_[i]; 
        } 
        return result; 

    } 

    friend int add(sparse_matrix& A, sparse_matrix& B, sparse_matrix& C)
    {
        int i = 0,j = 0;int op = 0;
        while(i < A.nnz_ && j < B.nnz_)
        {
            if( A.row_[i] > B.row_[j] || A.row_[i] == B.row_[j] && A.col_[i] > B.col_[j])
            {
                C.insert_seq(B.row_[j], B.col_[j], B.val_[j]);
                ++op;
                //cout<<B.row_[j]<<" "<<B.col_[j]<<" "<< B.val_[j]<<endl;
                ++j;
            }
            else if( A.row_[i] < B.row_[j] || A.row_[i] == B.row_[j] && A.col_[i] < B.col_[j])
            {
                C.insert_seq(A.row_[i], A.col_[i], A.val_[i]);
                ++op;
                ++i;
            }
            else
            {
                data x = A.val_[i]+B.val_[j];

                if(x != A.nnz_)
                {
                    C.insert_seq(B.row_[j], B.col_[j], x);
                    ++op;
                }
                ++j;
                ++i;
                

            }
        }

        while(i < A.nnz_)
        {
            C.insert_seq(A.row_[i], A.col_[i], A.val_[i]);
            ++i;++op;
        }

        while(j < B.nnz_)
        {
            C.insert_seq(B.row_[j], B.col_[j], B.val_[j]);
            ++j;++op;
        }
        return op;
    }

    friend int multiply(sparse_matrix& A, sparse_matrix& Bt, sparse_matrix& C)
    {
        //sparse_matrix Bt(B.transpose());
        int r,c;
        int i,j;
        int op = 0;

        i = 0;
        while(i < A.nnz_)
        {
            r = A.row_[i];
            j = 0;
            while(j < Bt.nnz_)
            {
                c = Bt.row_[j];
                int ti = i;
                int tj = j;

                data sum = data();

                while(ti < A.nnz_ && A.row_[ti] == r && tj < Bt.nnz_ && Bt.row_[tj] == c)
                {
                    if(A.col_[ti] < Bt.col_[tj])
                        ++ti;
                    else if(A.col_[ti] > Bt.col_[tj])
                        ++tj;
                    else
                    {
                        sum += (A.val_[ti])*(Bt.val_[tj]);
                        ++ti;
                        ++tj;
                        ++op;
                    }
                    
                }

                if(sum != data())
                {
                    C.insert_seq(r,c,sum);
                }
                while(j < Bt.nnz_ && Bt.row_[j] == c)
                    ++j;
            }
            while(i < A.nnz_ && A.row_[i] == r)
                ++i;

        }
        return op;
    }

    int row_size()
    {
        return r_;
    }

    int col_size()
    {
        return c_;
    }

    int count_non_zeros()
    {
        return nnz_;
    }
    

    void disp() const
    {
        cout<<"\033[1;32m"<<"AR-type matrix "<<name_<<"\033[0m\n";;
        for(int i = 0; i < nnz_; ++i)
        {
            cout<<"("<<row_[i]<<","<<col_[i]<<") : "<<val_[i]<<"\n";

        }
        cout<<"---------------------\n";

    }

private:
    int r_;
    int c_;
    data z_;
    int nnz_;

    vector<data> val_;
    vector<int> row_;
    vector<int> col_;

    void insert_seq(int r, int c, data v)
    {
        row_.push_back(r);
        col_.push_back(c);
        val_.push_back(v);
        ++nnz_;
    }

};

#endif