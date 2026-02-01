# structure-identifier

Identify data structures (stack, queue, priority queue) from push/pop operation sequences.

## Description

Given a sequence of push and pop operations with expected values, the program determines which data structure is being simulated:

- **Stack** (LIFO) - Last In, First Out
- **Queue** (FIFO) - First In, First Out  
- **Priority Queue** - Maximum element first

## Build

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

## Usage

```bash
./structure_identifier < input.txt
```

### Input Format

```
n                    # number of operations
op1 val1             # operation and value
op2 val2
...
```

- `1 value` - push value onto structure
- `2 value` - pop and expect this value

### Output

- `stos` - stack
- `kolejka` - queue
- `kolejka priorytetowa` - priority queue
- `brak pewnosci` - multiple structures match (uncertain)
- `niemozliwe` - no structure matches (impossible)

## Example

```
6
1 1
1 2
1 3
2 3
2 2
2 1
```

Output: `stos` (stack - LIFO behavior)

## Project Structure

```
structure-identifier/
├── CMakeLists.txt
├── include/structure_identifier/
│   ├── testers.hpp      # Data structure testers
│   └── identifier.hpp   # Main identification engine
├── src/main.cpp
└── examples/input.txt
```

## License

See [LICENSE](LICENSE) file.
