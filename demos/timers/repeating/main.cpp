#include <fmt/printf.h>

#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>

static void timeout(const boost::system::error_code& ec,
                    boost::asio::steady_timer& timer, int& count) {
  if (count < 5) {
    fmt::println("Timer expired ({}). ec = {}", (count)++, ec.message());
    timer.expires_at(timer.expiry() + boost::asio::chrono::seconds(1));

    timer.async_wait([&timer, &count](const boost::system::error_code& ec) {
      timeout(ec, timer, count);
    });
  }
}

int main(int argc, char** argv) {
  int count{0};
  fmt::println("Waiting on timer experations asyncronously.");
  boost::asio::io_context io;
  boost::asio::steady_timer timer(io, boost::asio::chrono::seconds(1));
  timer.async_wait([&timer, &count](const boost::system::error_code& ec) {
    timeout(ec, timer, count);
  });

  io.run();

  return 0;
}
