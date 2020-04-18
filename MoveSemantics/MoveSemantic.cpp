#include<iostream>
#include<vector>
#include<cstring>
#include<memory>
/*
	In this sample code I tried to demonstrate use of move semantics using RValue References in modern C++
	Special thanks to this link: https://www.internalpointers.com/post/c-rvalue-references-and-move-semantics-beginners
	
	Notice:
		Inorder to see when Move/Copy Constructors are invoked please disable ReturnValueOptimization in g++:
	 	g++ -fno-elide-constructors MoveSemantic.cpp
*/

#define DEFAULT_BUFFER_SIZE 100

typedef unsigned char Byte;
typedef std::vector<Byte> ByteArray;

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
		DataBuffer(DataBuffer&& other)
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
		
		const Byte* getData() { return m_data;} 

	private:
		unsigned int  m_data_size;
		Byte* m_data;
};

DataBuffer createInputBuffer()
{
	std::cout<<"inside createInputBuffer()"<<std::endl;
	DataBuffer input_buf("Input");
	//Move Constructor is used in order to return DataBuffer Object and then local variable will be destructed.
	return input_buf;
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
	DataBuffer db2(createInputBuffer());
	std::cout<<"Data2: "<<db2.getData()<<std::endl;

	printBuffer(DataBuffer("On the Fly Buffer"));

	return 0;
}
