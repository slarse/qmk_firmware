#! /usr/bin/env python

"""Script for perpetually sending data to a QMK keyboard."""

import contextlib
import sys
import typing as ty
import hid
import time
import subprocess

REPORT_LENGTH = 32

# Note: Requires udev rule to symlink the device to this path
# DOES NOT WORK; selects the wrong interface sometimes as the keyboard exposes TWO hidraw interfaces ...
HIDRAW_INTERFACE_PATH = b"/dev/hidraw_corne"

VENDOR_ID = 0x8D1D
PRODUCT_ID = 0x343A

USAGE_PAGE = 0xFF60
USAGE = 0x61


def main():
    device_interfaces = hid.enumerate(VENDOR_ID, PRODUCT_ID)
    raw_hid_interfaces = [
        i
        for i in device_interfaces
        if i["usage_page"] == USAGE_PAGE and i["usage"] == USAGE
    ]
    path = raw_hid_interfaces[0]["path"]

    while True:
        try:
            with hid.Device(path=path) as device:
                send_reports_forever(device)
        except hid.HIDException as e:
            print(e, file=sys.stderr)

        print("Trying to reconnect in 1 seconds", file=sys.stderr)
        time.sleep(1)


def send_reports_forever(device: hid.Device) -> None:
    while True:
        prev_cpu_times = get_cpu_times()
        time.sleep(5)
        cur_cpu_times = get_cpu_times()
        cpu_usage = compute_cpu_usage(cur_cpu_times, prev_cpu_times)
        ram_usage = get_ram_usage()
        signal_strength = get_signal_strength_perc()

        curtime = time.localtime()
        data = bytes(
            [
                curtime.tm_hour,
                curtime.tm_min,
                get_battery_perc(),
                cpu_usage,
                ram_usage,
                signal_strength,
            ]
        )

        send_raw_report(device, data)


@contextlib.contextmanager
def open_device(path: bytes) -> ty.Iterator[hid.Device]:
    device = hid.Device(path=path)

    try:
        yield device
    except Exception as e:
        print("Lost connection to device", file=sys.stderr)
        print(e, file=sys.stderr)
    finally:
        device.close()


def send_raw_report(device: hid.Device, data: bytes):
    if len(data) > REPORT_LENGTH:
        print("Too much data!")
        sys.exit(1)

    report_id = b"\x00"
    request_data = data + b"\x00" * (REPORT_LENGTH - len(data))
    request_report = report_id + request_data

    device.write(request_report)


def get_ram_usage() -> int:
    proc = subprocess.run("free", capture_output=True)
    mem_line = [
        line for line in proc.stdout.decode().split("\n") if line.startswith("Mem:")
    ][0]
    _, total, used, *_ = mem_line.split()
    print(total, used)
    return int(used) * 100 // int(total)


def get_cpu_times() -> list[int]:
    with open("/proc/stat", mode="r") as stat_file:
        cpu_stat_line = stat_file.readline()
        return [int(col) for col in cpu_stat_line.strip().split() if col != "cpu"]


def compute_cpu_usage(cur_cpu_times: list[int], prev_cpu_times: list[int]) -> int:
    """Compute CPU usage from stat file.

    CPU summary line format in stat file as follows:

        user – time spent in user mode
        nice – time spent processing nice processes in user mode
        system – time spent executing kernel code
        idle – time spent idle
        iowait – time spent waiting for I/O
        irq – time spent servicing interrupts
        softirq – time spent servicing software interrupts
        steal – time stolen from a virtual machine
        guest – time spent running a virtual CPU for a guest operating system
        guest_nice – time spent running a virtual CPU for a “niced” guest operating system
    """
    diff = [cur - prev for cur, prev in zip(cur_cpu_times, prev_cpu_times)]
    idle_time_fraction = diff[3] / sum(diff)
    return int((1 - idle_time_fraction) * 100)


def get_battery_perc() -> int:
    battery = subprocess.run(
        "upower -i /org/freedesktop/UPower/devices/battery_BAT0".split(),
        capture_output=True,
    )
    battery_data = [
        (key.strip(), value.strip())
        for line in battery.stdout.decode().split("\n")
        if len(line.strip().split()) == 2
        and (key := line.strip().split(":")[0])
        and (value := line.strip().split(":")[1])
        and key == "percentage"
    ]

    perc_data = 0
    if len(battery_data) > 0:
        _, perc = battery_data[0]
        perc = perc.strip("%")
        perc_data = int(perc)

    return perc_data


def get_signal_strength_perc() -> int:
    proc = subprocess.run(
        "iwctl station wlan0 show".split(),
        capture_output=True,
    )
    if proc.returncode != 0:
        return 0

    rssi_line, *_ = [
        line.strip()
        for line in proc.stdout.decode().split("\n")
        if line.strip().startswith("RSSI")
    ]
    rssi = int(rssi_line.split()[1])

    if rssi < -100:
        return 0
    elif rssi < -50:
        return 2 * (rssi + 100)
    else:
        return 100


if __name__ == "__main__":
    main()
