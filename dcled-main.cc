#include <chrono>
#include <iostream>
#include <thread>
#include <atomic>

#include <signal.h>
#include <unistd.h>

#include <args/args.hxx>
#include <hidapi/hidapi.h>

#include "animations.h"
#include "device.h"
#include "screen.h"

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

  std::atomic<bool> sigInt(false);
}

void sighandler(int s)
{
  sigInt = true;
}

int listDevices(bool emulate = false);

int main(int argc, char* argv[])
{
  args::ArgumentParser parser("Dream Cheeky LED Message Board driver.", "");
  args::HelpFlag helpArg(parser, "help", "Display this help menu", {'h', "help"});
  args::Flag listArg(parser, "list", "List all dcled devices", {'l', "list"});
  args::Flag stdoutArg(parser, "stdout", "Print device screen to stdout", {'s', "stdout"});
  args::Flag norealdevArg(parser, "norealdevice", "Do not search/use real device\n"
                        "(includes --stdout)", {'n', "norealdev"});
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
    error() << e.what();
    std::cerr << parser;
    return 1;
  }
  catch (args::ValidationError e)
  {
    error() << e.what();
    std::cerr << parser;
    return 1;
  }

  if (!norealdevArg && !HidApi::init()) {
    error() << "Error: hid_init() failed.";
    return 1;
  }

  if (listArg) {
    listDevices(norealdevArg);
    return 0;
  }

  if (!norealdevArg)
  {
    if (!dcled::Device::list().size()) {
      error() << "No DC LED Message Board found.";
      return 1;
    }
  }

  dcled::Device dev(norealdevArg ? dcled::Device(std::string(dcled::Device::EMULATED_DEV_PATH))
                                 : pathArg ? dcled::Device(pathArg.Get(), stdoutArg)
                                           : dcled::Device(stdoutArg));
  if (!dev.isOpen()) {
    error() << "Error opening device" << (pathArg ? " (" + pathArg.Get() + ")." : ".");
    return 1;
  }

  dcled::ExitAnimation e;
  dcled::ExitAnimation2 e2;

  dev.enqueue(std::make_unique<dcled::ExitAnimation>());
//s  dev.enqueue(std::make_unique<dcled::ExitAnimation2>());
  dev.enqueue(std::make_unique<dcled::FontAnimation1>());
  dev.playAll();
  return 0;
}

int listDevices(bool emulate)
{
  auto devices = emulate ? std::list<dcled::DeviceInfo>{{0, 0, dcled::Device::EMULATED_DEV_PATH,
                                                         "#0", "Emulator", "stdout"}}
                         : dcled::Device::list();
  for( const auto& dev : devices ) {
    print() << " * " << "Device:       " << std::hex << dev.vendor_id << " "
                                                     << dev.product_id << std::dec;
    print() << "   " << "Path:         " << dev.path;
    print() << "   " << "Manufacturer: " << dev.manufacturer;
    print() << "   " << "Product:      " << dev.product << std::endl;
  }
  print() << "*** Found " << devices.size() << (devices.size() == 1 ? " device." : " devices.") << std::endl;
  return static_cast<int>(devices.size());
}
