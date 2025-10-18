Communication via **messages** is an efficient form of **IPC** (Inter-Process Communication).
In practice, three common, kernel-mediated mechanisms are used to pass messages between processes:

- Message queues
- Pipes (anonymous or named/FIFOs)
- Sockets (Unix domain or TCP/UDP)

To compare them fairly, this repo implements the same chat app named **MessengerCLI** on top of each mechanism. Users can create or join channels; only the underlying IPC “transport” changes. This makes it easy to observe the strengths and trade-offs of each approach.

1. [Message Queue](messages/messageQueue)
2. Pipe _(Coming soon)_
3. Socket _(Coming soon)_
