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

Coders do not wait for cooldowns inside the priority queue. `wait_cooldowns` is called only after both dongles are already claimed (`occupied = TRUE`). This closes the race window where the previous holder could re-enqueue with a lower-sequence ticket and reclaim a dongle before the waiting coder's cooldown elapsed.

The FIFO scheduler guarantees each request is served in strict arrival order. Under EDF, the coder closest to burnout is always served first; combined with feasible timing parameters, no coder is starved.

### Cooldown handling

When a coder releases a dongle, `release_dongle` immediately sets `occupied = FALSE`, records `available_at = now + dongle_cooldown`, and broadcasts on the condition variable. Waiting coders claim the dongle as soon as it is unoccupied — no cooldown is waited inside the priority queue.

After claiming both dongles, `grab_dongles` calls `wait_cooldowns`, which reads `available_at` from each dongle under its mutex and sleeps for `max(D1.available_at, D2.available_at) − now`. Enforcing the cooldown after the grab ensures the previous holder cannot sneak back into the queue while a waiting coder is sleeping for a per-dongle cooldown.

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

Each dongle has a `pthread_cond_t condition`. When a coder cannot acquire a dongle (not at the top of the priority heap or dongle occupied), it calls `pthread_cond_wait`, atomically releasing the dongle mutex and sleeping. When a dongle is released, `pthread_cond_broadcast` wakes all waiters so the next eligible coder can proceed immediately.

**Thread-safe communication between coders and monitor:** the monitor sets `end_simulation` via `set_bool` (locked), and all coder threads check it via `get_bool` (locked) in their main loop and inside `request_dongle`. This guarantees that when the monitor signals the end, all threads see it and exit gracefully within one scheduling cycle.

### Priority heap

Each dongle owns a min-heap of `t_request` entries with a fixed capacity of 2, since each dongle is shared by exactly two adjacent coders. The heap comparator implements the chosen scheduler:

- **FIFO**: `priority = 0` for all; ordered by `ticket` (arrival sequence number).
- **EDF**: `priority = last_compile_start + time_to_burnout` (the coder's burnout deadline); tie-broken by `ticket`.

With at most 2 entries, push and pop reduce to a single comparison — O(1). A coder is granted the dongle only when its request is at the heap root, ensuring fair arbitration.

## Choosing safe timing parameters

A coder's full cycle, measured from the start of one compilation to the start of the next, is:

```
T_cycle = t_compile + t_cooldown + t_debug + t_refactor + W
```

where `W` is the time spent waiting for both dongles to become unoccupied, and `t_cooldown` is the single `wait_cooldowns` call (which waits for `max(D1_remaining, D2_remaining) ≤ t_cooldown`). Burnout occurs when `T_cycle > t_burnout`.

In the worst case a coder arrives just as both neighbours begin compiling and must wait for each to finish. Because `wait_cooldowns` takes the maximum of both dongles' remaining cooldowns rather than summing them, the even/odd distinction that existed before no longer applies:

```
W_max = 2×t_compile  (for any N ≥ 3)
```

Substituting:

```
t_burnout > 3×t_compile + t_cooldown + t_debug + t_refactor
```

Using the example `./codexion 5 1200 200 200 100 2 100 edf`:

```
3×200 + 100 + 200 + 100 = 1000 ms  →  t_burnout=1200 ms is safe (200 ms margin)
```

The monitor detects burnout a few milliseconds after the deadline because it runs in a polling loop, which accounts for the small overshoot visible in the log (e.g. `1020 ms` instead of exactly `1000 ms`).

## Resources

### Concurrency and synchronization

- [POSIX Threads Programming — Blaise Barney, LLNL](https://hpc-tutorials.llnl.gov/posix/)
- [Operating Systems: Three Easy Pieces — Arpaci-Dusseau](https://pages.cs.wisc.edu/~remzi/OSTEP/) (chapters on concurrency)
- `man pthread_mutex_init`, `man pthread_cond_wait`, `man gettimeofday`
- https://pages.cs.wisc.edu/~remzi/OSTEP/threads-locks.pdf
- https://man7.org/linux/man-pages/man3/timeval.3type.html
- [The Dining Philosophers in C: threads, race conditions and deadlocks](https://www.youtube.com/watch?v=zOpzGHwJ3MU&t=4906s)
- [Philosophers, 42 School Project. Dining Philosophers Project. C Implementation](https://www.youtube.com/watch?v=UGQsvVKwe90&t=2065s)

### Scheduling algorithms

- [Earliest Deadline First — Wikipedia](https://en.wikipedia.org/wiki/Earliest_deadline_first_scheduling)
- [Priority queue / Binary heap — Wikipedia](https://en.wikipedia.org/wiki/Binary_heap)

### AI usage

AI was used during this project for architectural guidance on the request/grant dongle mechanism and priority heap design, identifying edge cases (cooldown initialisation, `last_compile_start` race, single-coder burnout), and reviewing norminette and C89 compliance. All AI-generated content was reviewed, tested, and understood before inclusion.
