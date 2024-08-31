# MicroPython Beispiele - lux-camp-badge-2024

## Übersicht

### [Simple](Simple/main.py)

Ein kleines Beispielprogramm welches alle Pixel mit einer zufälligen Farbe
leuchten lässt.

## Beispielprogramme ausführen

Flashe zuerst Micropython auf das Board. Diese Beispiele wurden mit Version v1.23.0
getested. Eine Anleitung zum flashen des ESP8266 findest unter folgendem Link:

> [MicroPython tutorial for ESP8266](https://docs.micropython.org/en/latest/esp8266/tutorial/intro.html#deploying-the-firmware).

Danach installiere das
[`mpremote`](https://docs.micropython.org/en/latest/reference/mpremote.html) Tool.
Damit kannst du die Beispielprogramme wie folgt auf das Board kopieren:

    mpremote cp main.py :.

Alternativ kannst du auch Snippets direkt in die serielle Konsole kopieren,
in dem du über `CTRL+E` in den Einfügemodus wechselst, dein Beispielprogramm
aus der Zwischenablage hineinkopierst, und dann die Eingabe mit `CTRL+D`
abschliesst. Die serielle Konsole öffnest du wie folgt:

    mpremote repl

