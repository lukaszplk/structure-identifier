#ifndef STRUCTURE_IDENTIFIER_IDENTIFIER_HPP
#define STRUCTURE_IDENTIFIER_IDENTIFIER_HPP

#include "testers.hpp"

#include <algorithm>
#include <istream>
#include <memory>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace structure_identifier {

// ============================================================================
// Structure Identifier - the main engine
// ============================================================================

template<typename T>
class StructureIdentifier {
public:
    /// Result of structure identification
    struct Result {
        enum class Status { 
            IMPOSSIBLE,   ///< No structure matches or invalid operation
            UNCERTAIN,    ///< Multiple structures match
            IDENTIFIED    ///< Exactly one structure matches
        };
        
        Status status;
        std::vector<std::string_view> matchingStructures;
        
        /// Get localized result message (Polish)
        [[nodiscard]] std::string getMessage() const {
            switch (status) {
                case Status::IMPOSSIBLE: return "niemozliwe";
                case Status::UNCERTAIN:  return "brak pewnosci";
                case Status::IDENTIFIED: 
                    return matchingStructures.empty() 
                        ? "niemozliwe" 
                        : std::string(matchingStructures[0]);
            }
            return "niemozliwe";
        }
        
        /// Check if a unique structure was identified
        [[nodiscard]] bool isIdentified() const {
            return status == Status::IDENTIFIED && !matchingStructures.empty();
        }
    };

    /// Register a structure type to test against
    template<typename TesterType>
    void registerStructure() {
        testers_.push_back(std::make_unique<TesterType>());
        isValid_.push_back(true);
    }

    /// Process a sequence of operations and identify the structure
    /// @param operations Vector of (operation_code, value) pairs
    ///        operation_code: 1 = push, 2 = pop
    [[nodiscard]] Result identify(const std::vector<std::pair<int, T>>& operations) {
        resetAll();
        
        bool hasInvalidOp = false;
        
        for (const auto& [op, value] : operations) {
            if (op == OP_PUSH) {
                pushAll(value);
            } 
            else if (op == OP_POP) {
                if (testers_[0]->empty()) {
                    hasInvalidOp = true;
                    continue;
                }
                testPopAll(value);
            }
        }
        
        return buildResult(hasInvalidOp);
    }
    
    /// Read operations from stream and identify the structure
    /// @param in Input stream
    /// @param numOps Number of operations to read
    [[nodiscard]] Result identifyFromStream(std::istream& in, int numOps) {
        std::vector<std::pair<int, T>> operations;
        operations.reserve(numOps);
        
        for (int i = 0; i < numOps; ++i) {
            int op;
            T value;
            in >> op >> value;
            operations.emplace_back(op, value);
        }
        
        return identify(operations);
    }
    
    /// Get the number of registered structure testers
    [[nodiscard]] std::size_t testerCount() const {
        return testers_.size();
    }

private:
    static constexpr int OP_PUSH = 1;
    static constexpr int OP_POP = 2;
    
    std::vector<std::unique_ptr<IStructureTester<T>>> testers_;
    std::vector<bool> isValid_;
    
    void resetAll() {
        for (auto& tester : testers_) {
            tester->reset();
        }
        std::fill(isValid_.begin(), isValid_.end(), true);
    }
    
    void pushAll(const T& value) {
        for (auto& tester : testers_) {
            tester->push(value);
        }
    }
    
    void testPopAll(const T& expected) {
        for (std::size_t i = 0; i < testers_.size(); ++i) {
            if (!testers_[i]->testPop(expected)) {
                isValid_[i] = false;
            }
        }
    }
    
    [[nodiscard]] Result buildResult(bool hasInvalidOp) const {
        std::vector<std::string_view> matches;
        
        for (std::size_t i = 0; i < testers_.size(); ++i) {
            if (isValid_[i]) {
                matches.push_back(testers_[i]->name());
            }
        }
        
        if (hasInvalidOp || matches.empty()) {
            return {Result::Status::IMPOSSIBLE, {}};
        }
        if (matches.size() > 1) {
            return {Result::Status::UNCERTAIN, std::move(matches)};
        }
        return {Result::Status::IDENTIFIED, std::move(matches)};
    }
};

// ============================================================================
// Factory function
// ============================================================================

/// Create identifier with default structure testers (stack, queue, priority queue)
[[nodiscard]] inline auto createDefaultIdentifier() {
    StructureIdentifier<int> identifier;
    
    identifier.registerStructure<StackTester<int>>();
    identifier.registerStructure<QueueTester<int>>();
    identifier.registerStructure<MaxPriorityQueueTester<int>>();
    
    return identifier;
}

/// Create identifier with all available structure testers
[[nodiscard]] inline auto createFullIdentifier() {
    StructureIdentifier<int> identifier;
    
    identifier.registerStructure<StackTester<int>>();
    identifier.registerStructure<QueueTester<int>>();
    identifier.registerStructure<MaxPriorityQueueTester<int>>();
    identifier.registerStructure<MinPriorityQueueTester<int>>();
    identifier.registerStructure<DequeFrontTester<int>>();
    
    return identifier;
}

} // namespace structure_identifier

#endif // STRUCTURE_IDENTIFIER_IDENTIFIER_HPP
