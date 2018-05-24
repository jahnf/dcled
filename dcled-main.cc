#include <iostream>

#include <signal.h>

#include "animations.h"
#include "dcled-cli.h"
#include "device.h"

namespace {
  struct error {
    template<typename T>
    auto& operator<<(const T& a) const { return std::cerr << a; }
    ~error() { std::cerr << std::endl; }
  };

  void sighandler(int s)
  {
    dcled::stopThreads(s);
  }
} // end anonymous namespace

static std::list<std::unique_ptr<dcled::Animation>> mylist;

int main(int argc, char* argv[])
{
  auto& parser = dcled::cli::ArgParser::instance();
  const auto state = dcled::cli::ArgParser::instance().parse(argc, argv);
  if (state != dcled::cli::ArgParser::State::OK) {
    return static_cast<int>(state);
  }

  dcled::Device dev = parser.device();
  if (!dev.isOpen()) {
    error() << "Error opening device" << " (" + dev.path() + ").";
    return 1;
  }

  for( auto& a : dcled::cli::ArgParser::instance().animationList() ) {
    dev.enqueue(std::move(a));
  }

  // Install signal handler to be able to stop threads gracefully
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

  dev.playAll();

  return 0;
}
