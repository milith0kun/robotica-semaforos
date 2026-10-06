# Documento de Análisis, Diseño y Simulación de Circuitos

Este documento contiene el fundamento técnico, teórico y circuital de los 4 proyectos implementados en esta práctica de Robótica y Sistemas Embebidos con microcontrolador ATmega328P (Arduino UNO).

---

## 1. Análisis Teórico Común

### 1.1 Polarización de Diodos Emisores de Luz (LED)
Un diodo emisor de luz (LED) es un dispositivo semiconductor de unión p-n que emite fotones mediante electroluminiscencia cuando se polariza directamente.

A diferencia de una carga resistiva lineal, la relación corriente-voltaje en un diodo es exponencial (Ecuación de Shockley). Para fines de diseño en sistemas embebidos, se modela con su caída de tensión en conducción directa ($V_f$) y una corriente de trabajo de diseño ($I_f$).

```text
       Vcc (+5V) ────►[ Resistor R ]────►| [ Diodo LED ] ────► GND
```

Aplicando la Ley de Voltajes de Kirchhoff (LVK):
$$V_{cc} - V_R - V_f = 0 \implies V_R = V_{cc} - V_f$$

Aplicando la Ley de Ohm:
$$I_f = \frac{V_R}{R} \implies R = \frac{V_{cc} - V_f}{I_f}$$

#### Parámetros según color de LED:
| Color LED | Caída $V_f$ típica | Corriente de diseño $I_f$ | Resistencia Calculada | Resistencia Comercial recomendada |
|:---|:---:|:---:|:---:|:---:|
| **Rojo** | 2.0 V | 13.6 mA | 220.5 Ω | **220 Ω** (1/4 W) |
| **Amarillo / Ámbar** | 2.1 V | 13.2 mA | 219.7 Ω | **220 Ω** (1/4 W) |
| **Verde** | 3.0 V (o 2.2V difuso) | 9.1 mA - 12.7 mA | 157 Ω - 220 Ω | **220 Ω** (1/4 W) |

### 1.2 Límites Eléctricos del Microcontrolador ATmega328P
- Tensión nominal de operación: **5.0 V**
- Corriente máxima por pin de E/S (DC Current per I/O Pin): **40 mA** (Límite absoluto, no recomendado de forma continua).
- Corriente continua recomendada por pin: **≤ 20 mA**.
- Corriente total combinada a través del puerto $V_{cc}$ o $GND$: **200 mA**.
Con las resistencias de 220 Ω seleccionadas, cada LED consume entre **9 mA y 14 mA**, operando en una zona óptima de alta visibilidad, bajo calentamiento y total seguridad para el microcontrolador.

---

## 2. Estudio de los 4 Circuitos

### Circuito 1: Prender y Apagar un LED (Blink)
- **Función**: Control binario elemental de un pin GPIO en modo `OUTPUT`.
- **Comportamiento**: Onda cuadrada de ciclo de trabajo del 50% ($T = 2\text{ s}$, $f = 0.5\text{ Hz}$).
- **Objetivo didáctico**: Validar el funcionamiento del compilador, la inicialización de puertos y los estados lógicos `HIGH` (5V) y `LOW` (0V).

### Circuito 2: Semáforo Americano (MUTCD)
- **Función**: Secuencia cíclica de tránsito vehicular estandarizada.
- **Secuencia**: Verde $\to$ Amarillo $\to$ Rojo $\to$ Verde.
- **Diferencia técnica con otros estándares**: En Gran Bretaña y Alemania se enciende rojo y ámbar simultáneamente antes del verde. En el estándar americano, el paso es directo de rojo a verde.
- **Tiempos simulados**: 5 s Verde, 2 s Amarillo, 5 s Rojo.

### Circuito 3: Semáforo Sincronizado para Carros y Personas
- **Función**: Coordinación entre dos grupos semafóricos independientes (3 luces vehiculares + 2 luces peatonales).
- **Interbloqueo de seguridad (Safety Interlock)**:
  - Nunca permitir verde vehicular con verde peatonal.
  - Intervalo *All-Red* de 1 segundo para despeje de la vía antes de autorizar el cruce peatonal y antes de restablecer el tránsito vehicular.
  - Parpadeo de fin de cruce en el verde peatonal para alertar que el tiempo concluye.

### Circuito 4: Semáforo a Demanda con Botón Peatonal
- **Función**: Automatización a demanda con interfaz de entrada humana.
- **Topología de entrada**: Resistencia interna *Pull-Up* (`INPUT_PULLUP`).
  - Pin flotante evitado: Cuando el botón está abierto, el pin lee `HIGH` estable.
  - Al pulsar, se conecta a `GND` generando un flanco de bajada (`LOW`).
- **Antirrebote (Debounce)**: Los pulsadores mecánicos rebotan elásticamente durante 5 a 20 ms al cerrarse. Se implementa un filtro temporal de 200 ms con `millis()`.
- **Arquitectura de Software**: Máquina de Estados Finitos (FSM) no bloqueante. Permite que el sistema evalúe continuamente la entrada del usuario sin congelarse con `delay()`.

---

## 3. Guía de Ejecución y Simulación en Wokwi
Cada carpeta contiene:
- El código fuente `.ino`.
- El archivo `diagram.json` con la disposición exacta de componentes, cables y colores.
- El archivo `wokwi-project.txt`.

Para simular cualquiera de los proyectos:
1. Dirígete a [Wokwi Arduino Uno Simulator](https://wokwi.com/projects/new/arduino-uno).
2. Pega el código del archivo `.ino` en la pestaña de código.
3. Haz clic en la pestaña `diagram.json` de Wokwi y reemplázala con el `diagram.json` del proyecto.
4. Presiona el botón verde de reproducción **Start the simulation**.
