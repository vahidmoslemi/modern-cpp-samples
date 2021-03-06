#include<iostream>
#include<cstring>
#include<memory>
/*
	In this sample code I tried to demonstrate use of move semantics using RValue References in modern C++
	Special thanks to this link: https://www.internalpointers.com/post/c-rvalue-references-and-move-semantics-beginners
	
	Notice Only for learning purposes:
		Inorder to see when Move/Copy Constructors are invoked please disable ReturnValueOptimization in g++:
	 	g++ -fno-elide-constructors MoveSemantic.cpp
	
	More on Return Value Optimization (RVO):
		In case of RVO, the compiler will not even invoke redundant Move/Copy Contructor and 
		directly construct the return value of a function at the call site.
		
	According to https://www.ibm.com/developerworks/community/blogs/5894415f-be62-4bc0-81c5-3956e82276f3/entry/RVO_V_S_std_move?lang=en:
		To summarize, RVO is a compiler optimization technique, while std::move is just an rvalue cast,
		which also instructs the compiler that it's eligible to move the object. The price of moving is 
		lower than copying but higher than RVO, so never apply std::move to local objects if they would otherwise be eligible for the RVO.
		
*/

#define DEFAULT_BUFFER_SIZE 100
typedef unsigned char Byte;
const Byte result[1]{'\0'};

//Implementing sample class (DataBuffer) using Rule of 5: Copy/Move Constructor + Copy/Move Assignment Operator + Destructor
class DataBuffer{
	public: 
		DataBuffer(unsigned int t_data_size=DEFAULT_BUFFER_SIZE)
		{
			std::cout<<"Default Constructor used. Based on BufferLength."<<std::endl;
			m_data_size=t_data_size;
			m_data=new Byte[m_data_size];
		}

		DataBuffer(const char* t_data)
		{
			std::cout<<"Default Constructor used. Based on Initial Data."<<std::endl;
                        m_data_size = strlen(t_data);
                        m_data=new Byte[m_data_size];
                        std::copy(t_data,t_data + m_data_size, m_data);	
		}

		~DataBuffer()
		{
			m_data_size = 0;
			delete [] m_data;
			std::cout<<"Default Destructor invoked"<<std::endl;
		}

		//copy costructor
		DataBuffer(const DataBuffer& other)
		{
			m_data_size = other.m_data_size;
			m_data = new Byte[m_data_size];
			std::copy(other.m_data,other.m_data + m_data_size,m_data);
			std::cout<<"Copy Constructor used"<<std::endl;
		}

		//Move Constructor using RValue Reference ( optional: use noexcept for better code optimization) Move Conxtructor will not throw
		DataBuffer(DataBuffer&& other) noexcept
		{
			//grab/steal resource and data from  the other object
			m_data_size = other.m_data_size;
			m_data = other.m_data;
			//reset other's data pointer to avoid data free by the other's distructor (RValueReferece will go out of scope at the end)
			other.m_data=nullptr;
			other.m_data_size = 0;
			std::cout<<"Move Constructor used"<<std::endl;
		}

		//Copy Assignment Operator
		DataBuffer& operator=(const DataBuffer&  other)
		{
			//avoid self copy
			if(this!=&other)
			{
				delete[] m_data;
				m_data=nullptr;
	                        m_data_size = other.m_data_size;
        	                m_data = new Byte[m_data_size];
                	        std::copy(other.m_data,other.m_data + m_data_size,m_data);
			}
			std::cout<<"Copy Assignment Operator used"<<std::endl;
			return *this;
		}

		//Move Assignment Operator
		DataBuffer& operator=(DataBuffer&&  other)
		{
			//avoid self copy
			if(this!=&other)
			{
				delete[] m_data;
								m_data_size = other.m_data_size;
								m_data = other.m_data;
				other.m_data = nullptr;
				other.m_data_size = 0;
			}
			std::cout<<"Move Assignment Operator used"<<std::endl;
			return *this;
		}

		DataBuffer& operator+(DataBuffer&& other)
		{
			
		}

		void setData(const char* t_data)
		{
			delete[] m_data;
			m_data_size = strlen(t_data);
			m_data=new Byte[m_data_size];
			std::copy(t_data,t_data + m_data_size, m_data);
			std::cout<<"SetData invoked"<<std::endl;
		}
		
		const Byte* getData() 
		{
			if(m_data!=nullptr || m_data_size > 0) 
				return m_data;
			else
			{
				return result;
			}
		} 

	private:
		unsigned int  m_data_size;
		Byte* m_data;
};

//This function is placed only for learing purpose
DataBuffer createInputBuffer()
{
	std::cout<<"inside createInputBuffer()"<<std::endl;
	DataBuffer input_buf("Input");
	//Move Constructor is used in order to return DataBuffer Object and then local variable will be destructed.
	return input_buf;
	
	//Instead of the above 2 lines we could use inline object construction: This will lead to better optimization based on ReturnValueOptimization (RVO)
	//return DataBuffer("Input");
}

void printBuffer(DataBuffer&& buffer)
{
	std::cout<<"Inside printBuffer: "<<buffer.getData()<<std::endl;
}

int main()
{
	//Test DataBuffer using sample data
	DataBuffer db1(10);
	db1.setData("Hello");
	std::cout<<"Data1: "<<db1.getData()<<std::endl;

	//Move Constructor will be invoked: remember to disable RVO : g++ -fno-elide-constructors MoveSemantic.cpp
	DataBuffer db2 (createInputBuffer());	
	std::cout<<"Data2: "<<db2.getData()<<std::endl;
	
	db2 = db1;
	
	db2 = DataBuffer("TempBuffer");
	std::cout<<"Data2: "<<db2.getData()<<std::endl;

	db2 = std::move(db1);
	std::cout<<"Data2: "<<db2.getData()<<std::endl;

	std::cout<<"Data1 moved: "<<db1.getData()<<std::endl;

	printBuffer(DataBuffer("On the Fly Buffer"));

	db1.setData(" Modern C++!");
	std::cout<<"Set db1 again to '"<<db1.getData()<<"'"<<std::endl;
	std::cout<<"Swapping db1 and db2 data using Move Semantics"<<std::endl;
        std::swap(db1,db2);

        std::cout<<"Final Message: "<<db1.getData()<<db2.getData()<<std::endl;

	return 0;
}
