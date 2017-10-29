#include "dcledmessageboard.h"

#include <atomic>
#include <chrono>
#include <codecvt>
#include <iostream>
#include <locale>
#include <thread>
#include <hidapi/hidapi.h>

HidApi::HidApi() : res(hid_init()) {}

HidApi::~HidApi()
{
  hid_exit();
}

namespace {
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

    InternalScreen() : Screen(), modified(false) {}
    ~InternalScreen() = default;

    inline const auto& msgs() const { return msgs_; }

    inline Screen& operator&(const Screen& other) {
      modified = true; return Screen::operator&(other);
    }
    inline Screen& operator|(const Screen& other) {
      modified = true; return Screen::operator&(other);
    }
    inline Screen& operator^(const Screen& other) {
      modified = true; return Screen::operator&(other);
    }
    inline Screen& invert() {
      modified = true; return Screen::invert();
    }
    inline Screen& setBrightness(Brightness brightness) {
      modified = true; return Screen::setBrightness(brightness);
    }
    inline void set(uint8_t x, uint8_t y, bool on) {
      modified = true; Screen::set(x,y,on);
    }

    std::atomic<bool> modified;
  };
} // end anonymous namespace

void dcled::dumpScreen(const Screen& s)
{
  std::cout << "---" << std::endl;
  for(uint8_t y = 0; y < Screen::HEIGHT; ++y) {
    std::cout << "|";
    for(uint8_t x = 0; x < Screen::WIDTH; ++x) {
      std::cout << ( s.get(x,y) ? "*" : "." );
    }
    std::cout << std::endl;
  }
}

dcled::Screen::Screen(const PixMap& pixmap, uint32_t x, uint32_t y)
  : Screen()
{
  const auto x_max = std::min(x + Screen::WIDTH, x);
  const auto y_max = std::min(y + Screen::HEIGHT, y);
  for (auto screen_x = decltype(Screen::WIDTH){0}; x < x_max; ++x, ++screen_x) {
    for (auto screen_y = decltype(Screen::HEIGHT){0}; y < y_max; ++y, ++screen_y) {
      set(screen_x, screen_y, pixmap.at(x,y));
    }
  }
}

dcled::Screen& dcled::Screen::setBrightness(Brightness brightness)
{
  msgs_[0].brightness = brightness;
  msgs_[1].brightness = brightness;
  msgs_[2].brightness = brightness;
  msgs_[3].brightness = brightness;
  return *this;
}

dcled::Screen& dcled::Screen::setAll(bool on)
{
  msgs_[0].setAll(on);
  msgs_[1].setAll(on);
  msgs_[2].setAll(on);
  msgs_[3].setAll(on);
  return *this;
}

bool dcled::Screen::get(uint8_t x, uint8_t y) const
{
  if (y < HEIGHT && x < WIDTH)
    return !(msgs_[y/2].data[y%2][2-x/8] & (1 << x%8));

  return false;
}

void dcled::Screen::set(uint8_t x, uint8_t y, bool on)
{
  if (!(y < HEIGHT && x < WIDTH))
    return;

  if (on)
    msgs_[y/2].data[y%2][2-x/8] &= ~(1 << x%8);
  else
    msgs_[y/2].data[y%2][2-x/8] |= (1 << x%8);
}

dcled::Screen& dcled::Screen::operator|(const Screen& other)
{
  msgs_[0] |= other.msgs_[0];
  msgs_[1] |= other.msgs_[1];
  msgs_[2] |= other.msgs_[2];
  msgs_[3] |= other.msgs_[3];
  return *this;
}

dcled::Screen& dcled::Screen::operator&(const Screen& other)
{
  msgs_[0] &= other.msgs_[0];
  msgs_[1] &= other.msgs_[1];
  msgs_[2] &= other.msgs_[2];
  msgs_[3] &= other.msgs_[3];
  return *this;
}

dcled::Screen& dcled::Screen::operator^(const Screen& other)
{
  msgs_[0] ^= other.msgs_[0];
  msgs_[1] ^= other.msgs_[1];
  msgs_[2] ^= other.msgs_[2];
  msgs_[3] ^= other.msgs_[3];
  return *this;
}

dcled::Screen& dcled::Screen::invert()
{
  msgs_[0].invert();
  msgs_[1].invert();
  msgs_[2].invert();
  msgs_[3].invert();
  return *this;
}

void dcled::Screen::LedMsg::setAll(bool on)
{
  data[0][0] = data[0][1] = data[0][2] = !on;
  data[1][0] = data[1][1] = data[1][2] = !on;
}

const dcled::Screen::LedMsg& dcled::Screen::LedMsg::operator&=(const LedMsg& other)
{
  data[0][0] |= other.data[0][0];
  data[0][1] |= other.data[0][1];
  data[0][2] |= other.data[0][2];
  data[1][0] |= other.data[1][0];
  data[1][1] |= other.data[1][1];
  data[1][2] |= other.data[1][2];
  return *this;
}

const dcled::Screen::LedMsg& dcled::Screen::LedMsg::operator|=(const LedMsg& other)
{
  data[0][0] &= other.data[0][0];
  data[0][1] &= other.data[0][1];
  data[0][2] &= other.data[0][2];
  data[1][0] &= other.data[1][0];
  data[1][1] &= other.data[1][1];
  data[1][2] &= other.data[1][2];
  return *this;
}

const dcled::Screen::LedMsg& dcled::Screen::LedMsg::operator^=(const LedMsg& other)
{
  data[0][0] ^= ~other.data[0][0];
  data[0][1] ^= ~other.data[0][1];
  data[0][2] ^= ~other.data[0][2];
  data[1][0] ^= ~other.data[1][0];
  data[1][1] ^= ~other.data[1][1];
  data[1][2] ^= ~other.data[1][2];
  return *this;
}

void dcled::Screen::LedMsg::invert()
{
  data[0][0] = ~data[0][0];
  data[0][1] = ~data[0][1];
  data[0][2] = ~data[0][2];
  data[1][0] = ~data[1][0];
  data[1][1] = ~data[1][1];
  data[1][2] = ~data[1][2];
}

struct dcled::Device::Impl
{
  Impl()
    : handle(hid_open(VENDOR_ID, PRODUCT_ID, NULL)), threadStop(false), threadSet(false)
  {}

  Impl(const std::string usb_path)
    : handle(hid_open_path(usb_path.c_str())), threadStop(false), threadSet(false)
  {}

  ~Impl()
  {
    if (updaterThread.joinable()) {
      threadStop = true;
      updaterThread.join();
    }
    if (handle)
      hid_close(handle);
  }

  void updateLoop()
  {
    int count = 9;
    debug() << "Start thread: " << std::this_thread::get_id();
    while (!threadStop) {
      if (screen.modified || ++count > 9) {
        screen.modified = false; count = 0;
        sendToDevice();
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(25));
    }
    debug() << "Stop thread: " << std::this_thread::get_id();
  }

  void sendToDevice()
  {
    if (handle) {
      for (const auto& msg : screen.msgs() ) {
        hid_send_feature_report(handle, reinterpret_cast<const uint8_t*>(&msg), screen.LEDMSGSIZE );
      }
    }
  }

  hid_device* handle = nullptr;
  InternalScreen screen;
  std::thread updaterThread;
  std::atomic<bool> threadStop;
  std::atomic<bool> threadSet;
};

dcled::Device::Device()
  : p_(new Impl())
{
}

dcled::Device::Device(const std::string& device_path)
  : p_(new Impl(device_path))
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
  return p_->handle;
}

void dcled::Device::startUpdateThread()
{
  if (!p_->threadSet && !p_->updaterThread.joinable()) {
    p_->threadSet = true; p_->threadStop = false;
    p_->updaterThread = std::thread(&Impl::updateLoop, p_.get());
  }
}

void dcled::Device::stopUpdateThread()
{
  if (p_->threadSet && p_->updaterThread.joinable()) {
    p_->threadSet = false; p_->threadStop = true;
    p_->updaterThread.join();
  }
}

void dcled::Device::sendScreen()
{
  p_->sendToDevice();
}

dcled::PixMap::PixMap(uint32_t width, uint32_t height)
  : width_(width), height_(height), pixels_(width * height)
{}

dcled::PixMap::PixMap(const Screen& screen)
  : PixMap(Screen::WIDTH, Screen::HEIGHT)
{
  for (auto x = decltype(Screen::WIDTH){0}; x < Screen::WIDTH; ++x) {
    for (auto y = decltype(Screen::HEIGHT){0}; y < Screen::HEIGHT; ++y) {
      at(x,y) = screen.get(x,y);
    }
  }
}
