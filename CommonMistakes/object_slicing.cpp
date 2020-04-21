#include<iostream>
#include<string>

/**
 * This simple example demonstrate avoid Object Slicing and how to avoid it using call by reference
 * 
 * According to WikiPedia:
 * 		In C++ programming, object slicing occurs when an object of a subclass type is copied to an object
 * 		of superclass type: the superclass copy will not have any of the member variables defined in the subclass.
 * 		These variables have, in effect, been "sliced off". 
 */

class BaseLogger{
	public:
		BaseLogger(std::string t_msg="BASE LOGGER INITIALIZED"):m_msg{t_msg}{}
		
		//const identifier is used to avoid compiler error when using const object reference
		//this identifier tells the compiler that this method will not make any change to the object data
		virtual std::string getMessage() const {return m_msg;}
		
		void setMessage(std::string t_msg){m_msg=t_msg;}
	protected:
		std::string m_msg;
};

class MyLogger : public BaseLogger{
	public:
		//Using Parent Class Construtor
		MyLogger(std::string t_msg):BaseLogger(t_msg){}
		
		//const identifier is used to avoid compiler error when using const object reference
		//this identifier tells the compiler that this method will not make any change to the object data
		virtual std::string getMessage() const override{
			return std::string("Decorated Log Message: ") + "(" + m_msg + ")";
		}
};

//Call function by Value will cause object slicing to the input argument
void writeLogByVal(BaseLogger logger)
{
	std::cout<<"writeLogByVal - This will write to log file: \n"<<logger.getMessage()<<std::endl;
}

//avoid Object Slicing using object Reference
void writeLogByRef(const BaseLogger& logger)
{
	std::cout<<"writeLogByRef - This will write to log file: \n"<<logger.getMessage()<<std::endl;
}

int main()
{
	BaseLogger base_logger;
	std::cout<<"base_logger message: \n"<<base_logger.getMessage()<<std::endl;
	MyLogger mylogger("Preparing Logger");
	std::cout<<"mylogger message: \n"<<mylogger.getMessage()<<std::endl;
	writeLogByVal(mylogger);
	writeLogByRef(mylogger);
	return 0;
}
