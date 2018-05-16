#include <iostream>
#include <tuple>

#include <signal.h>

#include <args/args.hxx>
#include <hidapi/hidapi.h>

#include "animations.h"
#include "device.h"
#include "screen.h"

namespace {

  struct print {
    template<typename T>
    auto& operator<<(const T& a) const { return std::cout << a; }
    ~print() { std::cout << std::endl; }
  };

  struct error {
    template<typename T>
    auto& operator<<(const T& a) const { return std::cerr << a; }
    ~error() { std::cerr << std::endl; }
  };

  void sighandler(int s)
  {
    dcled::stopThreads(s);
  }

  struct AnimationReader {
    void operator()(const std::string &name, const std::string &value, std::string &errormsg)
    {
      error() << name << " .. " << value;
      // TODO parse value, add to local global list if valid
      // otherwise set errormsg.
    }
  };

  int listDevices(bool emulate)
  {
    auto devices = dcled::Device::list();
    if (emulate) devices.push_back( {0, 0, dcled::Device::EMULATED_DEV_PATH,
                                     "#0", "Emulator", "stdout"});

    for( const auto& dev : devices ) {
      print() << " * " << "Device:       " << std::hex << dev.vendor_id << " "
                                                       << dev.product_id << std::dec;
      print() << "   " << "Path:         " << dev.path;
      print() << "   " << "Manufacturer: " << dev.manufacturer;
      print() << "   " << "Product:      " << dev.product << std::endl;
    }

    print() << "*** Found " << devices.size()
            << (devices.size() == 1 ? " device." : " devices.") << std::endl;

    return static_cast<int>(devices.size());
  }
}

int main(int argc, char* argv[])
{
  args::ArgumentParser parser("Dream Cheeky LED Message Board driver.");
  args::HelpFlag help_arg(parser, "help", "Display this help menu", {'h', "help"});
  args::Flag list_arg(parser, "list", "List all dcled devices", {'l', "list"});
  args::ValueFlag<std::string> pathArg(parser, "path", "USB device path", {'p', "path"});
  args::Flag stdout_arg(parser, "stdout", "Print device screen to stdout", {'s', "stdout"});
  args::Flag virtual_dev_arg(parser, "virtual", "Use only virtual stdout device\n"
                        "(includes --stdout)", {'v', "virtual"});

  args::Base emptyLine(""); parser.Add( emptyLine );
  args::PositionalList<std::string, std::list, AnimationReader>
          anims(parser, "ANIMATION", "Animations....");

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

  if (!virtual_dev_arg && !HidApi::init()) {
    error() << "Error: hid_init() failed.";
    return 1;
  }

  if (list_arg) {
    listDevices(virtual_dev_arg || stdout_arg);
    return 0;
  }

//  if (anims) {
//    for(const auto& t: anims.Get()) {
//      error() << "anim: " << t.a << " " << t.b;
//    }
//    return 0;
//  }


  if (!virtual_dev_arg)
  {
    if (!dcled::Device::list().size()) {
      error() << "No DC LED Message Board found.";
      return 1;
    }
  }

  #ifdef _WIN32
    signal(SIGABRT, &sighandler);
    signal(SIGTERM, &sighandler);
    signal(SIGINT,  &sighandler);
  #else
  {
    struct sigaction action;
    sigemptyset( &action.sa_mask );
    action.sa_handler = sighandler;
    action.sa_flags = 0;
    if( sigaction( SIGABRT, &action, NULL ) < 0 ) /*signal install error*/ (void)NULL;
    if( sigaction( SIGTERM, &action, NULL ) < 0 ) /*signal install error*/ (void)NULL;
    if( sigaction( SIGINT,  &action, NULL ) < 0 ) /*signal install error*/ (void)NULL;
    action.sa_handler = SIG_IGN;
    if( sigaction( SIGPIPE, &action, NULL ) < 0 ) /*signal install error*/ (void)NULL;
  }
  #endif

  dcled::Device dev(virtual_dev_arg ? dcled::Device(std::string(dcled::Device::EMULATED_DEV_PATH))
                                    : pathArg ? dcled::Device(pathArg.Get(), stdout_arg)
                                              : dcled::Device(stdout_arg));
  if (!dev.isOpen()) {
    error() << "Error opening device" << (pathArg ? " (" + pathArg.Get() + ")." : ".");
    return 1;
  }
//  auto ua = std::make_unique<dcled::PrintAnimation>(dcled::PrintAnimation(dcled::Screen().invert()));
//  std::unique_ptr<dcled::Animation> ua2
//      = std::make_unique<dcled::PrintAnimation>(dcled::PrintAnimation(dcled::Screen().invert()));
//  dev.enqueue(dcled::ShowScreenAnimation(dcled::Screen().invert()));
//  dev.enqueue(dcled::ShowScreenAnimation(dcled::Screen().invert()));
  dev.enqueue(dcled::ShowScreenAnimation(dcled::Screen().invert()));
  dev.enqueue( new dcled::ShowScreenAnimation(dcled::Screen().invert()));
//  dev.enqueue(std::make_unique<dcled::FontAnimation1>("TEST"));
//  dev.enqueue(dcled::FontAnimation("This is a string scroll test @@ !", dcled::ScrollSpeed::Fast));
//  dcled::FontAnimation fa("Another test");
//  dev.enqueue(std::move(fa));

  dev.playAll();
  return 0;
}

