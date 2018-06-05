// dcled-hidapi - userland driver for the Dream Cheeky LED Message Board
// Copyright 2018 Jahn Fuchs <github.jahnf@wolke7.net>
// Distributed under the MIT License. See accompanying LICENSE file.

#ifndef DEVICE_H
#define DEVICE_H

#include <list>
#include <memory>

class HidApi {
public:
  static bool init() { static HidApi S; return S.res == 0; }

private:
  HidApi();
  ~HidApi();
  const int res;
};

namespace dcled
{
  class Animation;
  class Screen;

  struct DeviceInfo {
    const uint16_t vendor_id = 0;
    const uint16_t product_id = 0;
    const std::string path;
    const std::string serial_number;
    const std::string manufacturer;
    const std::string product;
  };

  class Device
  {
  public:
    static constexpr uint16_t VENDOR_ID = 0x1d34;
    static constexpr uint16_t PRODUCT_ID = 0x0013;
    static constexpr char EMULATED_DEV_PATH[] = { "/dev/stdout" };
    static constexpr char INVALID_DEV_PATH[] = { "/dev/null" };

    /// Returns a list of all found dcled devices.
    static const std::list<DeviceInfo> list();

    /// Creates a device instance that opens the first dlced device found.
    Device(bool toStdout = false);
    /// Creates a device instance and tries to open the device via \a device_path.
    Device(const std::string& device_path, bool toStdout = false);

    Device(Device&& other);
    ~Device();

    /// Returns a reference to the Screen, that is sent to the device with \a update() or by the
    /// updater thread.
    Screen& screen();

    /// Returns if the usb device was successfully opened.
    bool isOpen() const;

    /// Set internal flag to stop all running device animation threads.
    static void stopThreads(int s = 2);

    const std::string& path() const;

    /// Send the current screen to the device manually.
    void update();

    template<typename AnimType> void enqueue(const AnimType& a) {
      static_assert(std::is_base_of<Animation, AnimType>::value,
                    "AnimType must derive from Animation");
      enqueue_ptr(std::make_unique<AnimType>(a));
    }

    template<typename AnimType> void enqueue(const AnimType&& a) {
      static_assert(std::is_base_of<Animation, AnimType>::value,
                    "AnimType must derive from Animation");
      enqueue(std::make_unique<AnimType>(std::move(a)));
    }

    /// Takes ownership of the enqueued animation.
    template<typename AnimType> void enqueue(AnimType* a) {
      static_assert(std::is_base_of<Animation, AnimType>::value,
                    "AnimType must derive from Animation");
      enqueue(std::unique_ptr<AnimType>(a));
    }

    template<typename AnimType> void enqueue(std::unique_ptr<AnimType> a) {
      static_assert(std::is_base_of<Animation, AnimType>::value,
                    "AnimType must derive from Animation");
      enqueue_ptr(std::move(a));
    }

    /// Play all animations in the queue.
    void playAll();

  private:
    void enqueue_ptr(std::unique_ptr<Animation> a);
    struct Impl;
    std::unique_ptr<Impl> p_;
  };

} // end namespace dcled

#endif // DEVICE_H
