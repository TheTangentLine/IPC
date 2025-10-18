Before talking about **System V Message Queues**, let’s quickly revisit the **queue data structure**. A queue is **FIFO** (First-In, First-Out): the earliest element that enters is the first one to leave.

<div align="center">
<pre>
← [ O1 ][ O2 ][ O3 ][ O4 ] 
</pre>
</div>

When you pop from a queue, you remove the front element. You **cannot** remove `O2` before removing `O1`. This property will matter when we analyze message delivery.

With **SysV** message queues, one process sends a message to the kernel using `msgsnd()`. Another process receives it with `msgrcv()`. The kernel stores the messages internally in a queue until some receiver removes them.

If you run the program and find that “messages don’t show up” in the other process, it’s usually not that sending failed. More commonly, **another receiver got there first** and removed the message from the queue. Because the queue is FIFO and **a receive operation consumes the message**, once one process receives it, the message is gone; other processes won’t see it. You can uncomment line 166, then re-compile the program again to see it further.

This problem also occurs when we try running multiple processes that attached to the same queue: they will compete to receive messages of a given `mtype`. Whichever process calls `msgrcv()` first (for that type) will get the message; the others will block, return `ENOMSG` with `IPC_NOWAIT`, or later get subsequent messages.

Because of this, using a single SysV queue as a **broadcast channel** (where every listener should get every message) is not ideal. SysV queues do not provide “multicast” delivery semantics. You need to design around this (e.g., one message per recipient, multiple queues, or a pub/sub system).
