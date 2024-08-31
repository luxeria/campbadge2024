import machine, neopixel, random, time

# LED is on Pin 4
led = machine.Pin(4, machine.Pin.OUT)

# Boot Button is on Pin 0
btn = machine.Pin(0, machine.Pin.IN, machine.Pin.PULL_UP)

# There are 214 LEDs
np = neopixel.NeoPixel(led, 214)

# Shows random colors until the boot button is pressed
while btn.value():
    r = random.getrandbits(5)
    g = random.getrandbits(5)
    b = random.getrandbits(5)
    np.fill((r, g, b))
    np.write()
    time.sleep(1)
