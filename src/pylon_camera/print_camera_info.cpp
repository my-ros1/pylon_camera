/**
 This program prints the camera info.
 @author Kuang Fangjun
*/

#include <pylon/PylonIncludes.h>
#include <pylon/gige/BaslerGigEInstantCamera.h>
#include <algorithm>
#include <unistd.h>
#include <string>
#include <sstream>

typedef Pylon::CBaslerGigEInstantCamera CBaslerInstantCameraT;

std::string
print_settings(CBaslerInstantCameraT *cam)
{
    std::stringstream ss;

    ss << "device user id: " << cam->DeviceUserID() << "\n";
    ss << "device id: " << cam->DeviceID() << "\n";
    ss << "device model name: " << cam->DeviceModelName() << "\n";

    Pylon::String_t ip, mask, gateway;
    cam->GetPersistentIpAddress(ip, mask, gateway);
    ss << "ip: " << ip << "\n";
    ss << "network mask: " << mask << "\n";
    ss << "gateway: " << gateway << "\n";
    ss << "mtu size: " << cam->GevSCPSPacketSize.GetValue() << " bytes\n";
    if ( GenApi::IsAvailable(cam->ResultingFrameRateAbs) )
    {
        ss << "frame rate: " << cam->ResultingFrameRateAbs() << "\n";
    }

    ss << "device vendor name: " << cam->DeviceVendorName() << "\n";
    ss << "device version:" << cam->DeviceVersion() << "\n";
    ss << "device firmware version: " << cam->DeviceFirmwareVersion() << "\n";
    ss << "device manufacturer info: " << cam->DeviceManufacturerInfo() << "\n";
    ss << "width: " << cam->Width() << "\n";
    ss << "height: " << cam->Height() << "\n";
    ss << "pixel depth: " << cam->PixelSize.GetIntValue() << " bits" << "\n";
    ss << "reverse X: " << cam->ReverseX() << "\n";
    ss << "reverse Y: " << cam->ReverseY() << "\n";



    if ( GenApi::IsAvailable(cam->BinningHorizontal) &&
         GenApi::IsAvailable(cam->BinningVertical) )
    {
        ss << "Cam has binning range: x(hz) = ["
                                << cam->BinningHorizontal.GetMin() << " - "
                                << cam->BinningHorizontal.GetMax() << "], y(vt) = ["
                                << cam->BinningVertical.GetMin() << " - "
                                << cam->BinningVertical.GetMax() << "].";
    }
    else
    {
        ss << "Cam does not support binning.\n";
    }

    std::string gen_api_encoding(cam->PixelFormat.ToString().c_str());
    ss << "pixel format: " << gen_api_encoding << "\n";

    return ss.str();
}

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

        if (info.IsDeviceClassAvailable())
        {

            auto device_class = info.GetDeviceClass();
            if (device_class == "BaslerGigE" )
            {
                Pylon::IPylonDevice *dev = tl_factory.CreateDevice(info);
                CBaslerInstantCameraT *cam = new CBaslerInstantCameraT(dev);

                if (cam)
                {
                    cam->RegisterConfiguration(new Pylon::CSoftwareTriggerConfiguration,
                                               Pylon::RegistrationMode_ReplaceAll,
                                               Pylon::Cleanup_Delete);

                    try
                    {
                        cam->Open();
                        ss << print_settings(cam);
                        cam->Close();
                    }
                    catch ( const GenICam::GenericException &e )
                    {
                        std::cerr << e.GetDescription() << "\n";
                    }

                    delete cam;
                }
            }
        }

        std::cout << ss.str() << "\n";
    }

    // Releases all pylon resources.
    Pylon::PylonTerminate();

    return 0;
}
