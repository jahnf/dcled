#ifndef DCLEDMESSAGEBOARD_H
#define DCLEDMESSAGEBOARD_H

#include <vector>
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
  class PixMap;
  class Screen {
  public:
    static constexpr uint8_t WIDTH = 21;
    static constexpr uint8_t HEIGHT = 7;

    enum class Brightness : uint8_t { Low = 0, Mid = 1, High = 2 };

    constexpr Screen(): msgs_{LedMsg(0), LedMsg(2), LedMsg(4), LedMsg(6)} {}
    Screen(const PixMap& pixmap, uint32_t x_offset = 0, uint32_t y_offset = 0);
    Screen(const Screen& other) = default;
    Screen(Screen&& other) = default;
    virtual ~Screen() = default;

    Screen& operator=(const Screen& other) = default;
    Screen& operator=(Screen&& other) = default;

    virtual Screen& operator&(const Screen& other);
    virtual Screen& operator|(const Screen& other);
    virtual Screen& operator^(const Screen& other);

    virtual Screen& invert();
    virtual Screen& setBrightness(Brightness brightness);
    virtual Screen& setAll(bool on);

    /// Set the LED state at \a x, \a y. This sets the internal data, but does not send
    /// it to the actual device. This is done either via \a draw() or done automatically
    /// in intervals by the display update thread if activated.
    virtual void set(uint8_t x, uint8_t y, bool on);

    /// Get the LED state at \a x, \a y.
    bool get(uint8_t x, uint8_t y) const;

  protected:
    struct LedMsg
    {
      constexpr LedMsg(uint8_t row) : row(row) {}
      inline const LedMsg& operator&=(const LedMsg& other);
      inline const LedMsg& operator|=(const LedMsg& other);
      inline const LedMsg& operator^=(const LedMsg& other);
      inline void invert();
      inline void setAll(bool on);
      uint8_t report_id = 0;
      Brightness brightness = Brightness::High;
      uint8_t row;
      uint8_t data[2][3] = {{0xff, 0xff, 0xff}, {0xff, 0xff, 0xff}}; // all LEDs off
    };
    LedMsg msgs_[4];
  };

  void dumpScreen(const Screen& s);

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

    /// Returns a list of all found dcled devices.
    static const std::list<DeviceInfo> list();

    /// Creates a device instance that opens the first dlced device found.
    Device();
    /// Creates a device instance and tries to open the device via \a device_path.
    Device(const std::string& device_path);

    Device(Device&& other);
    ~Device();

    /// Returns a reference to the Screen, that is sent to the device with \a draw() or by the
    /// updater thread.
    Screen& screen();

    /// Returns if the usb device was successfully opened.
    bool isOpen() const;
    /// Sent the screen to the device.
    void sendScreen();

    /// Unfortunately the DC LED Message Board only lights up the LED's for about 300ms after
    /// sending it to the device. To have a constant display you manually send the screen to
    /// the device at least every 300ms. The updater thread does this for you.
    void startUpdateThread();
    void stopUpdateThread();
    bool updateThreadRunning();

  private:
    struct Impl;
    std::unique_ptr<Impl> p_;
  };

  class PixMap {
  public:
    PixMap(uint32_t width, uint32_t height);
    PixMap(const Screen& screen);
    PixMap(PixMap&& other) = default;
    PixMap(const PixMap& other) = default;
    PixMap& operator=(PixMap&& other) = default;
    PixMap& operator=(const PixMap& other) = default;

    //TODO:
    static PixMap fromFile();
    //-----------

    inline const uint8_t& at(uint32_t x, uint32_t y) const {
      return pixels_.at(x + y * width_);
    }

    inline uint8_t& at(uint32_t x, uint32_t y) {
      return pixels_.at(x + y * width_);
    }

    inline uint32_t width() const { return width_; }
    inline uint32_t height() const { return height_; }

  private:
    uint32_t width_ = 0;
    uint32_t height_ = 0;
    std::vector<uint8_t> pixels_;
  };

} // end namespace dc

#endif // DCLEDMESSAGEBOARD_H
