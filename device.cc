#include "device.h"

#include "animations.h"
#include "screen.h"

#include <atomic>
#include <chrono>
#include <codecvt>
#include <iostream>
#include <locale>
#include <thread>
#include <hidapi/hidapi.h>

// Linux only, TODO Windows alternative for isatty...
#include <unistd.h>

#include "moodycamel/blockingconcurrentqueue.h"
#include "moodycamel/concurrentqueue.h"

HidApi::HidApi() : res(hid_init()) {}

HidApi::~HidApi()
{
  hid_exit();
}

namespace {
  bool stdoutIsTty() {
    // TODO Windows alternative...
    return isatty(fileno(stdout));
  }

  struct debug
  {
    template<typename T>
    auto& operator<<(const T& a) const { return std::cout << a; }
    ~debug() { std::cout << std::endl; }
  };

  // Screen with modified flag for the internal implementation
  struct InternalScreen : public dcled::Screen
  {
    static constexpr size_t LEDMSGSIZE = sizeof(LedMsg);
    InternalScreen() = default;
    ~InternalScreen() = default;
    inline const auto& msgs() const { return msgs_; }
  };

} // end anonymous namespace


constexpr char dcled::Device::EMULATED_DEV_PATH[];

struct dcled::Device::Impl
{
  Impl(bool toStdout = false)
    : handle(hid_open(VENDOR_ID, PRODUCT_ID, NULL)), threadStopSignal(false),
      to_stdout(toStdout), stdout_is_tty(stdoutIsTty())
  {
    if (this->to_stdout && stdout_is_tty) {
      for (int i = 0; i < Screen::HEIGHT; ++i) std::cout << std::endl;
    }
  }

  Impl(const std::string usb_path, bool toStdout = false)
    : threadStopSignal(false), to_stdout(toStdout), stdout_is_tty(stdoutIsTty())
  {
    if (usb_path.compare( EMULATED_DEV_PATH ) != 0) {
      handle = hid_open_path(usb_path.c_str());
    } else {
      this->to_stdout = emulated_only = true;
    }

    if (this->to_stdout && stdout_is_tty) {
      for (int i = 0; i < Screen::HEIGHT; ++i) std::cout << std::endl;
    }
  }

  ~Impl()
  {
    // TODO check if an animation thread is running -> stop, join
    if (handle)
      hid_close(handle);
  }

  uint32_t sleepDuration(uint32_t& ms) const
  {
    if (ms < 250) { auto res = ms; ms = 0; return res; }
    if (ms < 350) { ms -= 175; return 175; }
    ms -= 250; return 250;
  }

  void animationThread()
  {
    //debug() << "started animation thread " << std::this_thread::get_id();
    std::unique_ptr<Animation> a;
    while (!threadStopSignal && animations.try_dequeue(a))
    {
      while( uint32_t animDurationMs = a->step(screen) ) {
        auto timeUntil = std::chrono::high_resolution_clock::now();
        while (!threadStopSignal && ( animDurationMs > 0 ))
        {
          screenToDevice();
          timeUntil += std::chrono::milliseconds(sleepDuration(animDurationMs));
          std::this_thread::sleep_until(timeUntil);
        }
      }
    }
    debug() << "stopping animation thread " << std::this_thread::get_id();
  }

  void terminalPosReset() {
    if (stdout_is_tty)
      std::cout << "\x1b[" << char(Screen::HEIGHT + 0x30) << "A";
    else
      std::cout << "\n";
  }

  void screenToDevice()
  {
    if (handle) {
      for (const auto& msg : screen.msgs() ) {
        hid_send_feature_report(handle, reinterpret_cast<const uint8_t*>(&msg), screen.LEDMSGSIZE );
      }
    }
    if (to_stdout) {
      terminalPosReset();
      screen.print(stdout_is_tty);
    }
  }

  hid_device* handle = nullptr;
  InternalScreen screen;
  moodycamel::ConcurrentQueue<std::unique_ptr<Animation>> animations;
  std::atomic<bool> threadStopSignal;
  bool to_stdout = false;
  bool stdout_is_tty = false;
  bool emulated_only = false;
};

dcled::Device::Device(bool toStdout)
  : p_(new Impl(toStdout))
{
}

dcled::Device::Device(const std::string& device_path, bool toStdout)
  : p_(new Impl(device_path, toStdout))
{
}

dcled::Device::Device(Device&& other)
  : p_(std::move(other.p_))
{
}

dcled::Device::~Device()
{
}

const std::list<dcled::DeviceInfo> dcled::Device::list()
{
  std::list<DeviceInfo> device_list;
  if (HidApi::init())
  {
    hid_device_info* devs = hid_enumerate(0x0, 0x0);
    for (hid_device_info* cur_dev = devs; cur_dev; cur_dev = cur_dev->next) {
      if (cur_dev->vendor_id == VENDOR_ID && cur_dev->product_id == PRODUCT_ID) {
        std::wstring_convert<std::codecvt_utf8<wchar_t>,wchar_t> cv;
        device_list.push_back( DeviceInfo{ VENDOR_ID, PRODUCT_ID, cur_dev->path,
                                           cv.to_bytes(cur_dev->serial_number),
                                           cv.to_bytes(cur_dev->manufacturer_string),
                                           cv.to_bytes(cur_dev->product_string) });
      }
    }
    hid_free_enumeration(devs);
  }
  return device_list;
}

dcled::Screen& dcled::Device::screen()
{
  return p_->screen;
}

bool dcled::Device::isOpen() const
{
  if( p_->emulated_only )
    return true;

  return p_->handle;
}

void dcled::Device::update()
{
  p_->screenToDevice();
}

void dcled::Device::enqueue(std::unique_ptr<Animation>&& a)
{
  if(a) p_->animations.enqueue(std::move(a));
}

void dcled::Device::playAll()
{
  auto t = std::thread(&Impl::animationThread, p_.get());
  t.join();
}



