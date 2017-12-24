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
  class Screen;

  class Animation
  {
  public:
    virtual ~Animation() = default;
    //virtual bool update(Screen& screen, uint32_t timePassed) = 0;
    //virtual bool next(Screen& screen) = 0;
    /// Updates the screen with the next animation step if the end is not reached and return
    /// the time in ms that the updated screen should be shown. Returns 0 if the end of the
    /// animation was reached.
    virtual uint32_t step(Screen&) { return 0; }
    /// Resets the animation to the beginning.
    virtual void reset() = 0;
    virtual int get() = 0;
  };

  class ExitAnimation : public Animation
  {
  public:
    virtual ~ExitAnimation() = default;
    virtual uint32_t step(Screen&) override;
    virtual void reset() override { counter = 10; }
    virtual int get() override { return 32; }
  private:
    int counter = 10;
  };

  class ExitAnimation2 : public Animation
  {
  public:
    virtual ~ExitAnimation2() = default;
    virtual uint32_t step(Screen&) override;
    virtual void reset() override { counter= 0;}
    virtual int get() override { return 64; }
  private:
    int counter = 0;
  };


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
    /// Send the screen to the device.
    void update();

    void enqueue(std::unique_ptr<Animation>&& a);

    void playAll();

    /// Unfortunately the DC LED Message Board only lights up the LED's for about 300ms after
    /// sending it to the device - to be able to constantly diplay something you would have to
    /// update the device screen at least every 300ms - the updater thread does this for you.
    void startUpdateThread();
    void stopUpdateThread();
    bool updateThreadRunning();

  private:
    struct Impl;
    std::unique_ptr<Impl> p_;
  };
} // end namespace dcled

#endif // DEVICE_H
