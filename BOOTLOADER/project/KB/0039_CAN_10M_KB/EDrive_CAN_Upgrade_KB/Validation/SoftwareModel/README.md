# KB_CAN_UpgradeTask software model

2026-10-19: 15 / 15 executable cases passed. See `results.json`, `execution.log`, `build.log`, and `source_hashes.json`.

`kb_can_upgrade.c`, `boot_core.c`, `boot_core.h`, `kb_can_upgrade.h`, `app_partition.h`, and `kb_customer_shutdown.c` are byte-for-byte copies of the delivered APP_KB sources. The test configuration differs only in `KB_UPGRADE_REQUEST_ADDR`, mapped to `request_words[4]`. The delivery shutdown callback is verified to have an empty body; it is copied for SHA256 evidence but replaced at link time by an instrumented test implementation in `model.c`. The stub declares the delivered 100 MHz system clock for the clock-consistency compile check. SHA256 is checked both before and after the test run; delivered source files are never written.

The native C executable runs each case in a separate process so all function-static state starts fresh. Assertions remain active independently of platform `NDEBUG`. CAN/TIMER2/register/IRQ/watchdog/reset calls are model stubs. `SysCtl_resetDevice()` uses longjmp to inspect the completed transition instead of entering the target's final infinite loop.

Passed scenarios:

- Valid ENTER_BOOT request fragmented into three CAN frames; BL_OK reply reconstructed from four outgoing frames, reply CRC verified. While the fourth frame remains pending, no reset and no RAM cookie are allowed. Reset occurs only after the last TX request clears.
- Caller INTM state retained when initially enabled and initially disabled, across initialization, RX and TX register-load critical sections. The final deliberate reset path disables interrupts as designed.
- INFO and DATA requests produce no entry response and no reset.
- Bad packet CRC, missing/out-of-order fragment, wrong CAN ID, extended frame and invalid DLC rejected.
- A 200 ms inter-frame gap remains accepted; 201 ms discards the partial packet, matching the actual strict `>` condition.
- A TX pending for 501 ms clears the send state without writing the RAM cookie or resetting; a fresh valid request subsequently completes successfully.
- An occupied RX object 31 or TX object 32 causes status 3 without any CAN object configuration writes; subsequent calls leave customer object state untouched.
- TIMER2 downward counter naturally wraps during a fragmented request; a 1500 us elapsed interval across zero still leads to a valid request and response.
- Model verifies Timer2 prescaler 99 at 100 MHz, full 32-bit period, interrupt disable, reply CAN object/ID/DLC, four RAM marker words, and twenty watchdog/delay calls before reset.
- The shutdown callback is called exactly once, only after all four response frames finish, before the reset path disables interrupts and before any RAM marker is written. Its entry INTM state matches the caller's original enabled/disabled state. Ignored commands, invalid input, occupied objects and TX timeout do not call it. The adapter is checked to contain no hardcoded PWM trip operation; the model no longer provides PWM register stubs, so reintroducing those accesses causes compilation to fail.

Boundary: this executes the real adapter and protocol C, not C28x instructions. It does not verify an actual oscillator, PLL, interrupt latency, IF hardware atomicity, CAN arbitration/ACK/error behavior, message-object buffering, PWM electrical outputs, real watchdog timing, physical reset retention, BOOT recognition, Flash erase/program/ECC or a complete firmware-download cycle. IF critical-section assertions prove the intended software calls are made with INTM disabled in the model, not real concurrent ISR timing. Timekeeping assumes Timer2 is dedicated and sampled before an entire 32-bit microsecond period is lost (about 71.6 minutes).

Re-run `run_model.py` with the bundled Python. Zig requires execution outside this session's restricted sandbox to discover its own executable path. All executable, compiler-cache and test output writes are confined to this directory.
