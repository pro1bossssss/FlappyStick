# FlappyStick 🐦

## English

Flappy Bird clone for the **M5StickC S3**, built with Arduino (M5Unified).
Real pixel sprites, tap-to-fly controls, high score saved to flash memory,
on-the-fly screen rotation.

### Features
- Real pixel sprites (wing-flap animation, tiled pipes, background)
- High score persists across reboots (NVS / Preferences)
- Screen orientation toggle with a short press of Button B
- Builds into a ready `.bin` via GitHub Actions — no Arduino IDE required

### Hardware
- M5StickC S3 (ESP32-S3)

### Build
Firmware builds automatically on every push to `main` — the finished
`.bin` appears under **Actions → latest run → Artifacts**.

Manual build via Arduino IDE:
1. Add Board Manager URL: `https://static-cdn.m5stack.com/resource/arduino/package_m5stack_index.json`
2. Install the **M5Unified** library
3. Select the StickC S3 board, compile and upload

### Controls
- **Button A** — jump / start / restart
- **Button B** — toggle screen orientation

### Assets
Graphics are adapted open-source pixel sprites in the Flappy Bird style.

### License
MIT — see [LICENSE](LICENSE). Free to use, modify, and distribute,
as long as authorship is credited.

---

## Русский

Клон Flappy Bird для **M5StickC S3**, написан на Arduino (M5Unified).
Настоящие пиксель-спрайты, полёт через тап по кнопке, рекорд сохраняется
во флеш-памяти, смена ориентации экрана на лету.

### Возможности
- Настоящие пиксель-спрайты (анимация взмаха крыла, тайловые трубы, фон)
- Рекорд сохраняется между перезагрузками (NVS / Preferences)
- Смена ориентации экрана короткой кнопкой B
- Собирается в готовый `.bin` через GitHub Actions — не нужен Arduino IDE

### Железо
- M5StickC S3 (ESP32-S3)

### Сборка
Прошивка собирается автоматически при пуше в `main` — готовый `.bin`
появляется во вкладке **Actions → последний run → Artifacts**.

Ручная сборка через Arduino IDE:
1. Установи Board Manager URL: `https://static-cdn.m5stack.com/resource/arduino/package_m5stack_index.json`
2. Установи библиотеку **M5Unified**
3. Выбери плату StickC S3, собери и залей

### Управление
- **Кнопка A** — прыжок / старт / рестарт
- **Кнопка B** — смена ориентации экрана

### Ассеты
Графика — доработанные open-source пиксель-спрайты в стиле Flappy Bird.

### Лицензия
MIT — см. [LICENSE](LICENSE). Можно свободно использовать, менять и
распространять, при условии сохранения указания авторства.

---

## Українська

Клон Flappy Bird для **M5StickC S3**, написаний на Arduino (M5Unified).
Справжні піксель-спрайти, політ через тап по кнопці, рекорд зберігається
у флеш-пам'яті, зміна орієнтації екрана на льоту.

### Можливості
- Справжні піксель-спрайти (анімація змаху крила, тайлові труби, фон)
- Рекорд зберігається між перезавантаженнями (NVS / Preferences)
- Зміна орієнтації екрана коротким натисканням кнопки B
- Збирається в готовий `.bin` через GitHub Actions — не потрібен Arduino IDE

### Залізо
- M5StickC S3 (ESP32-S3)

### Збірка
Прошивка збирається автоматично при пуші в `main` — готовий `.bin`
з'являється у вкладці **Actions → останній run → Artifacts**.

Ручна збірка через Arduino IDE:
1. Встанови Board Manager URL: `https://static-cdn.m5stack.com/resource/arduino/package_m5stack_index.json`
2. Встанови бібліотеку **M5Unified**
3. Обери плату StickC S3, зібери і залий

### Керування
- **Кнопка A** — стрибок / старт / рестарт
- **Кнопка B** — зміна орієнтації екрана

### Асети
Графіка — доопрацьовані open-source піксель-спрайти в стилі Flappy Bird.

### Ліцензія
MIT — див. [LICENSE](LICENSE). Можна вільно використовувати, змінювати
і розповсюджувати за умови збереження вказівки авторства.
