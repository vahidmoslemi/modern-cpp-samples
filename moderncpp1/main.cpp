/*
 * In this simple example the following concepts are implemented and tested in modern C++:
 * 1- using contructor initializer list (using braces) : here for non-static const/reference data members
 * 2- RValue Reference parameter (&&) needed by make_unique
 * 3- smart std::unique_pointer : using std::move helper function to assign a unique_ptr object to another (used in vector::push_back)
 * 4- using std::map and std::vector
 * 5- range-based for loop
 * 6- auto data type
 * 7- nested standard container initialization (e.g. std::map device_type_labels and device_status_labels)
 * 8- explicit identifier usage in class Constructor
 * 9- implementing move semantics : move contructor and move assignment operator
 */

#include<memory>
#include<vector>
#include<iostream>
#include "device.h"

//Sample object accessor function based on object Reference
void printDeviceInfo(Device& t_device){
    std::cout<<"device (id,status): "<< t_device.getId()<<","<<t_device.getStatusLabel()<<std::endl;
    std::cout<<"device Comment: "<< t_device.getComment()<<std::endl;
    t_device.addCommentToDevice("Request to Print Device Info DONE");
}

//Sample object accessor function based on unique_ptr reference
void printDeviceInfo(const std::unique_ptr<Device>& t_device_ptr){
    std::cout<<"device (id,status): "<< t_device_ptr->getId()<<","<<t_device_ptr->getStatusLabel()<<std::endl;
    std::cout<<"device Comment: "<< t_device_ptr->getComment()<<std::endl;
    t_device_ptr->addCommentToDevice("Request to Print Device Info DONE");
}

//smart std::unique_pointer usage
std::unique_ptr<Device> createDevice(DEVICE_TYPE t_type,DEVICE_STATUS t_status)
{
    return std::make_unique<Device>(Device(t_type,t_status,32));
}

int main(int argc, char *argv[])
{
    std::vector<std::unique_ptr<Device>> device_list;
    //auto data type
    auto ptr = createDevice(GPIO,IDLE);

    //using explicit identifier befor Device(DATA_SIZE t_buffer_size) constructor
    // making the bellow line invalid and will not compile.
    //if you remove explicit the 1.5 is evaluated is auto converted to DATA_SIZE/unsigned long (narrowed to 1) and
    //Device(1) constructor makes a device object with buffer_size 1 and assign it to a;
    //--------------------------------------------------------------------------------------------------------------
//    Device a=1.5;
    //--------------------------------------------------------------------------------------------------------------


    printDeviceInfo(ptr);

    device_list.push_back(std::move(ptr));
    std::cout<<"Device Object Comment after printInfo invokation: "<<device_list[0]->getComment()<<std::endl;

    std::cout<<"Adding 4 more devices to the device list vector . . ."<<std::endl;
    device_list.push_back(createDevice(KEYBOARD,STOPPED));
    device_list.push_back(createDevice(MOUSE,STOPPED));
    device_list.push_back(createDevice(DISPLAY,IDLE));
    device_list.push_back(createDevice(PRINTER,IDLE));

    device_list.push_back(std::make_unique<Device>(256));


    std::cout<<"Listing all " << device_list.size()<<" devices to the device list vector . . ."<<std::endl;


    //range-based for loop and auto data type
    for(const auto& device:device_list)
    {
        std::cout<<"Device #"<<device->getId()<<" -> Type: "<<device->getTypeLabel()
                <<" -> Status: "<<device->getStatusLabel()<<std::endl;
    }

    //inhvoking Copy Assignment operator and Copy Constructor
    Device a = *device_list[0];

    //inhvoking Move Assignment operator cause a to reset data
    Device b = std::move(a);


    std::cout<<"----------------------------------------------------"<<std::endl;
    std::cout<<"Now Modify Devices List"<<std::endl;

    //Invoking Move Constructor
    device_list[0] = std::make_unique<Device>(Device(128));

    //Invoking Copy Constructor
    device_list.push_back(std::make_unique<Device>(b));

    //range-based for loop and auto data type
    for(const auto& device:device_list)
    {
        std::cout<<"Device #"<<device->getId()<<" -> Type: "<<device->getTypeLabel()
                <<" -> Status: "<<device->getStatusLabel()<<std::endl;
    }

    device_list.clear();

    return 0;
}
