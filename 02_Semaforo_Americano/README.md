# Circuito 2: Simulación de Semáforo Americano (MUTCD)

## 1. Análisis del Funcionamiento
El semáforo vehicular estadounidense sigue la norma **MUTCD (Manual on Uniform Traffic Control Devices)**.
A diferencia de los sistemas de algunos países europeos donde la fase roja se acompaña de ámbar antes de cambiar a verde (Rojo + Amarillo), el sistema americano utiliza una secuencia directa de 3 fases:

1. **Fase Verde**: El tráfico avanza con derecho de paso.
2. **Fase Amarillo (Ámbar)**: Intervalo de cambio/despeje que alerta a los conductores que la luz roja es inminente. Si el conductor está demasiado cerca para frenar con seguridad, cruza; si no, debe detenerse.
3. **Fase Rojo**: Detención obligatoria total de vehículos.

### Tabla de Estados
| Estado | LED Verde (D10) | LED Amarillo (D9) | LED Rojo (D8) | Duración Típica |
|:---:|:---:|:---:|:---:|:---:|
| **1. Tránsito Libre** | **HIGH (ON)** | LOW (OFF) | LOW (OFF) | 5.0 s |
| **2. Advertencia / Cambio** | LOW (OFF) | **HIGH (ON)** | LOW (OFF) | 2.0 s |
| **3. Alto Total** | LOW (OFF) | LOW (OFF) | **HIGH (ON)** | 5.0 s |

---

## 2. Esquema Eléctrico
- **Pin D10** -> Ánodo LED Verde -> Resistor 220 Ω -> GND
- **Pin D9** -> Ánodo LED Amarillo -> Resistor 220 Ω -> GND
- **Pin D8** -> Ánodo LED Rojo -> Resistor 220 Ω -> GND

---

## 3. Simulación
Abre los archivos `02_Semaforo_Americano.ino` y `diagram.json` en [Wokwi](https://wokwi.com/projects/new/arduino-uno) para probar la animación en tiempo real.
