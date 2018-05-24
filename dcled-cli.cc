#include "dcled-cli.h"

#include "animations.h"

#include <args/args.hxx>

#include <iostream>
#include <vector>

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

  int listDevices(bool emulate)
  {
    auto devices = dcled::Device::list();
    if (emulate) devices.push_back( {0, 0, dcled::Device::EMULATED_DEV_PATH,
                                     "#0", "Emulator", "stdout"});

    for( const auto& dev : devices ) {
      print() << " * " << "Device:       " << std::hex
              << dev.vendor_id << " " << dev.product_id << std::dec;
      print() << "   " << "Path:         " << dev.path;
      print() << "   " << "Manufacturer: " << dev.manufacturer;
      print() << "   " << "Product:      " << dev.product << std::endl;
    }

    print() << "*** Found " << devices.size()
            << (devices.size() == 1 ? " device." : " devices.") << std::endl;

    return static_cast<int>(devices.size());
  }

  std::vector<std::string> split(const std::string& s, const std::string& delimiter = ",",
                                 bool skipEmpty = true)
  {
    std::vector<std::string> strings;
    size_t last = 0, next = 0;
    while ((next = s.find(delimiter, last)) != std::string::npos)
    {
      auto substr = s.substr(last, next-last);
      if (!skipEmpty || substr.size()) {
        strings.push_back(std::move(substr));
      }
      last = next + 1;
    }
    auto substr = s.substr(last);
    if (!skipEmpty || substr.size()) {
      strings.push_back(std::move(substr));
    }
    return strings;
  }

  std::vector<std::string> split_first(const std::string& s, const std::string& delimiter = ",",
                                       bool skipEmpty = true)
  {
    std::vector<std::string> strings;
    size_t pos = 0, last = 0;
    if ((pos = s.find(delimiter, last)) != std::string::npos)
    {
      auto substr = s.substr(last, pos-last);
      if (!skipEmpty || substr.size()) {
        strings.push_back(std::move(substr));
      }
      last = pos + 1;
    }
    auto substr = s.substr(last);
    if (!skipEmpty || substr.size()) {
      strings.push_back(std::move(substr));
    }
    return strings;
  }

  template<typename Container, class T>
  bool contains(const Container& c, T p) {
      return std::find(c.cbegin(), c.cend(), p) != c.cend();
  }

  constexpr std::array<char, 10> animationShorts = { 't', 'f' };

  template<std::list<std::unique_ptr<dcled::Animation>>* List>
  struct AnimationReader {
    void operator()(const std::string &name, const std::string &value, std::string &errormsg)
    {
      if (!value.size()) return;

      if (!contains(animationShorts, value[0])) {
        //throw args::ParseError("aaaaaaaaaaaaaaaaaaa");
        errormsg += "Invalid Animation descr4ipttion.";
        return;
      }
      // t,100=skdfjasdlfk lkj lk
//      List->push_back( value );
//      if (

      const auto a = split_first(value, "=");
      if (a.size() > 1 && a.front()[0] == 't')
      {
        List->push_back(std::make_unique<dcled::TextAnimation>(a[1]));
      }

      else
        errormsg = value;
    }
  };

  dcled::AnimationList Animation_List_;
} // end anonymous namespace

dcled::AnimationList dcled::cli::ArgParser::animationList()
{
  return std::move(Animation_List_);
}

dcled::Device dcled::cli::ArgParser::device()
{
  if (!parsed_) return dcled::Device();
  if (path_.size()) return dcled::Device(path_, to_stdout_);
  return dcled::Device(to_stdout_);
}

dcled::cli::ArgParser::State
dcled::cli::ArgParser::parse(int argc, char *argv[])
{
  Animation_List_.clear();
  path_.clear();
  parsed_ = false;
  virtual_ = false;
  to_stdout_ = false;

  args::ArgumentParser parser("Dream Cheeky LED Message Board driver.");
  args::HelpFlag help_arg(parser, "help", "Display this help menu", {'h', "help"});
  args::HelpFlag version_arg(parser, "version", "Display the program version", {"version"});
  args::Flag list_arg(parser, "list", "List all dcled devices", {'l', "list"});
  args::ValueFlag<std::string> path_arg(parser, "path", "USB device path", {'p', "path"});
  args::Flag stdout_arg(parser, "stdout", "Print device screen to stdout", {'s', "stdout"});
  args::Flag virtual_dev_arg(parser, "virtual", "Use only virtual stdout device\n"
                                                "(includes --stdout)", {'v', "virtual"});

  args::Base emptyLine(""); parser.Add( emptyLine );
  args::PositionalList<std::string, std::list, AnimationReader<&Animation_List_>>
          animations(parser, "ANIMATION", "List of animations with the format:\n" "[Type][,Arguments...][=Data]");
  parser.Add( emptyLine );

  args::NamedBase text1("f : Read from File","Read animation list from file\n"
                                            "  Data (required) : File path" );
  parser.Add( text1 ); parser.Add( emptyLine );

  args::NamedBase text0("t : TextAnimation","Scrolling text\n"
                                            "  Optional Arguments:\n"
                                            "   - ScrollSpeed (default=100)\n"
                                            "  Data (required) : Scroll text" );
  parser.Add( text0 );

  try
  {
    parser.ParseCLI(argc, argv);
  }
  catch (args::Help flag)
  {
    if (flag.what() == std::string("version"))
      print() << "TODO: Build in version information (auto generated via CMake & git)";
    else
      std::cout << parser;
    return State::EXIT;
  }
  catch (args::ParseError e)
  {
    error() << "ParseError: " << e.what();
    //std::cerr << parser;
    return State::ERROR;
  }
  catch (args::ValidationError e)
  {
    error() << "ValidationError: " << e.what();
    //std::cerr << parser;
    return State::ERROR;
  }

  if (!virtual_dev_arg && !HidApi::init()) {
    error() << "Error: hid_init() failed.";
    return State::ERROR;
  }

  if (list_arg) {
    listDevices(virtual_dev_arg || stdout_arg);
    return State::EXIT;
  }

  if (!virtual_dev_arg)
  {
    if (!dcled::Device::list().size()) {
      error() << "No DC LED Message Board found.";
      return State::ERROR;
    }
  }

  if (animations) {
    bool error_occured = false;
    for (auto& errmsg: animations.Get()) {
      if (errmsg.size()) {
        if (!error_occured) {
          error_occured = true;
          error() << "Animation parse error:";
        }
        error() << " - " << errmsg;
      }
    }
    if (error_occured)
      return State::ERROR;
  }

  parsed_ = true;
  to_stdout_ = stdout_arg;

  if (virtual_dev_arg) {
    virtual_ = true;
    path_ = std::string(dcled::Device::EMULATED_DEV_PATH);
  }
  else if (path_arg) {
    path_ = path_arg.Get();
  }

  return State::OK;
}
