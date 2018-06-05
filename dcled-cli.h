// dcled-hidapi - userland driver for the Dream Cheeky LED Message Board
// Copyright 2018 Jahn Fuchs <github.jahnf@wolke7.net>
// Distributed under the MIT License. See accompanying LICENSE file.

#ifndef DCLED_CLI_H
#define DCLED_CLI_H

#include <list>
#include <memory>
#include <string>

#include <device.h>

namespace dcled {
  class Animation;
  using AnimationList = std::list<std::unique_ptr<Animation>>;

  namespace cli {
    class ArgParser {
    public:
      enum class State : int { OK = -1, EXIT = 0, ERROR = 1 };

      static ArgParser& instance() {
        static ArgParser parser;
        volatile int dummy{};
        return parser;
      }

      /// Parse and print out error messages and help messages.
      State parse(int argc, char* argv[]);

      /// Returns a valid device after parse returns OK.
      Device device();

      /// Returns the list of animations after a successful parse;
      AnimationList animationList();

    private:
      ArgParser()= default;
      ~ArgParser()= default;
      ArgParser(const ArgParser&)= delete;
      ArgParser& operator=(const ArgParser&)= delete;

      void reset();
      std::string path_;
      bool parsed_ = false;
      bool virtual_ = false;
      bool to_stdout_ = false;
    };
  }
}



#endif // DCLED_CLI_H
