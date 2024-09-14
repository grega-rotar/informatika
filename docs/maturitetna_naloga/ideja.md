# Krmiljenje električnega motorja za roleta preko spletnega vmesnika
## Opis projekta
### Komponente
- **Obstoječe komponente**:
    - roleta
    - električni motor
- **Potrebne komponente za krmilnik**
    - mikrokrmilnik
    - logični operatorji za PCB
    - triac (BTA06) za vklop / izkop elektromotorja
    - mikrokrmilnik (ESP32)
    - napajalnik
- **Potrebne komponente za nadzor**
    - MQTT posredniški (broker) strežnik
    - strežnik za podatkovno bazo in beleženje ukazov 
    - podatkovna baza za beleženje ukazov

### Krmilnik rolet
- WiFi provisoning (preko BLE)
- povezava z WiFi omrežjem
- povezava z EMQX (posredniški strežnik za MQTT)
- premikanje rolet na podlagi prejetega ukaza preko MQTT
- OTA Updates (brezžična posodobitev krmilnika)

### Spletna aplikacija
#### Tehnologije
- **Frontend**
    - Next.js
    - Tailwindcss
- **Backend**
    - Node.js
    - Python
    - Telegraf
    - MySQL
    - InfluxDB
#### Funkcionalnosti
- prijava uporabnika
- vpogled v naprave, ki si jih uporabnik lasti
- posodebitev naprave
- upravljanje rolet preko spletne aplikacije
- vpogled v zgodovino upravljanja rolet
- možnost programiranja rolet v naprej
    - npr. da se zjutraj ob 8h dvignejo in zvečer spustijo
- obveščanje uporabnika o morebitnih težavah / napakah s sistemom




