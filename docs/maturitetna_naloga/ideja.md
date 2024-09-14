# Krmiljenje električnega motorja za rolete prek spletnega vmesnika

## Opis projekta
Projekt omogoča daljinsko upravljanje rolet prek spletnega vmesnika. S pomočjo ESP32 mikrokrmilnika in protokola MQTT lahko uporabniki nadzirajo rolete, nastavljajo urnike za njihovo delovanje in prejemajo obvestila o težavah. Sistem podpira brezžične posodobitve in beleženje vseh ukazov za kasnejšo analizo.

### Komponente
#### Obstoječe komponente
- **Roleta**: Fizična naprava, ki jo motor premika.
- **Električni motor**: Motor, ki dviga in spušča roleto.

#### Potrebne komponente za krmilnik
- **ESP32 mikrokrmilnik**: Upravljalna enota za krmiljenje rolet in komunikacijo z WiFi ter MQTT.
- **Logična vezja za PCB**: Povezava mikrokrmilnika z ostalimi komponentami.
- **Triak (BTA06)**: Krmiljenje vklopa in izklopa motorja.
- **Napajalnik**: Zagotavlja napajanje za ESP32 in motor.

#### Potrebne komponente za nadzor
- **MQTT posredniški strežnik (broker)**: Omogoča komunikacijo med krmilnikom in uporabnikom.
- **Strežnik za podatkovno bazo**: Shranjuje vse ukaze in beleži delovanje sistema.
- **Podatkovna baza**: Beleži zgodovino ukazov in stanja sistema.

### Krmilnik za rolete
- **WiFi provisioning prek BLE**: Prvotna konfiguracija povezave z omrežjem.
- **Povezava z WiFi in MQTT**: Povezava ESP32 na lokalno omrežje in komunikacija z EMQX strežnikom prek MQTT.
- **Krmiljenje motorja**: Premikanje rolet na podlagi prejetih ukazov.
- **OTA posodobitve**: Brezžične posodobitve programske opreme na ESP32.

### Spletna aplikacija
#### Tehnologije
- **Frontend**: Next.js, Tailwind CSS
- **Backend**: Node.js, Python, Telegraf, MySQL, InfluxDB

#### Funkcionalnosti
- **Upravljanje rolet**: Daljinsko dvigovanje in spuščanje rolet.
- **Prijava uporabnika**: Varna prijava v sistem.
- **Pregled naprav**: Informacije o stanju naprav v lasti uporabnika.
- **Zgodovina ukazov**: Pregled zgodovine upravljanja rolet.
- **Programiranje urnikov**: Nastavljanje urnikov za samodejno dvigovanje in spuščanje rolet.
- **Obvestila o napakah**: Obveščanje o težavah ali napakah sistema.
