#include <chrono>
#include <iostream>
#include <thread>

#include <args/args.hxx>
#include <hidapi/hidapi.h>

#include "dcledmessageboard.h"

namespace {
  struct print
  {
    template<typename T>
    auto& operator<<(const T& a) const { return std::cout << a; }
    ~print() { std::cout << std::endl; }
  };
  struct error
  {
    template<typename T>
    auto& operator<<(const T& a) const { return std::cerr << a; }
    ~error() { std::cerr << std::endl; }
  };
}

int listDevices();

int main(int argc, char* argv[])
{
  args::ArgumentParser parser("Dream Cheeky LED Message Board driver.", "");
  args::HelpFlag helpArg(parser, "help", "Display this help menu", {'h', "help"});
  //args::Group xorGroup(parser, "", args::Group::Validators::Xor);
  args::Flag listArg(parser, "list", "List all dcled devices", {'l', "list"});
//    args::Flag listArg2(parser, "list2", "List all dcled devices", {'t', "test"});
  args::ValueFlag<std::string> pathArg(parser, "path", "USB device path", {'p', "path"});
  try
  {
    parser.ParseCLI(argc, argv);
  }
  catch (args::Help)
  {
    std::cout << parser;
    return 0;
  }
  catch (args::ParseError e)
  {
    std::cerr << e.what() << std::endl;
    std::cerr << parser;
    return 1;
  }
  catch (args::ValidationError e)
  {
    std::cerr << e.what() << std::endl;
    std::cerr << parser;
    return 1;
  }

  if(!HidApi::init()) {
    error() << "hid_init() error...";
    return 1;
  }

  if(listArg) {
    listDevices();
    return 0;
  }

  auto dev_list = dcled::Device::list();
  if (!dev_list.size()) {
    error() << "No DC LED Message Board found.";
    return 1;
  }

  dcled::Device dev(pathArg ? dcled::Device(pathArg.Get()) : dcled::Device());
  if (!dev.isOpen()) {
    error() << "Error opening device" << (pathArg ? " (" + pathArg.Get() + ")." : ".");
    return 1;
  }

  dev.screen().setAll(true);
  dev.sendScreen();
  std::this_thread::sleep_for(std::chrono::milliseconds(250));

  return 0;
}

int listDevices()
{
  auto devs = dcled::Device::list();
  for( const auto& dev : devs ) {
    std::cout << " * " << "Device:       " << std::hex << dev.vendor_id << " " << dev.product_id
                                                                        << std::dec << std::endl
              << "   " << "Path:         " << dev.path << std::endl
              << "   " << "Manufacturer: " << dev.manufacturer << std::endl
              << "   " << "Product:      " << dev.product << std::endl << std::endl;
  }
  std::cout << "*** Found " << devs.size() << (devs.size() == 1 ? " device." : " devices.")
            << std::endl << std::endl;
  return static_cast<int>(devs.size());
}
