#include <fmt/printf.h>

#include <boost/asio.hpp>
#include <thread>

class ActiveObject {
  int m_Count{0};
  mutable boost::asio::static_thread_pool m_Pool{1};

 public:
  ActiveObject() = default;
  ~ActiveObject() { m_Pool.wait(); }

  void Increment(int value) {
    m_Pool.executor().execute([this, value] {
      m_Count += value;
      fmt::println("Count is {}", m_Count);
    });
  }

  void Decrement(int value) {
    m_Pool.executor().execute([this, value] {
      m_Count -= value;
      fmt::println("Count is {}", m_Count);
    });
  }
};

int main(int argc, char** argv) {
  ActiveObject obj;
  std::vector<std::jthread> threads;
  for (int i = 0; i < 2; ++i) {
    threads.emplace_back([&obj] {
      for (int i = 0; i < 10; ++i) {
        obj.Increment(i);
      }
    });
    threads.emplace_back([&obj] {
      for (int i = 0; i < 10; ++i) {
        obj.Decrement(i);
      }
    });
  }

  for (auto& thread : threads) {
    thread.join();
  }
}
