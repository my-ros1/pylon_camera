/**
 This program prints the camera info.
 @author Kuang Fangjun
*/

#include <pylon/PylonIncludes.h>
#include <algorithm>
#include <unistd.h>
#include <string>
#include <sstream>

int main(int argc, char* argv[])
{

    // Before using any pylon methods, the pylon runtime must be initialized.
    Pylon::PylonInitialize();

    Pylon::CTlFactory& tl_factory = Pylon::CTlFactory::GetInstance();
    Pylon::DeviceInfoList_t device_list;

    // EnumerateDevices() returns the number of devices found
    if ( 0 == tl_factory.EnumerateDevices(device_list) )
    {
        Pylon::PylonTerminate();
        std::cout << "No camera present\n";
        return -1;
    }

    int i = 0;
    for (const auto& info : device_list)
    {
        std::stringstream ss;
        ss << "====================\n";
        ss <<  "camera " << i << "\n";
        ss << "--------------------\n";

        ss << ">>>--->>user defined name: " << info.GetUserDefinedName() << "\n";
        ss << ">>>--->>full name: " << info.GetFullName() << "\n";
        ss << "device class: " << info.GetDeviceClass() << "\n";
        ss << "friendly name: " << info.GetFriendlyName() << "\n";
        ss << "device factory: " << info.GetDeviceFactory() << "\n";
        ss << "device version: " << info.GetDeviceVersion() << "\n";
        ss << "model name: " << info.GetModelName() << "\n";
        ss << "serial number: " << info.GetSerialNumber() << "\n";
        ss << "vendor name: " << info.GetVendorName() << "\n";

        std::cout << ss.str() << "\n";
    }

    // Releases all pylon resources.
    Pylon::PylonTerminate();

    return 0;
}
