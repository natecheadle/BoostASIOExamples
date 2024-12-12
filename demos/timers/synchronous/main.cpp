#include <fmt/printf.h>

#include <boost/asio.hpp>

int main(int argc, char** argv) {
  boost::asio::io_context io;
  boost::asio::steady_timer timer(io, boost::asio::chrono::seconds(2));
  fmt::println("Waiting timer synchronously.");

  timer.wait();
  fmt::println("Timer expired.");
  return 0;
}
