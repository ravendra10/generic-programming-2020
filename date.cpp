#include <iostream>
using namespace std;

#include<vector>
#include"sparse_matrix.h"

class MyDate
{
	public:
	explicit MyDate(int dd = 1, int mm = 1, int yy = 1970) 
		: dd_(dd), mm_(mm), yy_(yy) {  }
	MyDate& operator++(); // pre
	MyDate operator++(int); // post
	friend ostream& operator<<(ostream&, const MyDate&);
	bool operator<(const MyDate& rhs);
	bool operator==(const MyDate& rhs);
    bool operator!=(const MyDate& rhs);
    private:
	int dd_;
	int mm_;
	int yy_;
};

ostream& operator<<(ostream& o, const MyDate& d)
{
	return o << d.dd_ << "-" << d.mm_ << "-" << d.yy_ ;
}

bool MyDate::operator<(const MyDate& rhs)
{
	if((*this).yy_ < rhs.yy_) 
		return true;
	else if((*this).yy_ == rhs.yy_ && (*this).mm_ < rhs.mm_) 
		return true;
	else if((*this).yy_ == rhs.yy_ && (*this).mm_ == rhs.mm_ &&
		(*this).dd_ < rhs.dd_) 
		return true;
	else
		return false;
		
}

bool MyDate::operator==(const MyDate& rhs) {
    return ((*this).dd_ == rhs.dd_) && ((*this).mm_ == rhs.mm_) && ((*this).yy_ == rhs.yy_);
}

bool MyDate::operator!=(const MyDate& rhs) {
    return !((*this)==rhs);
}
MyDate& MyDate::operator++() // pre
{
	// logic to change the date
	++dd_;
	return *this;
}

// prefer pre over post
// two extra objects ; 4 extra function calls
MyDate MyDate::operator++(int) // post
{
#if 0
	MyDate temp(*this);
	// logic to change the date
	++dd_;
	return temp;
#endif
//	write in canonical form
//	avoid repeating the logic
//	make post call pre
	MyDate temp(*this);
	++*this;
	return temp;
}
int main() {
    vector<MyDate> date_mat;
    for (int i=0;i<16;i++) {
        date_mat.push_back(MyDate());
    }
    date_mat[2] = MyDate(2,4,2019);
    date_mat[6] = MyDate(3,7,2019);
    date_mat[9] = MyDate(2,5,2019);
    date_mat[11] = MyDate(2,9,2019);
    date_mat[14] = MyDate(31,5,2019);
   
    
    sparse_matrix<MyDate,map<int,MyDate>> date_sparse(date_mat.begin(),
    date_mat.end(),4,4,MyDate());
    date_sparse.disp();
    date_sparse.insert_at(3,3,MyDate());
    date_sparse.disp();
    
    
}



