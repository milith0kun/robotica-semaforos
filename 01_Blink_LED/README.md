# Circuito 1: Prender y Apagar un LED (Blink)

## 1. Análisis Teórico y Circuital
El circuito consiste en el control digital de una carga de baja potencia (Diodo Emisor de Luz - LED) a través de un pin de entrada/salida de propósito general (GPIO) de un microcontrolador ATmega328P (Arduino UNO).

### Ley de Ohm y Cálculo de la Resistencia Limitadora:
Un LED polarizado en directa presenta una caída de tensión característica $V_f$ (Forward Voltage) y requiere limitar la corriente $I_f$ para evitar que se destruya por sobrecorriente o dañe el puerto del microcontrolador (máximo 40 mA por pin en ATmega328P, recomendado < 20 mA).

$$R = \frac{V_{cc} - V_f}{I_f}$$

Donde:
- $V_{cc} = 5.0\text{ V}$ (Nivel lógico HIGH de Arduino UNO)
- $V_f \approx 2.0\text{ V}$ (Caída de tensión típica para LED rojo)
- $I_f = 13.6\text{ mA} = 0.0136\text{ A}$ (Corriente óptima de brillo y seguridad)

$$R = \frac{5.0 - 2.0}{0.0136} \approx 220.58\ \Omega \implies R_{comercial} = 220\ \Omega$$

### Disipación de Potencia:
$$P = I_f^2 \cdot R = (0.0136)^2 \cdot 220 \approx 0.041\text{ W} \quad (\ll 0.25\text{ W de un resistor de 1/4W})$$

---

## 2. Diagrama Esquemático de Conexión
```text
  [Arduino UNO]
    Pin D8 ────(+) Ánodo [ LED Rojo ] Cátodo (-)────[ Resistor 220 Ω ]──── GND
```

---

## 3. Simulación en Wokwi
Este proyecto incluye los archivos:
- `01_Blink_LED.ino`: Código fuente para Arduino.
- `diagram.json`: Configuración visual del circuito en Wokwi.
- `wokwi-project.txt`: Marcador para la extensión Wokwi en VS Code.

Para simularlo:
1. Abre [Wokwi Arduino Uno Simulator](https://wokwi.com/projects/new/arduino-uno).
2. Pega el contenido de `01_Blink_LED.ino` y `diagram.json`.
3. Presiona el botón verde de "Play" (Iniciar simulación).
