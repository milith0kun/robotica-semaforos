# Circuito 3: Semáforo Sincronizado para Carros y Peatones

## 1. Análisis de Sincronización y Seguridad
En cualquier sistema de semaforización urbana, la prioridad principal es la **seguridad del peatón y del conductor**. La lógica previene condiciones de colisión mediante interbloqueos temporales.

### Matriz de Transición de Estados
| Fase | Semáforo Autos (V / A / R) | Semáforo Peatones (R / V) | Duración | Descripción |
|:---:|:---:|:---:|:---:|:---|
| **1** | **VERDE** / off / off | **ROJO** / off | 5.0 s | Tránsito de vehículos activo |
| **2** | off / **AMARILLO** / off | **ROJO** / off | 2.0 s | Frenado de vehículos |
| **3** | off / off / **ROJO** | **ROJO** / off | 1.0 s | *All-Red*: Despeje completo |
| **4** | off / off / **ROJO** | off / **VERDE** | 4.0 s | Cruce peatonal habilitado |
| **5** | off / off / **ROJO** | off / **VERDE (Blink)** | 2.0 s | Advertencia final de cruce |
| **6** | off / off / **ROJO** | **ROJO** / off | 1.0 s | Transición previa a verde vehicular |

---

## 2. Conexiones
- **Semáforo Autos**:
  - D10: Verde vehicular -> Resistor 220 Ω -> GND
  - D9: Amarillo vehicular -> Resistor 220 Ω -> GND
  - D8: Rojo vehicular -> Resistor 220 Ω -> GND
- **Semáforo Peatones**:
  - D7: Rojo peatonal -> Resistor 220 Ω -> GND
  - D6: Verde peatonal -> Resistor 220 Ω -> GND

---

## 3. Simulación
Abre el proyecto en [Wokwi](https://wokwi.com/projects/new/arduino-uno) cargando `03_Semaforo_Vehicular_Peatonal.ino` y `diagram.json`.
