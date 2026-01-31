#ifndef STRUCTURE_IDENTIFIER_TESTERS_HPP
#define STRUCTURE_IDENTIFIER_TESTERS_HPP

#include <deque>
#include <functional>
#include <queue>
#include <stack>
#include <string_view>
#include <vector>

namespace structure_identifier {

// ============================================================================
// Abstract base class for testable data structures
// ============================================================================

template<typename T>
class IStructureTester {
public:
    virtual ~IStructureTester() = default;
    
    virtual void push(const T& value) = 0;
    virtual T peek() const = 0;
    virtual void pop() = 0;
    virtual bool empty() const = 0;
    virtual void reset() = 0;
    
    [[nodiscard]] virtual std::string_view name() const = 0;
    
    /// Test if popping returns expected value (keeps structure in sync)
    [[nodiscard]] bool testPop(const T& expected) {
        if (empty()) return false;
        bool matches = (peek() == expected);
        pop();
        return matches;
    }
};

// ============================================================================
// Concrete structure testers
// ============================================================================

template<typename T>
class StackTester : public IStructureTester<T> {
    std::stack<T> data_;
public:
    void push(const T& value) override { data_.push(value); }
    T peek() const override { return data_.top(); }
    void pop() override { data_.pop(); }
    bool empty() const override { return data_.empty(); }
    void reset() override { data_ = std::stack<T>{}; }
    std::string_view name() const override { return "stos"; }
};

template<typename T>
class QueueTester : public IStructureTester<T> {
    std::queue<T> data_;
public:
    void push(const T& value) override { data_.push(value); }
    T peek() const override { return data_.front(); }
    void pop() override { data_.pop(); }
    bool empty() const override { return data_.empty(); }
    void reset() override { data_ = std::queue<T>{}; }
    std::string_view name() const override { return "kolejka"; }
};

template<typename T>
class MaxPriorityQueueTester : public IStructureTester<T> {
    std::priority_queue<T> data_;
public:
    void push(const T& value) override { data_.push(value); }
    T peek() const override { return data_.top(); }
    void pop() override { data_.pop(); }
    bool empty() const override { return data_.empty(); }
    void reset() override { data_ = std::priority_queue<T>{}; }
    std::string_view name() const override { return "kolejka priorytetowa"; }
};

template<typename T>
class MinPriorityQueueTester : public IStructureTester<T> {
    std::priority_queue<T, std::vector<T>, std::greater<T>> data_;
public:
    void push(const T& value) override { data_.push(value); }
    T peek() const override { return data_.top(); }
    void pop() override { data_.pop(); }
    bool empty() const override { return data_.empty(); }
    void reset() override { data_ = decltype(data_){}; }
    std::string_view name() const override { return "kolejka priorytetowa min"; }
};

template<typename T>
class DequeFrontTester : public IStructureTester<T> {
    std::deque<T> data_;
public:
    void push(const T& value) override { data_.push_back(value); }
    T peek() const override { return data_.front(); }
    void pop() override { data_.pop_front(); }
    bool empty() const override { return data_.empty(); }
    void reset() override { data_.clear(); }
    std::string_view name() const override { return "deque (FIFO)"; }
};

} // namespace structure_identifier

#endif // STRUCTURE_IDENTIFIER_TESTERS_HPP
