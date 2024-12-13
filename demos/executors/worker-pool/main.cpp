#include <fmt/format.h>
#include <fmt/printf.h>

#include <boost/asio.hpp>
#include <functional>
#include <future>
#include <mutex>
#include <vector>

struct Data {
  double Value1;
  float Value2;
  int Value3;
};

std::string format_as(const Data& value) {
  return fmt::format(".Value1 = {} .Value2 = {} .Value3 = {}", value.Value1,
                     value.Value2, value.Value3);
}

std::mutex OutputMutex;

class WorkerPool {
  mutable boost::asio::static_thread_pool m_Pool;

 public:
  WorkerPool(std::size_t threadCount) : m_Pool(threadCount) {}

  ~WorkerPool() { m_Pool.join(); }

  std::future<void> QueueWork(std::function<void()> f) {
    std::packaged_task<void()> task(std::move(f));
    return boost::asio::post(m_Pool, std::move(task));
  }
};

int main(int argc, char** argv) {
  WorkerPool workerPool(4);
  std::vector<std::future<void>> tasks;
  for (int i = 0; i < 20; ++i) {
    auto data =
        std::make_shared<Data>(Data{.Value1 = static_cast<double>(i),
                                    .Value2 = static_cast<float>(i) * 2.0f,
                                    .Value3 = i * 3});
    auto work = [d = std::move(data)] {
      // Simulate some computationally intensive task
      std::this_thread::sleep_for(std::chrono::seconds(1));

      // Now lock shared resource to report results
      std::unique_lock lock(OutputMutex);
      fmt::println("Completed task {}", *d);
    };
    tasks.push_back(workerPool.QueueWork(std::move(work)));
  }

  for (auto& task : tasks) {
    task.wait();
  }

  return 0;
}
