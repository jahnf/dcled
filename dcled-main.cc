// dcled-hidapi - userland driver for the Dream Cheeky LED Message Board
// Copyright 2018 Jahn Fuchs <github.jahnf@wolke7.net>
// Distributed under the MIT License. See accompanying LICENSE file.

#include <signal.h>

#include "animations.h"
#include "dcled-cli.h"
#include "device.h"
#include "output.h"

namespace {
  void sighandler(int s) {
    dcled::Device::stopThreads(s);
  }
} // end anonymous namespace

int main(int argc, char* argv[])
{
  // Parse command line, print errors, print help ...
  auto& parser = dcled::cli::ArgParser::instance();
  const auto state = dcled::cli::ArgParser::instance().parse(argc, argv);
  if (state != dcled::cli::ArgParser::State::OK) {
    return static_cast<int>(state);
  }

  // Get device and check if it is in a valid 'open' state.
  dcled::Device dev = parser.device();
  if (!dev.isOpen()) {
    error() << "Error opening device" << " (" + dev.path() + ").";
    return 1;
  }

  // Add all animations from the command line parser to the device's animation queue.
  for( auto& a : dcled::cli::ArgParser::instance().animationList() ) {
    dev.enqueue(std::move(a));
  }

  // Install signal handler to be able to stop animation threads gracefully
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
    action.sa_handler = SIG_IGN; // Ignore pipe signal
    if( sigaction( SIGPIPE, &action, NULL ) < 0 ) /*signal install error*/ (void)NULL;
  }
  #endif

  // Play all the animations in the device's queue
  dev.playAll();
  return 0;
}
