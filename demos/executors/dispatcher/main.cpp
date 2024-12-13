#include <fmt/format.h>
#include <fmt/printf.h>

#include <boost/asio.hpp>
#include <functional>
#include <future>
#include <vector>

class Dispatcher {
  boost::asio::io_context m_Ctx;
  boost::asio::executor_work_guard<boost::asio::io_context::executor_type>
      m_WorkGuard;

 public:
  Dispatcher() : m_WorkGuard(boost::asio::make_work_guard(m_Ctx)) {}

  ~Dispatcher() { m_Ctx.stop(); }

  std::future<void> DispatchWork(std::function<void()> f) {
    std::packaged_task<void()> task(std::move(f));
    return boost::asio::post(m_Ctx, std::move(task));
  }

  std::unique_ptr<boost::asio::steady_timer> DispatchPeriodic(
      std::function<void()> f, boost::asio::chrono::milliseconds duration) {
    std::unique_ptr<boost::asio::steady_timer> timer{
        std::make_unique<boost::asio::steady_timer>(m_Ctx, duration)};

    timer->async_wait([timer = timer.get(), f = std::move(f),
                       duration](const boost::system::error_code& ec) mutable {
      timeout(ec, timer, std::move(f), duration);
    });

    return timer;
  }

  void Run() { m_Ctx.run(); }
  void Stop() { m_Ctx.stop(); }

 private:
  static void timeout(const boost::system::error_code& ec,
                      boost::asio::steady_timer* timer, std::function<void()> f,
                      boost::asio::chrono::milliseconds duration) {
    if (!ec.failed()) {
      f();
      timer->expires_at(timer->expiry() + duration);
      timer->async_wait([timer, f = std::move(f), duration](
                            const boost::system::error_code& ec) mutable {
        timeout(ec, timer, std::move(f), duration);
      });
    } else {
      fmt::println("Received error-code, {}", ec.message());
    }
  };
};

int main(int argc, char** argv) {
  fmt::println("Main thread id = {}",
               std::hash<std::thread::id>{}(std::this_thread::get_id()));
  Dispatcher dispatcher;

  auto task = std::async([&] {
    auto timer = dispatcher.DispatchPeriodic(
        [] {
          fmt::println(
              "Timer expired from thread id = {}",
              std::hash<std::thread::id>{}(std::this_thread::get_id()));
          return true;
        },
        boost::asio::chrono::milliseconds(250));

    for (int i = 0; i < 10; ++i) {
      auto job = dispatcher.DispatchWork([i]() {
        fmt::println("Hello from thread id = {}, count = {}",
                     std::hash<std::thread::id>{}(std::this_thread::get_id()),
                     i);
      });

      job.wait();
      if (i > 5) {
        timer.reset();
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    dispatcher.Stop();
  });

  dispatcher.Run();

  task.wait();
}
