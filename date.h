#ifndef DATE_H
#define DATE_H
class MyDate
{
	public:
	explicit MyDate(int dd = 1, int mm = 1, int yy = 1970) 
		: dd_(dd), mm_(mm), yy_(yy) {  }
	MyDate& operator++(); // pre
	MyDate operator++(int); // post
	friend ostream& operator<<(ostream&, const MyDate&);
	friend bool operator<(const MyDate& lhs, const MyDate& rhs);
	private:
	int dd_;
	int mm_;
	int yy_;
};
#endif
// all assignment operators : members of the class
// all relational operators : friends    
