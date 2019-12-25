#include "device.h"

//nested standard container initialization
const static std::map<DEVICE_STATUS,std::string> device_status_labels={
    {READY,"READY"},
    {STARTING,"STARTING"},
    {IDLE,"IDLE"},
    {FAULT,"FAULT"},
    {STOPPED,"STOPPED"}
};

const static std::map<DEVICE_TYPE,const std::string> device_type_labels={
    {KEYBOARD,"KEYBOARD"},
    {MOUSE,"MOUSE"},
    {DISPLAY,"DISPLAY"},
    {GPIO,"GENERIC_IO"},
    {PRINTER,"PRINTER"}
};

int Device::next_device_id=1;


Device& Device::operator=(Device&& t_source)
{
    std::cout<<">>Inside Move Assignment Operator."<<std::endl;
    delete [] m_buffer;
    m_buffer_capacity = t_source.m_buffer_capacity;
    m_buffer = t_source.m_buffer;
    t_source.m_buffer = nullptr;
    t_source.m_buffer_capacity=0;

    m_type = t_source.m_type;
    m_status = t_source.m_status;
    m_comment = t_source.m_comment;
    m_buffer_data_size = t_source.m_buffer_data_size;

    t_source.m_type = GPIO;
    t_source.m_comment="";
    t_source.m_status = STOPPED;

    return *this;
}

void Device::addCommentToDevice(std::string t_comment)
{
    m_comment += "\n"+t_comment;
}

int Device::getId() const
{
    return m_id;
}

std::string Device::getComment() const
{
    return  m_comment;
}

DEVICE_STATUS Device::getStatusCode() const
{
     return m_status;
}

const std::string Device::getStatusLabel() const
{
    // device_status_labels is defined as "const std::map" so we use .at method here instead of operator[]
    // Because operator[] will add a node if key is not found (and its forbidden for a const map)
    return device_status_labels.at(m_status);
}

const std::string Device::getTypeLabel() const
{
    // device_status_labels is defined as "const std::map" so we use .at method here instead of operator[]
    // Because operator[] will add a node if key is not found (and its forbidden for a const map)
    return device_type_labels.at(m_type);
}
