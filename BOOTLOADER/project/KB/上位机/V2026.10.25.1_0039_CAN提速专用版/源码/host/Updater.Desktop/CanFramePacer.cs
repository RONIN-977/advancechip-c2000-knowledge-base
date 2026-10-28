using System.Diagnostics;
using System.Runtime.InteropServices;
using Microsoft.Win32.SafeHandles;

namespace Updater.Desktop;

// One relative wait after each transmit: delayed scheduling never causes catch-up bursts.
internal sealed class CanFramePacer : IDisposable
{
    internal const int DelayMilliseconds = 1;
    private TimerWaitHandle? timer;
    private bool disposed;
    internal string Mode { get; private set; } = "legacy-wait-fallback";

    internal CanFramePacer(bool forceFallback = false)
    {
        if (forceFallback) return;
        // CREATE_WAITABLE_TIMER_HIGH_RESOLUTION, auto reset; no system timer-resolution change.
        var handle = CreateWaitableTimerExW(IntPtr.Zero, null, 0x2, 0x001F0003);
        if (handle.IsInvalid)
        {
            handle.Dispose();
            return;
        }
        timer = new TimerWaitHandle(handle);
        Mode = "high-resolution-timer";
    }

    internal void Wait(CancellationToken cancellationToken)
    {
        ObjectDisposedException.ThrowIf(disposed, this);
        cancellationToken.ThrowIfCancellationRequested();
        long started = Stopwatch.GetTimestamp();
        if (timer is not null)
        {
            // Negative 100 ns units mean relative time, measured anew for every frame.
            long dueTime = -10000L * DelayMilliseconds;
            if (SetWaitableTimer(timer.SafeWaitHandle, ref dueTime, 0, IntPtr.Zero, IntPtr.Zero, false))
            {
                if (WaitHandle.WaitAny(new WaitHandle[] { timer, cancellationToken.WaitHandle }) == 1)
                    cancellationToken.ThrowIfCancellationRequested();
            }
            else
            {
                timer.Dispose();
                timer = null;
                Mode = "legacy-wait-fallback";
                WaitFallback(cancellationToken);
            }
        }
        else WaitFallback(cancellationToken);

        // Guard against an early wake without busy-waiting the entire interval.
        while (Stopwatch.GetElapsedTime(started).TotalMilliseconds < DelayMilliseconds)
        {
            cancellationToken.ThrowIfCancellationRequested();
            Thread.SpinWait(64);
        }
        cancellationToken.ThrowIfCancellationRequested();
    }

    private static void WaitFallback(CancellationToken cancellationToken)
    {
        if (cancellationToken.WaitHandle.WaitOne(DelayMilliseconds))
            cancellationToken.ThrowIfCancellationRequested();
    }

    public void Dispose()
    {
        if (disposed) return;
        disposed = true;
        timer?.Dispose();
        timer = null;
    }

    private sealed class TimerWaitHandle : WaitHandle
    {
        internal TimerWaitHandle(SafeWaitHandle handle) => SafeWaitHandle = handle;
    }

    [DllImport("kernel32.dll", CharSet = CharSet.Unicode, SetLastError = true)]
    private static extern SafeWaitHandle CreateWaitableTimerExW(
        IntPtr attributes, string? name, uint flags, uint desiredAccess);

    [DllImport("kernel32.dll", SetLastError = true)]
    [return: MarshalAs(UnmanagedType.Bool)]
    private static extern bool SetWaitableTimer(SafeWaitHandle timerHandle,
        ref long dueTime, int period, IntPtr completionRoutine, IntPtr completionArgument,
        [MarshalAs(UnmanagedType.Bool)] bool resume);
}
