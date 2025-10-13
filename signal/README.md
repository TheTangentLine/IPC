This example demonstrates process-to-process communication using **POSIX signals**.

### Roles:

1. `program1`: sender (prompts for a PID and sends a signal)
2. `program2`: receiver (prints a message whenever it receives the signal)

### What it does:

- `program1` calls `kill(pid, SIGTERM)` to send `SIGTERM (15)` to the specified process.
- `program2` registers a handler for `SIGTERM` and, instead of terminating, prints a line each time the signal is received.

### Notes and corrections:

- **Signals are generally one-way** and carry very limited information. In this example, the receiver does not obtain the sender's PID or payload. However, with `sigaction` and `SA_SIGINFO`, a handler can access the sender PID via `siginfo_t->si_pid` (not implemented here).
- `SIGTERM` is catchable and can be handled or ignored. `SIGKILL (9)` and `SIGSTOP` cannot be caught or ignored.
- You can only send signals to processes you own (or with sufficient privileges).

### Build (macOS/Linux):

```bash
g++ -o program2 program2.cpp
g++ -o program1 program1.cpp
```

### Run:

**1. Start the receiver in one terminal:**

```bash
./program2
```

It will print its Process ID (PID).

**2. In another terminal, run the sender and input the PID shown by `program2`:**

```bash
./program1
```

When prompted: `Input the process id: <PID_from_program2>`

You can customize the signal by changing the signal called in both program, refer to this [Link](https://www.ibm.com/docs/en/zos/3.1.0?topic=descriptions-kill-end-process-job-send-it-signal) for more documentation.

### Education-only example:

This code is for learning purposes and **not intended for production use**.
