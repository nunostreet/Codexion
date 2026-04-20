*This project has been created as part of the 42 curriculum by nunostreet.*

## Description

**Codexion** simulates a group of coders sharing limited USB dongles in a circular co-working hub. Each coder is a POSIX thread that cycles through three phases: **compile → debug → refactor**. Compiling requires holding two dongles simultaneously. After release, each dongle is unavailable for a configurable cooldown period. The simulation ends when a coder burns out (fails to start compiling within `time_to_burnout` milliseconds) or when all coders have compiled enough times.

The project explores real-world concurrency problems: mutual exclusion, deadlock prevention, starvation avoidance, priority scheduling, and precise timing.

## Instructions

### Compilation

```bash
make
```

Compiles with `-Wall -Wextra -Werror -pthread` into the binary `codexion`.

### Usage

```
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug \
           time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

All times are in milliseconds. `scheduler` must be `fifo` or `edf`.

### Examples

```bash
# 4 coders, 800ms burnout, 200ms compile, 200ms debug, 200ms refactor,
# 5 compiles required, 50ms cooldown, FIFO scheduling
./codexion 4 800 200 200 200 5 50 fifo

# Same but with Earliest Deadline First scheduling
./codexion 4 800 200 200 200 5 50 edf

# 1 coder (immediately burns out — cannot compile with only one dongle)
./codexion 1 500 200 100 100 3 0 fifo
```

### Cleaning

```bash
make clean    # remove object files
make fclean   # remove objects and binary
make re       # full rebuild
```

## Blocking cases handled

### Deadlock prevention

Deadlock is prevented by imposing a **global dongle acquisition order**: each coder always requests the dongle with the lower `dongle_id` first. This breaks the circular-wait condition (one of Coffman's four necessary conditions), making deadlock impossible regardless of how many coders compete simultaneously.

### Starvation prevention

Even-numbered coders sleep 1 ms after the simulation start before competing for dongles. This staggers the initial burst of acquisition attempts, preventing the chain scenario where every coder holds its first dongle simultaneously and blocks its neighbour.

The FIFO scheduler guarantees each request is served in strict arrival order. Under EDF, the coder closest to burnout is always served first; combined with feasible timing parameters, no coder is starved.

### Cooldown handling

After a dongle is released, it is marked unavailable until `available_at = release_time + dongle_cooldown`. Waiting coders use `pthread_cond_timedwait` with a 1 ms wake interval and check `get_time_ms() >= available_at` before proceeding. This prevents a coder from immediately re-acquiring a dongle it just released.

### Precise burnout detection

A dedicated monitor thread checks every 1 ms whether `elapsed_ms(last_compile_start) >= time_to_burnout` for each active coder. `last_compile_start` is initialised with the actual simulation start time and updated atomically at the moment each compilation begins. The burnout log is printed within 10 ms of the actual event.

### Log serialisation

All output goes through `print_state`, which holds the `write` mutex for the duration of the `printf` call. This ensures no two log lines ever interleave on stdout, regardless of how many threads are active simultaneously.

## Thread synchronization mechanisms

### Mutexes (`pthread_mutex_t`)

Two global mutexes protect shared state:

- **`mutexes.write`** — serialises all stdout output via `print_state`.
- **`mutexes.state`** — protects coder fields read/written by multiple threads: `last_compile_start`, `compile_counter`, `finished`, `end_simulation`, `all_threads_ready`.

Each dongle also has its own **`dongle.mutex`** which protects the priority queue, `occupied`, and `available_at` fields. It is held only during the enqueue/dequeue operations and the condition variable wait, never across the full compilation phase.

**Race condition prevention example:** the monitor reads `last_compile_start` via `get_long` (which locks `state`), while the coder writes it via `set_long` (same lock). Neither can see a partial write.

### Condition variables (`pthread_cond_t`)

Each dongle has a `pthread_cond_t condition`. When a coder cannot acquire a dongle (not at the top of the priority heap, dongle occupied, or cooldown active), it calls `pthread_cond_timedwait`, releasing the dongle mutex and sleeping up to 1 ms. When a dongle is released, `pthread_cond_broadcast` wakes all waiters so the next eligible coder can proceed immediately.

**Thread-safe communication between coders and monitor:** the monitor sets `end_simulation` via `set_bool` (locked), and all coder threads check it via `get_bool` (locked) in their main loop and inside `request_dongle`. This guarantees that when the monitor signals the end, all threads see it and exit gracefully within one scheduling cycle.

### Priority heap

Each dongle owns a min-heap of `t_request` entries with a fixed capacity of 2, since each dongle is shared by exactly two adjacent coders. The heap comparator implements the chosen scheduler:

- **FIFO**: `priority = 0` for all; ordered by `ticket` (arrival sequence number).
- **EDF**: `priority = last_compile_start + time_to_burnout` (the coder's burnout deadline); tie-broken by `ticket`.

With at most 2 entries, push and pop reduce to a single comparison — O(1). A coder is granted the dongle only when its request is at the heap root, ensuring fair arbitration.

## Resources

### Concurrency and synchronization

- [POSIX Threads Programming — Blaise Barney, LLNL](https://hpc-tutorials.llnl.gov/posix/)
- [The Little Book of Semaphores — Allen B. Downey](https://greenteapress.com/wp/semaphores/)
- [Operating Systems: Three Easy Pieces — Arpaci-Dusseau](https://pages.cs.wisc.edu/~remzi/OSTEP/) (chapters on concurrency)
- `man pthread_mutex_init`, `man pthread_cond_timedwait`, `man gettimeofday`
- https://pages.cs.wisc.edu/~remzi/OSTEP/threads-locks.pdf
- https://man7.org/linux/man-pages/man3/timeval.3type.html

### Scheduling algorithms

- [Earliest Deadline First — Wikipedia](https://en.wikipedia.org/wiki/Earliest_deadline_first_scheduling)
- [Priority queue / Binary heap — Wikipedia](https://en.wikipedia.org/wiki/Binary_heap)

### AI usage

AI was used during this project for architectural guidance on the request/grant dongle mechanism and priority heap design, identifying edge cases (cooldown initialisation, `last_compile_start` race, single-coder burnout), and reviewing norminette and C89 compliance. All AI-generated content was reviewed, tested, and understood before inclusion.
