#include <structure_identifier/identifier.hpp>

#include <iostream>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    auto identifier = structure_identifier::createDefaultIdentifier();
    
    for (int numOps; std::cin >> numOps; ) {
        auto result = identifier.identifyFromStream(std::cin, numOps);
        std::cout << result.getMessage() << '\n';
    }
    
    return 0;
}
