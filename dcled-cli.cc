// dcled-hidapi - userland driver for the Dream Cheeky LED Message Board
// Copyright 2018 Jahn Fuchs <github.jahnf@wolke7.net>
// Distributed under the MIT License. See accompanying LICENSE file.

#include "dcled-cli.h"

#include "animations.h"
#include "output.h"

#include <args/args.hxx>
#include <dcled-GitVersion.h>

#include <algorithm>
#include <cerrno>
#include <cstring>
#include <fstream>
#include <map>
#include <vector>

#define ANIMATIONREADER_THROWS 1
#define ANIMATION_ARG_DELIMITER_STR ","
#define ANIMATION_ARG_DELIMITER_CHAR ','

namespace // *****************************************************************
{
  int listDevices(bool emulate);
  std::vector<std::string> split(const std::string& s, const std::string& delimiter = ",",
                                 bool skipEmpty = true, size_t maxSplits = 0);
  std::vector<std::string> split_first_one(const std::string& s, const std::string& delimiter = ",",
                                           bool skipEmpty = true);
  template<typename Container, typename T>
  bool contains(const Container& c, const T& value) {
    return (std::find(c.cbegin(), c.cend(), value) != c.cend());
  }
  void parseError(std::string msg, std::string& errormsg, bool forceNoParseErrorException = false);
  bool parseAnimationLine(const std::string& value, std::string& errormsg, dcled::AnimationList* list,
                          bool forceNoParseErrorException = false);
  std::map<std::string, std::string> parseAnimationArgs(const std::vector<std::string>& animationArgs,
                                                        const std::vector<std::string>& validArgs,
                                                        const std::vector<std::string>& requiredArgs,
                                                        const std::string& animationName,
                                                        bool& errorOccured,
                                                        std::string& errormsg,
                                                        bool forceNoParseErrorException = false);

  bool make_TextAnimation(const std::vector<std::string>& animationArgs,
                          const std::string& animationData,
                          std::string& errormsg, dcled::AnimationList* list,
                          bool forceNoParseErrorException = false);

  bool make_ClockAnimation(const std::vector<std::string>& animationArgs,
                           const std::string& animationData,
                           std::string& errormsg, dcled::AnimationList* list,
                           bool forceNoParseErrorException = false);

  bool make_InvertAnimation(const std::vector<std::string>& animationArgs,
                            const std::string& animationData,
                            std::string& errormsg, dcled::AnimationList* list,
                            bool forceNoParseErrorException = false);

  bool make_FlipAnimation(const std::vector<std::string>& animationArgs,
                          const std::string& animationData,
                          std::string& errormsg, dcled::AnimationList* list,
                          bool forceNoParseErrorException = false);

  bool make_NilAnimation(const std::vector<std::string>& animationArgs,
                          const std::string& animationData,
                          std::string& errormsg, dcled::AnimationList* list,
                          bool forceNoParseErrorException = false);

  bool make_SetRectAnimation(const std::vector<std::string>& animationArgs,
                             const std::string& animationData,
                             std::string& errormsg, dcled::AnimationList* list,
                             bool forceNoParseErrorException = false);

  bool anim_FileReader(const std::vector<std::string>& animationArgs,
                       const std::string& animationData,
                       std::string& errormsg, dcled::AnimationList* list,
                       bool forceNoParseErrorException = false);

  // Struct for animation type and corresponding parser function.
  struct TypeEntry {
    char type = 0;
    // Function takes a vector of arguments (while the first argument
    // normally equals the type.), a string of data (from after the =),
    // a reference to the errormsg, a pointer to the animation list
    // and a "force no parse error exception" boolean flag.
    bool (*func)(const std::vector<std::string>&, const std::string&,
                 std::string& errormsg, dcled::AnimationList*, bool) = nullptr;
  };

  // Animation type -> parser function array
  constexpr std::array<TypeEntry, 7> animationTypes = {{
    {'t', make_TextAnimation}, {'f', anim_FileReader}, {'c', make_ClockAnimation},
    {'I', make_InvertAnimation}, {'F', make_FlipAnimation}, {'n', make_NilAnimation},
    {'s', make_SetRectAnimation}
  }};

  // AnimationReader - custom parsing for args::ArgumentParser
  template<dcled::AnimationList* List>
  struct AnimationReader {
    void operator()(const std::string& name, const std::string& value, std::string& errormsg) {
      parseAnimationLine(value, errormsg, List);
    }
  };

  // Static list (template parameters need linkage...)
  dcled::AnimationList Animation_List_;
} // end anonymous namespace

namespace dcled { namespace cli // ****************************************************************
{
  dcled::AnimationList ArgParser::animationList()
  {
    return std::move(Animation_List_);
  }

  dcled::Device ArgParser::device()
  {
    if (!parsed_) return dcled::Device();
    if (path_.size()) return dcled::Device(path_, to_stdout_);
    return dcled::Device(to_stdout_);
  }

  void ArgParser::reset()
  {
    Animation_List_.clear();
    path_.clear();
    parsed_ = false;
    virtual_ = false;
    to_stdout_ = false;
  }

  ArgParser::State ArgParser::parse(int argc, char *argv[])
  {
    reset();

    args::ArgumentParser parser("Dream Cheeky LED Message Board driver.");
    args::CompletionFlag c(parser, {"complete"});
    args::HelpFlag help_arg(parser, "help", "Display this help menu", {'h', "help"});
    args::HelpFlag example_arg(parser, "example", "Display detailed examples", {'e', "examples"});
    args::HelpFlag version_arg(parser, "version", "Display the program version", {"version"});
    args::Flag list_arg(parser, "list", "List all dcled devices", {'l', "list"});
#ifndef WITHOUT_HIDAPI
    args::ValueFlag<std::string> path_arg(parser, "path", "USB device path", {'p', "path"});
    args::Flag stdout_arg(parser, "stdout", "Print device screen to stdout", {'s', "stdout"});
    args::Flag virtual_dev_arg(parser, "virtual", "Use only virtual stdout device\n"
                                                  "(includes --stdout)", {'v', "virtual"});
#else
    static bool virtual_dev_arg = true;
    static bool stdout_arg = true;
#endif
    args::Base emptyLine("");
    parser.Add(emptyLine);
    args::PositionalList<std::string, std::list, AnimationReader<&Animation_List_>>
            animations(parser, "ANIMATION", "List of animations with the format:\n" "[Type]["
            ANIMATION_ARG_DELIMITER_STR "Arg:Value...][=Data]");

    // Vector of description and boolean to indicate if this option is for the extended help list.
    std::vector<std::pair<args::NamedBase, bool>> animDescriptions = {
      { args::NamedBase("f : Read from File", "Read animation list from file\n"
                                              "  Data (required) : File path" ), false },
      { args::NamedBase("t : TextAnimation", "Scrolling text\n"
                                             "  Optional Arguments:\n"
                                             "   - ScrollSpeed 's' : ms (default=100)\n"
                                             "  Data (required) : Scroll text"), false },
      { args::NamedBase("c : ClockAnimation", "Show the current time\n"
                                              "  Optional Arguments:\n"
                                              "   - Duration 'd' : seconds (default=30)\n"
                                              "   - ColonBlink 'b' : 0 or 1 (default=1)\n"
                                              "   - Format 'f' : 12 or 24 (default=24)"), false },
      { args::NamedBase("s : SetRectAnimation", "Set screen LEDs on/off\n"
                                                 "  Required Arguments:\n"
                                                 "   - 'x' : LED in x pos\n"
                                                 "   - 'y' : LED in y pos\n"
                                                 "  Optional Arguments:\n"
                                                 "   - 'w' : width of rect (default = 1)\n"
                                                 "   - 'h' : height of rect (default = 1)\n"
                                                 "  Required Data: on/1 or off/0"), true},
      { args::NamedBase("n : NilAnimation", "Do nothing for\n"
                                            "  Optional Arguments:\n"
                                            "   - 't' : time in ms (default = 1000)"), true },
      { args::NamedBase("I : InvertAnimation", "Invert either screen or nested animation(s)\n"
                                               "  Optional Data: ANIMATION"), false },
      { args::NamedBase("F : FlipAnimation", "Flip either screen or nested animation(s)\n"
                                             "  Optional Arguments:\n"
                                             "   - Direction 'd' : h or v (default=h)\n"
                                             "  Optional Data: ANIMATION"), false },
    };

    for (auto& d : animDescriptions) {
      parser.Add(emptyLine);
      parser.Add(d.first);
    }

    try
    {
      parser.ParseCLI(argc, argv);
    }
    catch (args::Help flag)
    {
      if (flag.what() == std::string("version"))
      {
        print() << dcled::version_string();
        if (std::strcmp( dcled::version_branch(), "master"))
        { // Not a build from master, put out extra information:
          print() << "  - git-branch: " << dcled::version_branch();
          print() << "  - git-hash: " << dcled::version_fullhash();
        }
        // Show if we have a build from modified sources
        if (dcled::version_isdirty()) print() << "  - dirty-flag: " << dcled::version_isdirty();
      }
      else {
        std::cout << parser;
      }
      return State::EXIT;
    }
    catch (args::ParseError e)
    {
      error() << "ParseError: " << e.what();
      return State::ERROR;
    }
    catch (args::Completion &e)
    {
      std::cout << e.what();
      return State::EXIT;
    }
    catch (args::ValidationError e)
    {
      error() << "ValidationError: " << e.what();
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
            error() << "Animation parse errors:";
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
#ifndef WITHOUT_HIDAPI
    else if (path_arg) {
      path_ = path_arg.Get();
    }
#endif

    return State::OK;
  }
}} // end namespace dcled::cli

namespace // *****************************************************************
{
  // Pretty print the list of devices
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

  // Helper function to split strings with a given delimiter into a vector
  std::vector<std::string> split(const std::string& s, const std::string& delimiter,
                                 bool skipEmpty, size_t maxSplits)
  {
    std::vector<std::string> strings;
    size_t last = 0, next = 0, count = 0;
    while (((next = s.find(delimiter, last)) != std::string::npos)
            && (maxSplits == 0 || count < maxSplits))
    {
      auto substr = s.substr(last, next-last);
      if (!skipEmpty || substr.size()) {
        strings.push_back(std::move(substr));
        ++count;
      }
      last = next + 1;
    }
    auto substr = s.substr(last);
    if (!skipEmpty || substr.size()) {
      strings.push_back(std::move(substr));
    }
    return strings;
  }

  // Split only on the first occurance
  std::vector<std::string> split_first_one(const std::string& s, const std::string& delimiter,
                                           bool skipEmpty)
  {
    return split(s, delimiter, skipEmpty, 1);
  }

  // Generate a parse error
  void parseError(std::string msg, std::string& errormsg, bool forceNoParseErrorException)
  {
    #if ANIMATIONREADER_THROWS > 0
      if (!forceNoParseErrorException) throw args::ParseError(msg);
    #endif
    errormsg = std::move(msg);
  }

  bool make_InvertAnimation(const std::vector<std::string>& animationArgs,
                            const std::string& animationData,
                            std::string& errormsg, dcled::AnimationList* list,
                            bool forceNoParseErrorException)
  {
    const std::string animationName = "InvertAnimation";
    {
      const std::vector<std::string> validArgs = {}, requiredArgs;
      bool argError = false;
      const auto args = parseAnimationArgs(animationArgs, validArgs, requiredArgs, animationName,
                                           argError, errormsg, forceNoParseErrorException);
      if( argError ) return false;
    }

    if (!animationData.size()) {
      list->emplace_back(std::make_unique<dcled::InvertScreenAnimation>(0));
      return true;
    }

    dcled::AnimationList nestedList;
    if (!parseAnimationLine(animationData, errormsg, &nestedList, true))
    {
      parseError( animationName + ": '" + animationData + "': " + errormsg, errormsg);
      return false;
    }
    else
    {
      for (auto& a : nestedList) {
        list->emplace_back(std::make_unique<dcled::InvertAnimation>(std::move(a)));
      }
    }
    return true;
  }

  bool make_FlipAnimation(const std::vector<std::string>& animationArgs,
                          const std::string& animationData,
                          std::string& errormsg, dcled::AnimationList* list,
                          bool forceNoParseErrorException)
  {
    const std::string animationName = "FlipAnimation";
    const std::vector<std::string> validArgs = { "f", "d", "b" }, requiredArgs;

    bool argError = false;
    const auto args = parseAnimationArgs(animationArgs, validArgs, requiredArgs, animationName,
                                         argError, errormsg, forceNoParseErrorException);
    if( argError ) return false;

    dcled::Screen::Flip flip_direction = dcled::Screen::Flip::Horizontal;
    for (const auto& arg : args)
    {
      if (arg.first == "d") {
        if( arg.second.size() != 1 || !(arg.second[0] == 'h' || arg.second[0] == 'v') ) {
          parseError(std::string(animationName + ": Invalid value for argument '")
                                  + arg.first + "'.", errormsg, forceNoParseErrorException);
          return false;
        }
        if (arg.second[0] == 'v')
          flip_direction = dcled::Screen::Flip::Vertical;
      }
    }

    if (!animationData.size()) {
      list->emplace_back(std::make_unique<dcled::FlipScreenAnimation>(flip_direction, 0));
      return true;
    }

    dcled::AnimationList nestedList;
    if (!parseAnimationLine(animationData, errormsg, &nestedList, true))
    {
      parseError( animationName + ": '" + animationData + "': " + errormsg, errormsg);
      return false;
    }
    else
    {
      for (auto& a : nestedList) {
        list->emplace_back(std::make_unique<dcled::FlipAnimation>(std::move(a), flip_direction));
      }
    }
    return true;
  }

  // Try to create and append a ClockAnimation with the given arguments
  bool make_ClockAnimation(const std::vector<std::string>& animationArgs,
                           const std::string& animationData,
                           std::string& errormsg, dcled::AnimationList* list,
                           bool forceNoParseErrorException)
  {
    const std::string animationName = "ClockAnimation";
    const std::vector<std::string> validArgs = { "f", "d", "b" }, requiredArgs;

    bool argError = false;
    const auto args = parseAnimationArgs(animationArgs, validArgs, requiredArgs, animationName,
                                         argError, errormsg, forceNoParseErrorException);
    if( argError ) return false;

    // default values for arguments
    dcled::ClockAnimation::Mode mode = dcled::ClockAnimation::Mode::H24;
    uint32_t display_time_s = 30;
    bool blinking_colon = true;
    for (const auto& arg : args)
    {
      if (arg.first == "f") {
        const int mode_arg = std::atoi( arg.second.c_str() );
        if( mode_arg != static_cast<int>(dcled::ClockAnimation::Mode::H12)
            && mode_arg != static_cast<int>(dcled::ClockAnimation::Mode::H24) ) {
          parseError(std::string(animationName + ": Invalid value for '") + arg.first + "' (12 or 24)",
                                 errormsg, forceNoParseErrorException);
          return false;
        }
        mode = static_cast<dcled::ClockAnimation::Mode>(mode_arg);
      }
      else if (arg.first == "d") {
        try { display_time_s = std::stoul( arg.second ); }
        catch(...) {
          parseError(std::string(animationName + ": Invalid value for '") + arg.first + "'",
                                 errormsg, forceNoParseErrorException);
          return false;
        }
      }
      else if (arg.first == "b") {
        try{ blinking_colon = std::stoi( arg.second ); }
        catch(...) {
          parseError(std::string(animationName + ": Invalid value for '") + arg.first + "' (1 or 0)",
                                 errormsg, forceNoParseErrorException);
          return false;
        }
      }
    }
    list->emplace_back(std::make_unique<dcled::ClockAnimation>(display_time_s, blinking_colon, mode));
    return true;
  }

  // Try to create and append TextAnimation to the list with the given arguments and data
  bool make_TextAnimation(const std::vector<std::string>& animationArgs,
                          const std::string& animationData,
                          std::string& errormsg, dcled::AnimationList* list,
                          bool forceNoParseErrorException)
  {
    const std::string animationName = "TextAnimation";
    const std::vector<std::string> validArgs = { "s" }, requiredArgs;

    if (!animationData.size()) {
      parseError(animationName + ": Empty text data.", errormsg, forceNoParseErrorException);
      return false;
    }

    bool argError = false;
    const auto args = parseAnimationArgs(animationArgs, validArgs, requiredArgs, animationName,
                                         argError, errormsg, forceNoParseErrorException);
    if( argError ) return false;

    uint32_t scrollspeed = 100; // default value
    for (const auto& arg : args)
    {
      if (arg.first == "s") {
        scrollspeed = std::atol( arg.second.c_str() );
        if( scrollspeed == 0 ) {
          parseError(std::string(animationName + ": Invalid value for argument '")
                                  + arg.first + "'.", errormsg, forceNoParseErrorException);
          return false;
        }
      }
    }
    list->emplace_back(std::make_unique<dcled::TextAnimation>(animationData, scrollspeed));
    return true;
  }

  bool make_NilAnimation( const std::vector<std::string>& animationArgs,
                          const std::string& animationData,
                          std::string& errormsg, dcled::AnimationList* list,
                          bool forceNoParseErrorException)
  {
    const std::string animationName = "NilAnimation";
    const std::vector<std::string> validArgs = { "t" }, requiredArgs;

    bool argError = false;
    const auto args = parseAnimationArgs(animationArgs, validArgs, requiredArgs, animationName,
                                         argError, errormsg, forceNoParseErrorException);
    if( argError ) return false;

    uint32_t time_ms = 1000; // default value
    for (const auto& arg : args)
    {
      if (arg.first == "t") {
        time_ms = std::atol( arg.second.c_str() );
        if( time_ms == 0 ) {
          parseError(std::string(animationName + ": Invalid value for argument '")
                                  + arg.first + "'.", errormsg, forceNoParseErrorException);
          return false;
        }
      }
    }
    list->emplace_back(std::make_unique<dcled::NilAnimation>(time_ms));
    return true;
  }

  bool make_SetRectAnimation( const std::vector<std::string>& animationArgs,
                              const std::string& animationData,
                              std::string& errormsg, dcled::AnimationList* list,
                              bool forceNoParseErrorException)
  {
    const std::string animationName = "SetRectAnimation";
    const std::vector<std::string> validArgs = { "x", "y", "w", "h" }, requiredArgs = { "x", "y" };

    bool argError = false;
    const auto args = parseAnimationArgs(animationArgs, validArgs, requiredArgs, animationName,
                                         argError, errormsg, forceNoParseErrorException);
    if( argError ) return false;

    bool ledsOn = true;
    {
      std::string data = animationData;
      std::transform(animationData.begin(), animationData.end(), data.begin(), ::tolower);
      if (data == "on" ||  data == "1") {
        ledsOn = true;
      } else if (data == "off" ||  data == "0") {
        ledsOn = false;
      } else {
        parseError(animationName + ": Invalid or missing data.", errormsg, forceNoParseErrorException);
        return false;
      }
    }

    uint32_t x = 0, y = 0, w = 1, h = 1; // default values
    for (const auto& arg : args)
    {
      if (arg.first == "x") {
        try { x = std::stoul( arg.second ); }
        catch(...) {
          parseError(std::string(animationName + ": Invalid value for '") + arg.first + "'",
                                 errormsg, forceNoParseErrorException);
          return false;
        }
      }
      else if (arg.first == "y") {
        try { y = std::stoul( arg.second ); }
        catch(...) {
          parseError(std::string(animationName + ": Invalid value for '") + arg.first + "'",
                                 errormsg, forceNoParseErrorException);
          return false;
        }
      }
      else if (arg.first == "w") {
        try { w = std::stoul( arg.second ); }
        catch(...) {
          parseError(std::string(animationName + ": Invalid value for '") + arg.first + "'",
                                 errormsg, forceNoParseErrorException);
          return false;
        }
      }
      else if (arg.first == "h") {
        try { h = std::stoul( arg.second ); }
        catch(...) {
          parseError(std::string(animationName + ": Invalid value for '") + arg.first + "'",
                                 errormsg, forceNoParseErrorException);
          return false;
        }
      }
    }
    list->emplace_back(std::make_unique<dcled::SetRectAnimation>(ledsOn, x, y, w, h, 0));
    return true;
  }

  // Parse an animation description
  bool parseAnimationLine(const std::string& value, std::string& errormsg,
                          dcled::AnimationList* list, bool forceNoParseErrorException)
  {
    if (!value.size()) return false;
    const auto svalue = split_first_one(value, "=");
    // check if second character is either a '=' or a ANIMATION_ARG_DELIMITER_CHAR
    if (value.size() > 1 && value[1] != '=' && value[1] != ANIMATION_ARG_DELIMITER_CHAR ) {
      parseError(std::string("Invalid animation type '") + split(svalue[0], ANIMATION_ARG_DELIMITER_STR)[0] + "'",
                 errormsg, forceNoParseErrorException);
      return false;
    }
    // Check if the character is a valid animation type
    const auto it = std::find_if(animationTypes.cbegin(), animationTypes.cend(),
                                 [&value](const TypeEntry& tp){return tp.type == value[0];});

    if ( it == animationTypes.cend() ) {
      parseError(std::string("Invalid animation type '") + value[0] + "'",
                 errormsg, forceNoParseErrorException);
      return false;
    }
    // Call animation parser function from array.
    return it->func(split(svalue[0], ANIMATION_ARG_DELIMITER_STR), svalue.size() > 1 ? svalue[1] : std::string(),
                    errormsg, list, forceNoParseErrorException);
  }

  std::map<std::string, std::string> parseAnimationArgs(const std::vector<std::string>& animationArgs,
                                                        const std::vector<std::string>& validArgs,
                                                        const std::vector<std::string>& requiredArgs,
                                                        const std::string& animationName,
                                                        bool& errorOccured,
                                                        std::string& errormsg,
                                                        bool forceNoParseErrorException)
  {
    std::map<std::string, std::string> args;
    errorOccured = false;
    if (animationArgs.size() > 1) { // first argument is always animation identifier itself.
      for (auto it = ++animationArgs.cbegin(), end = animationArgs.cend(); it != end; ++it)
      {
        const auto arg = split_first_one(*it, ":", true);
        if (arg.size())
        {
          if (!contains(validArgs, arg[0])) {
            parseError(animationName + ": Unknown argument '" + arg[0] + "'.",
                       errormsg, forceNoParseErrorException);
            errorOccured = true;
            break;
          }
          if (arg.size() < 2) {
            parseError(animationName + ": Argument '" + arg[0] + "' without value.",
                       errormsg, forceNoParseErrorException);
            errorOccured = true;
            break;
          }
          args.emplace(std::make_pair(arg[0], (arg.size() < 2) ? std::string() : arg[1]));
        }
      }
    }
    for (const auto& rarg : requiredArgs) {
      if (!args[rarg].size()) {
        parseError(animationName + ": Required argument '" + rarg + "' missing.",
                   errormsg, forceNoParseErrorException);
        errorOccured = true;
        break;
      }
    }
    return args;
  }

  // Try to read animation descriptions from the given filename
  bool anim_FileReader(const std::vector<std::string>& animationArgs,
                       const std::string& animationData,
                       std::string& errormsg, dcled::AnimationList* list,
                       bool forceNoParseErrorException)
  {
    const std::string animationName = "Animation FileReader";
    const std::vector<std::string> validArgs = {};

    bool argError = false;
    const auto args = parseAnimationArgs(animationArgs, validArgs, validArgs, animationName,
                                         argError, errormsg, forceNoParseErrorException);
    if( argError ) return false;

    if (!animationData.size()) {
      parseError(animationName + ": Missing file name.", errormsg, forceNoParseErrorException);
      return false;
    }

    std::ifstream infile(animationData);
    if (!infile.is_open()) {
      parseError("Could not open file '" + animationData + "': " + std::strerror(errno),
                 errormsg, forceNoParseErrorException );
      return false;
    }

    size_t linecount = 0, animationCount = 0;
    auto pos = std::string::npos;
    for (std::string line; std::getline(infile, line); ++linecount)
    {
      // Skip empty lines, lines that only contain whitespaces and comment lines
      if (!line.size()) continue;
      if ((pos = line.find_first_not_of(" \t")) == std::string::npos) continue;
      if (line[pos] == '#' || line[pos] == ';') continue;

      if (!parseAnimationLine(line.substr(pos), errormsg, list, true))
      {
        parseError( animationName + ": '" + animationData + "' line "
                    + std::to_string(linecount) + ": " + errormsg, errormsg);
        return false;
      } else {
        ++animationCount;
      }
    }
    return true;
  }

} // end anonymous namespace
