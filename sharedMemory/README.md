Shared Memory is one of the most standard ways to communicate between processes.

This example simulates a simple "treasure" game to demonstrate inter-process communication using System V shared memory:

1. **Producer**: hides the treasure and writes an initial message into shared memory (`sharedMemory/producer.cpp`).
2. **Consumer**: finds the treasure by writing a new message into the same shared memory segment (`sharedMemory/consumer.cpp`).

Quick start

- **Build**:

  - `g++ -std=c++11 -o producer producer.cpp`
  - `g++ -std=c++11 -o consumer consumer.cpp`

- **Run**:
  1. In one terminal:
     - `./producer`
     - Enter a numeric key (e.g., `1234`).
  2. In another terminal:
     - `./consumer`
     - Enter the same numeric key.

The producer monitors the shared memory and prints a message when the consumer updates it.
