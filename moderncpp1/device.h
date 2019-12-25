#ifndef DEVICE_H
#define DEVICE_H

#include <iostream>
#include <string>
#include <map>

typedef unsigned char BYTE;
typedef unsigned long DATA_SIZE;

const DATA_SIZE DEFAULT_BUFFER_CAPACITY = 32;

enum DEVICE_STATUS{READY=0,STARTING,IDLE,FAULT,STOPPED};
enum DEVICE_TYPE{GPIO,KEYBOARD,MOUSE,DISPLAY,PRINTER};



class Device{
public:
    //automatic device_id
    static int next_device_id;

    //constructors
    //1- using contructor initializer list (using braces) : here for non-static const/reference data members
    Device():m_id{next_device_id++},m_type{GPIO},m_status{STARTING},m_buffer_capacity{DEFAULT_BUFFER_CAPACITY},
             m_buffer{new BYTE[m_buffer_capacity]}{}

    //8-Using explicit identifier to prevent expressions such as: Device a =1; see main.cpp
    explicit Device(DATA_SIZE t_buffer_size):m_id{next_device_id++},m_type{GPIO},m_status{STARTING},m_buffer_capacity{t_buffer_size},
             m_buffer{new BYTE[t_buffer_size]}{}

    Device(DEVICE_TYPE t_type,DEVICE_STATUS t_status,DATA_SIZE t_buffer_size):m_id{next_device_id++},m_type{t_type},
                                                      m_status{t_status},m_buffer_capacity{t_buffer_size},
                                                      m_buffer{new BYTE[t_buffer_size]}{}
    Device(int t_id, DEVICE_TYPE t_type, DEVICE_STATUS t_status,DATA_SIZE t_buffer_size):m_id{t_id},m_type{t_type},m_status{t_status}
                                                                 ,m_buffer_capacity{t_buffer_size}
                                                                 ,m_buffer{new BYTE[t_buffer_size]}{}

    //also we could define t_source_dev as (const Device&) to serve
    //2- RValue Reference in copy constructor(needed by make_unique function)
    Device(Device&& t_source_dev):m_id{t_source_dev.m_id},m_type{t_source_dev.m_type},m_status{t_source_dev.m_status}{
        std::cout<<"#New Device Instance Created using Move Constructor."<<std::endl;
        *this = std::move(t_source_dev);
    }

    Device(Device& t_source_dev):m_id{t_source_dev.m_id},m_type{t_source_dev.m_type},m_status{t_source_dev.m_status}
                                ,m_buffer_capacity{t_source_dev.m_buffer_capacity}
                                ,m_buffer{new BYTE[t_source_dev.m_buffer_capacity]}
    {
        std::cout<<"#New Device Instance Created using Copy Constructor."<<std::endl;
    }

    Device& operator=(Device&& t_source);

    void addCommentToDevice(std::string t_comment);
    std::string getComment() const;
    DEVICE_STATUS getStatusCode() const;
    int getId() const;
    const std::string getStatusLabel() const;
    const std::string getTypeLabel() const;

private:
    const int m_id;
    DEVICE_TYPE m_type;
    DEVICE_STATUS m_status;
    std::string m_comment;
    DATA_SIZE m_buffer_capacity=DEFAULT_BUFFER_CAPACITY;
    int m_buffer_data_size;
    BYTE* m_buffer=new BYTE[m_buffer_capacity];
};

#endif // DEVICE_H
