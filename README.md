<div align="center">

#  ChromaDock


[![Language](https://img.shields.io/badge/Language-C%2B%2B20-blue.svg?style=for-the-badge&logo=c%2B%2B)](https://en.wikipedia.org/wiki/C%2B%2B)
[![Platform](https://img.shields.io/badge/Platform-Windows%2010%20%7C%2011-0078D6.svg?style=for-the-badge&logo=windows)](https://www.microsoft.com/)
[![RAM](https://img.shields.io/badge/RAM%20Usage-~4%20MB-success.svg?style=for-the-badge)]()
[![License](https://img.shields.io/badge/License-MIT-purple.svg?style=for-the-badge)](LICENSE)

[**English**](#english) • [**Русский**](#russian)

</div>

---

<a name="english"></a>
## 🇬🇧 English

### ✨ Highlights
* **Chrome & Dark Glass**: Custom specular gradients mimicking molten mercury and multi-layer translucent dark glass (`UpdateLayeredWindow` per-pixel alpha).
* **Extreme Efficiency**: ~3.5–5 MB active RAM. When hidden, the window swapchain is disconnected from DWM and memory is purged to bare minimum.
* **Proximity Sensor**: Completely invisible on the desktop until your cursor approaches within ~4 cm, waking up with a soft glow.
* **Drag & Drop Everything**: Drop executables, `.lnk` shortcuts, Steam web links, or directories straight onto the dock.
* **Smart High-Res Icon Pipeline**: Pulls 256×256 Jumbo shell icons directly from the Windows Shell (`SHGetImageList`), auto-trims empty padding, and resamples with high-quality bicubic filtering while preserving the original aspect ratio.
* **Multi-Dock & 4 Directions**: Supports multiple independent dock windows simultaneously. Expand Down, Up, Right, or Left without coordinate shifting or flickering.


### 🛠️ Controls & Interaction
* **Hover near dock edge** — Proximity sensor wakes the liquid chrome handle.
* **Click handle** — Smoothly unfolds/folds the capsule.
* **Click icon** — Launches program or opens directory in File Explorer.
* **Right-click icon** — Remove item from dock.
* **Right-click dock/tray** — Context menu:
  * *Add Application / Add Folder*
  * *Expand Direction* (Down, Up, Right, Left)
  * *Icon Size* (+4 / -4)
  * *Dock Scale* (+10% / -10% / 100%)
  * *Always on Top* toggle
  * *Create Another Dock*
  * *Autostart with Windows*

---

<a name="russian"></a>
## 🇷🇺 Русский


### ✨ Ключевые возможности
* **Хром и тёмное стекло**: Зеркальные многоточечные градиенты и тёмное полупрозрачное стекло с попиксельным альфа-каналом (`UpdateLayeredWindow`).
* **Экстремальная оптимизация памяти**: ~3.5–5 МБ ОЗУ. В скрытом состоянии буфер окна отключается от DWM, а процесс сжимается до голого ядра.
* **Датчик приближения**: На рабочем столе нет лишнего шума — индикатор спит и плавно проявляется только тогда, когда курсор мыши подходит на ~4 см к кромке экрана.
* **Drag & Drop всего**: Перетаскивай мышью исполняемые файлы `.exe`, ярлыки `.lnk`, игры Steam или любые папки прямо на виджет.
* **Чёткие иконки**: Извлекает оригинальные 256×256 Jumbo-иконки из оболочки Windows (`SHGetImageList`), автоматически обрезает пустые поля и сглаживает методом `HighQualityBicubic` с сохранением пропорций.

### 🛠️ Управление
* **Подвести мышь к краю** — датчик приближения плавно зажигает палочку.
* **Клик по палочке** — раскрыть / свернуть капсулу.
* **Клик по иконке** — запустить программу или открыть папку в Проводнике.
* **ПКМ по иконке** — удалить элемент из дока.
* **ПКМ по доку / иконке в трее** — меню настроек:
  * *Добавить приложение / Добавить папку*
  * *Направление раскрытия* (Вниз, Вверх, Вправо, Влево)
  * *Размер иконок* (+4 / -4)
  * *Масштаб дока* (+10% / -10% / 100%)
  * *Поверх всех окон*
  * *Создать ещё один док*
  * *Автозапуск при старте Windows*

---

## 📄 License
This project is licensed under the [MIT License](LICENSE).