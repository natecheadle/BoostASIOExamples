#include <fmt/printf.h>

#include <array>
#include <boost/asio.hpp>
// Write data to the pipe
std::string_view msg = "Hello from the pipe!";
std::array<char, 64> out;
int count{0};

static void write_callback(const boost::system::error_code& error,
                    std::size_t bytes_transferred,
                    boost::asio::writable_pipe& pipe) {
  if (!error) {
    fmt::println("Wrote {} bytes to the pipe", bytes_transferred);
    if (count++ < 5) {
      pipe.async_write_some(boost::asio::buffer(msg),
                            [&pipe](const boost::system::error_code& error,
                                    std::size_t bytes_transferred) {
                              write_callback(error, bytes_transferred, pipe);
                            });
    } else {
      pipe.close();
    }
  } else {
    fmt::println("Error writing to pipe: {}", error.message());
  }
}

static void read_callback(const boost::system::error_code& error,
                   std::size_t bytes_transferred,
                   boost::asio::readable_pipe& pipe) {
  if (bytes_transferred > 0) {
    fmt::println("Read {} bytes to the pipe: {}", bytes_transferred,
                 std::string_view(out.data(), bytes_transferred));
  }
  if (!error) {
    pipe.async_read_some(boost::asio::buffer(out),
                         [&pipe](const boost::system::error_code& error,
                                 std::size_t bytes_transferred) {
                           read_callback(error, bytes_transferred, pipe);
                         });
  } else {
    fmt::println("Error reading from pipe: {}", error.message());
  }
}

int main() {
  boost::asio::io_context io_context;
  boost::asio::readable_pipe read_end(io_context);
  boost::asio::writable_pipe write_end(io_context);
  boost::asio::connect_pipe(read_end, write_end);

  write_end.async_write_some(
      boost::asio::buffer(msg),
      [&write_end](const boost::system::error_code& error,
                   std::size_t bytes_transferred) {
        write_callback(error, bytes_transferred, write_end);
      });
  read_end.async_read_some(boost::asio::buffer(out),
                           [&read_end](const boost::system::error_code& error,
                                       std::size_t bytes_transferred) {
                             read_callback(error, bytes_transferred, read_end);
                           });

  io_context.run();

  return 0;
}