#include <fmt/printf.h>

#include <boost/asio.hpp>

int main(int argc, char** argv) {
  fmt::println("Waiting on timer asyncronously.");
  boost::asio::io_context io;
  boost::asio::steady_timer timer(io, boost::asio::chrono::seconds(2));
  timer.async_wait([](const boost::system::error_code& ec) {
    fmt::println("Timer expired. ec = {}", ec.message());
  });

  io.run();

  return 0;
}
