import machine, neopixel, random, time

# LED is on Pin 4
led = machine.Pin(4, machine.Pin.OUT)

# There are 214 LEDs
np = neopixel.NeoPixel(led, 214)

# Shows 5 different random colors for one second each
for i in range(5):
    r = random.getrandbits(5)
    g = random.getrandbits(5)
    b = random.getrandbits(5)
    np.fill((r, g, b))
    np.write()
    time.sleep(1)
