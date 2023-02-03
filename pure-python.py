import evdev

devices = [evdev.InputDevice(path) for path in evdev.list_devices()]
for device in devices:
    if device.name == 'N/A':
        break

# Attach to the joystick device
joystick = evdev.InputDevice(device.path)

# Print out all events
for event in joystick.read_loop():
    print(evdev.categorize(event))
