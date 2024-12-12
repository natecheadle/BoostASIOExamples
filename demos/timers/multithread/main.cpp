#include <fmt/printf.h>

#include <boost/asio.hpp>
#include <boost/atomic.hpp>
#include <boost/bind/bind.hpp>

class MultiTimer {
  boost::asio::io_context m_Context;
  boost::asio::strand<boost::asio::io_context::executor_type> m_Strand;
  std::vector<std::jthread> m_Threads;
  int m_Count{0};
  const int m_MaxCount;

 public:
  MultiTimer(int threadCount)
      : m_Strand(boost::asio::make_strand(m_Context)),
        m_MaxCount(threadCount * 5) {
    for (int i = 0; i < threadCount; ++i) {
      m_Threads.emplace_back([this] {
        boost::asio::steady_timer timer(m_Context,
                                        boost::asio::chrono::seconds(1));
        timer.async_wait(boost::asio::bind_executor(
            m_Strand, [this, &timer](const boost::system::error_code& ec) {
              print(ec, timer);
            }));
        m_Context.run();
      });
    }
  }

  void join() {
    for (auto& thread : m_Threads) {
      thread.join();
    }
  }

 private:
  void print(const boost::system::error_code& ec,
             boost::asio::steady_timer& timer) {
    if (m_Count < m_MaxCount) {
      fmt::println("Timer expired ({}). ec = {}", m_Count++, ec.message());
      timer.expires_at(timer.expiry() + boost::asio::chrono::seconds(1));

      timer.async_wait(boost::asio::bind_executor(
          m_Strand, [this, &timer](const boost::system::error_code& ec) {
            print(ec, timer);
          }));
    }
  }
};

int main(int argc, char** argv) {
  MultiTimer timer(5);
  timer.join();
  return 0;
}
