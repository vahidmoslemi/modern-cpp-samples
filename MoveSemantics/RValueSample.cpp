#include<iostream> 

//Sample code to demonstrate RValue reference concept

/*
According to  https://www.learncpp.com/cpp-tutorial/15-2-rvalue-references/

R-values references cannot be initialized with l-values.

R-value reference 		Can be initialized with 	Can modify
--------------------------------------------------------------------------
Modifiable l-values 		No			 	No
Non-modifiable l-values 	No 				No
R-values 			Yes		 		Yes
--------------------------------------------------------------------------
R-value reference to const 	Can be initialized with 	Can modify
--------------------------------------------------------------------------
Modifiable l-values 		No	 			No
Non-modifiable l-values 	No 				No
R-values 			Yes 				No

*/

using namespace std;

class MyClass{
	public:
		//default constructor initilaizes m_data property
		MyClass(){m_data=0;}
		
		//copy constructor
		MyClass(const MyClass& _A)
		{
			cout<<"Copy Constructor invoked."<<endl;
			m_data=_A.m_data;
		}

		//will avoid MyClass a=2  and will raise an error. it prevents from unwanted casting data types 
		explicit MyClass(int t_data):m_data{t_data}{
			cout<<"Regular value based constructor invoked: MyClass(int t_data)"<<endl;
		}

		//assignment operator to copy m_data from one object of type MyClass to another of the same time
		MyClass& operator=(const MyClass& _A)
		{
			cout<<"Assignment Operator Invoked."<<endl;
			if(this!=&_A)
			{
				m_data=_A.m_data;
			}
			return *this;
		}

		//plus (add) operator to return sum of 2 objects of type MyClass
		//Actually it returns an RValue object with m_data equal to the sum of the m_data properties from both invoker object and its operand
		MyClass operator+(const MyClass& _A)
		{
			MyClass result;	
			result.m_data=m_data+_A.m_data;
			cout<<"Addition Operator invoked"<<endl;
			return result;
		}

		//simple getter method
		int data() const {return m_data;}
	private:
		int m_data;
};

//Two overloaded methods to distinguish RValue and LValue integer arguments:
//You can comment any of the methods and recompile using g++ to check what happeds according to the concepts

//prefers constant LValue reference argument

void func(const int& x)
{
	cout<<"int Type: Call using const int: lvalue reference. Value: "<< x <<endl;
};

//Only Accepts RValue Reference argument
void func(int&& x)
{
	cout<<"int Type: Call using rvalue reference. Value: "<<x<<endl;
};

//Two overloaded methods to distinguish RValue and LValue MyClass type arguments:
//You can comment any of the methods and recompile using g++ to check what happeds according to the concepts

//Preferss LValue argument
void func(const MyClass& x)
{
	cout<<"MyClass Type: Call using const A: lvalue reference. Value: "<< x.data() <<endl;
};


//Only Accepts RValue argument
void func(MyClass &&x)
{
	cout<<"MyClass Type: Call using rvalue reference. Value: "<< x.data() <<endl;
};


int main()
{
	MyClass a(5); //same as:   MyClass a = MyClass(5);
	MyClass a2 = a; //same as: MyClass a2(a);
	cout<<"a is defined and initialized with 5"<<endl;
	a+MyClass(10);
	cout<<"a+10; executed and no changed was made to a itself"<<endl;
	int b=8;
	//uncommenting the following line will cause an error, because basic types addition operator  doesn't support
	//b+4=12;
	a+MyClass(4)=MyClass(12);
	cout<<"a+4=12 statement executed using rvalue concept with no error. but a not changed itself."<<endl;
	cout<<"a value: "<<a.data()<<endl;
	cout<<"a+4 value: "<<(a+MyClass(4)).data()<<endl;
	func(b+4);
	func(b);
	func(a+MyClass(4));
	func(a);
	return 0;
}


