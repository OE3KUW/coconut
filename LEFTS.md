# Technische Herkunft und Unterrichtszweck

Die in diesem Projekt aktive `ccn_*`-Implementierung stellt keine
unveränderte Kopie des ursprünglichen Quellcodes dar. Sie wurde als
eigenständige Portierung beziehungsweise Neuimplementierung für den ATmega328P
erstellt und an dessen Hardware, Zeitgeber, Interruptsystem und Speichergrenzen
angepasst. Sie kommt ohne das Arduino-Framework aus.

Architektur, Grundideen und historische Modulbezeichnungen gehen auf die
PAOLINA-/Philips-Arbeit zurück. Die historischen Module BPCI, EVTI, MBXI, QUEI
und TIMI bleiben davon getrennt im Repository erhalten und wurden für die
heutige AVR-Werkzeugkette angepasst, damit Herkunft und Entwicklung technisch
nachvollziehbar bleiben. Für diese historischen Bestandteile gilt der in
[RIGHTS.md](./RIGHTS.md) beschriebene ungeklärte Rechte- und Lizenzstatus.

Die ATmega328P-Fassung wurde für den Unterricht an Höheren Technischen
Lehranstalten (HTL) in den Fachbereichen Elektronik und Technische Informatik
erstellt. Sie soll Betriebssystemgrundlagen, kooperatives Scheduling, Events,
Mailboxen, Softwaretimer, Interruptbehandlung und hardwarenahe
Mikrocontrollerprogrammierung nachvollziehbar machen.

Diese Beschreibung des technischen und didaktischen Zwecks ist keine Lizenz
und gewährt keine zusätzlichen Nutzungsrechte.

## Technical origin and educational purpose

The active `ccn_*` implementation in this project is not an unchanged copy of
the original source code. It was created as an independent port or
reimplementation for the ATmega328P and adapted to its hardware, timers,
interrupt system and memory constraints. It does not use the Arduino framework.

The architecture, fundamental ideas and historical module names originate from
the PAOLINA/Philips work. The historical BPCI, EVTI, MBXI, QUEI and TIMI modules
are retained separately in the repository and were adapted to the current AVR
toolchain so that their provenance and development remain technically
traceable. Their unresolved rights and licensing status is described in
[RIGHTS.md](./RIGHTS.md).

The ATmega328P version was created for teaching at Austrian higher technical
colleges (HTL), particularly in electronics and technical computer science. It
is intended to illustrate operating-system fundamentals, cooperative
scheduling, events, mailboxes, software timers, interrupt handling and
low-level microcontroller programming.

This description of technical and educational purpose is not a licence and
does not grant additional rights of use.
