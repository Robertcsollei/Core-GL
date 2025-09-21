#pragma once
#include <mutex>
#include <queue>

namespace terrakit::network {
template<typename T>
class ConcurrentQueue
{
public:
  void Push(T value)
  {
    std::lock_guard<std::mutex> lock(m_Mutex);
    m_Queue.push(std::move(value));
  }

  bool TryPop(T& out)
  {
    std::lock_guard<std::mutex> lock(m_Mutex);
    if (m_Queue.empty())
      return false;
    out = std::move(m_Queue.front());
    m_Queue.pop();
    return true;
  }

  bool empty() { return m_Queue.empty(); }

private:
  std::mutex m_Mutex;
  std::queue<T> m_Queue;
};
} // namespace terrakit::network