#include <codecvt>
#include <iostream>
#include <locale>
#include <string>

int main(int argc, char* argv[])
{
  std::wstring_convert<std::codecvt_utf8<wchar_t>,wchar_t> cv;
  std::wstring wstr =  L"z\u00df\u6c34\U0001f34c"; // or L"zß水🍌"
  std::cout << cv.to_bytes(wstr) << std::endl;
  return 0;
}
