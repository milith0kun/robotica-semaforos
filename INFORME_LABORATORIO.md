# INFORME DE LABORATORIO: ANÁLISIS, DISEÑO Y SIMULACIÓN DE SISTEMAS SEMAFÓRICOS

**Institución:** Universidad Nacional de San Antonio Abad del Cusco (UNSAAC)  
**Facultad:** Facultad de Ingeniería Eléctrica, Electrónica, Informática y Mecánica  
**Curso:** Robótica / Sistemas Embebidos  
**Estudiante:** Edmil Jampier Saire Bustamante  
**Código:** 174449  
**Fecha:** 5 de Octubre de 2026  
**Repositorio GitHub:** [https://github.com/milith0kun/robotica-semaforos](https://github.com/milith0kun/robotica-semaforos)  

---

## 📸 EVIDENCIA DEL MONTAJE EXPERIMENTAL REAL EN BANCO DE TRABAJO

A continuación se presenta la fotografía real del banco de pruebas y protoboard con el microcontrolador energizado vía interfaz USB, implementado en la estación de trabajo:

![Montaje real del microcontrolador en protoboard](docs/img/captura_real_laboratorio.png)
*Figura 1: Montaje experimental físico de la placa de desarrollo sobre protoboard, energizada y programada vía puerto serie USB.*

---

## RESUMEN
En este informe se presenta el análisis teórico, diseño circuital, diagramas de conexión eléctrica y codificación en C++/Arduino para la implementación de cuatro sistemas de control secuencial:
1. **Control básico de encendido y apagado de un LED (Blink).**
2. **Semáforo vehicular estándar americano (MUTCD).**
3. **Semáforo para tránsito vehicular y peatonal sincronizado.**
4. **Semáforo inteligente a demanda mediante pulsador peatonal y Máquina de Estados Finitos (FSM) no bloqueante.**

Cada módulo cuenta con su análisis mediante la Ley de Ohm, dimensionamiento de potencia, esquemático detallado y archivo de simulación nativo para Wokwi (`diagram.json`).

---

## 1. FUNDAMENTO TEÓRICO Y CÁLCULOS CIRCUITALES

### 1.1 Modelo del Diodo Emisor de Luz (LED) y Ley de Ohm
Un diodo LED emite luz cuando se polariza en sentido directo. Debido a que su resistencia interna en conducción directa es muy pequeña, es imperativo colocar una resistencia limitadora de corriente ($R$) en serie para evitar que se queme el LED o se destruya el pin del microcontrolador.

```text
       Pin GPIO (5V / 3.3V) ────►[ Resistor R ]────►| [ Diodo LED ] ────► GND
```

Aplicando la **Ley de Voltajes de Kirchhoff (LVK)**:
$$V_{CC} - V_R - V_f = 0 \implies V_R = V_{CC} - V_f$$

Por **Ley de Ohm**:
$$R = \frac{V_{CC} - V_f}{I_f}$$

#### Parámetros según el color del LED:
* **LED Rojo:** $V_f \approx 2.0\text{ V}$, $I_f \approx 13.6\text{ mA}$
  $$R = \frac{5.0\text{ V} - 2.0\text{ V}}{0.0136\text{ A}} \approx 220\ \Omega$$
* **LED Amarillo:** $V_f \approx 2.1\text{ V}$, $I_f \approx 13.2\text{ mA} \implies R \approx 220\ \Omega$
* **LED Verde:** $V_f \approx 2.4\text{ V} - 3.0\text{ V}$, $I_f \approx 10\text{ mA} - 12\text{ mA} \implies R \approx 220\ \Omega$

#### Cálculo de Potencia Disipada:
$$P_R = I_f^2 \cdot R = (0.0136\text{ A})^2 \cdot 220\ \Omega \approx 0.041\text{ W} = 41\text{ mW}$$
Una resistencia estándar comercial de carbón de $1/4\text{ W}$ ($250\text{ mW}$) opera holgadamente sin calentamiento.

---

## 2. DESARROLLO DE LOS CUATRO CIRCUITOS

---

### CIRCUITO 1: Prender y Apagar un LED (Blink)

#### 1. Objetivo y Análisis
Comprobar el funcionamiento del ciclo de reloj del microcontrolador y la conmutación digital binaria en un pin de salida (`OUTPUT`). El LED conmuta a una frecuencia de $0.5\text{ Hz}$ ($1000\text{ ms}$ encendido, $1000\text{ ms}$ apagado).

#### 2. Esquemático Eléctrico
```text
  [Microcontrolador]
       Pin D8 ────────(+) Ánodo [ LED Rojo ] Cátodo (-)────────[ R = 220 Ω ]──────── GND
```

#### 3. Código Fuente Arduino (`01_Blink_LED.ino`)
```cpp
const uint8_t PIN_LED = 8;
const unsigned long TIEMPO = 1000;

void setup() {
  Serial.begin(9600);
  pinMode(PIN_LED, OUTPUT);
}

void loop() {
  digitalWrite(PIN_LED, HIGH); // LED ON
  Serial.println(F("[ESTADO] LED ENCENDIDO"));
  delay(TIEMPO);

  digitalWrite(PIN_LED, LOW);  // LED OFF
  Serial.println(F("[ESTADO] LED APAGADO"));
  delay(TIEMPO);
}
```

---

### CIRCUITO 2: Semáforo Americano (MUTCD)

#### 1. Objetivo y Análisis
Simular el ciclo estándar de un semáforo vehicular en Estados Unidos según la norma **MUTCD (Manual on Uniform Traffic Control Devices)**.
* **Fase 1 (Verde - 5s):** Flujo vehicular continuo.
* **Fase 2 (Amarillo - 2s):** Precaución y despeje de intersección.
* **Fase 3 (Rojo - 5s):** Detención absoluta.
* *Nota:* No existe fase simultánea de Rojo+Amarillo al arrancar (secuencia limpia Verde $\to$ Amarillo $\to$ Rojo $\to$ Verde).

#### 2. Esquemático Eléctrico
```text
  [Microcontrolador]
       Pin D10 ─────(+) Ánodo [ LED Verde ] Cátodo (-)────[ R = 220 Ω ]──── GND
       Pin D9  ─────(+) Ánodo [ LED Amarillo ] Cátodo (-)─[ R = 220 Ω ]──── GND
       Pin D8  ─────(+) Ánodo [ LED Rojo ] Cátodo (-)─────[ R = 220 Ω ]──── GND
```

#### 3. Tabla de Estados
| Estado | Luz Verde (D10) | Luz Amarilla (D9) | Luz Roja (D8) | Duración |
|:---:|:---:|:---:|:---:|:---:|
| **1. Tráfico Libre** | HIGH | LOW | LOW | 5.0 s |
| **2. Advertencia** | LOW | HIGH | LOW | 2.0 s |
| **3. Alto Total** | LOW | LOW | HIGH | 5.0 s |

---

### CIRCUITO 3: Semáforo para Carros y Personas Sincronizado

#### 1. Objetivo y Reglas de Seguridad Crítica
1. **Exclusión Mutua:** Nunca deben encenderse simultáneamente el verde vehicular y el verde peatonal.
2. **Intervalo All-Red (Todo en Rojo - 1s):** Permite evacuar la intersección de vehículos antes de dar paso seguro a peatones.
3. **Advertencia de Fin de Cruce Peatonal:** Durante los últimos 2 segundos del tiempo peatonal, el LED verde peatonal parpadea 4 veces indicando que se debe terminar de cruzar.

#### 2. Asignación de Pines
* **Vehicular:** Pin 10 (Verde), Pin 9 (Amarillo), Pin 8 (Rojo)
* **Peatonal:** Pin 7 (Rojo Peatonal), Pin 6 (Verde Peatonal)
* Todos con resistencias limitadoras de $220\ \Omega$ a `GND`.

#### 3. Matriz de Fases
| Fase | Autos (V / A / R) | Peatones (R / V) | Duración | Acción |
|:---:|:---:|:---:|:---:|:---|
| **1** | **ON** / off / off | **ON** / off | 5.0 s | Autos avanzan, peatones esperan |
| **2** | off / **ON** / off | **ON** / off | 2.0 s | Autos frenan, peatones esperan |
| **3** | off / off / **ON** | **ON** / off | 1.0 s | *All-Red:* Despeje de vía |
| **4** | off / off / **ON** | off / **ON** | 4.0 s | Peatones cruzan con seguridad |
| **5** | off / off / **ON** | off / **Blink** | 2.0 s | Peatones terminan cruce |
| **6** | off / off / **ON** | **ON** / off | 1.0 s | Transición previa a verde vehicular |

---

### CIRCUITO 4: Semáforo Inteligente a Demanda con Botón Peatonal

#### 1. Arquitectura por Máquina de Estados Finitos (FSM) No Bloqueante
A diferencia de los circuitos secuenciales con `delay()`, este circuito opera de forma reactiva y no bloqueante mediante la función del temporizador interno `millis()`:
* **Estado Reposo:** Mantiene el semáforo vehicular en VERDE y el peatonal en ROJO de forma permanente mientras nadie solicite cruzar.
* **Entrada del Botón (`D2` con `INPUT_PULLUP`):** Conectado directo a `GND`. Al presionar produce un nivel lógico `LOW`.
* **Filtro Antirrebote (Debounce):** Ventana de $200\text{ ms}$ para eliminar ruido por rebote mecánico de contactos.
* **Tiempo Mínimo Garantizado:** Asegura al menos $4\text{ s}$ continuos de verde vehicular para evitar congestión por pulsaciones consecutivas.

#### 2. Esquemático Eléctrico
```text
  [Microcontrolador]
       Pin D2 ────────[ Pulsador Normalmente Abierto ]──────── GND (Pull-Up interno)
       Pin D10 ───────(+) Ánodo [ LED Verde Auto ] ───[ 220 Ω ]─── GND
       Pin D9  ───────(+) Ánodo [ LED Amarillo Auto ] ─[ 220 Ω ]─── GND
       Pin D8  ───────(+) Ánodo [ LED Rojo Auto ] ─────[ 220 Ω ]─── GND
       Pin D7  ───────(+) Ánodo [ LED Rojo Peatón ] ──[ 220 Ω ]─── GND
       Pin D6  ───────(+) Ánodo [ LED Verde Peatón ] ─[ 220 Ω ]─── GND
```

#### 3. Diagrama de Transición de Estados
```text
  [ESTADO_REPOSO_AUTO_VERDE] 
             │ (Botón presionado y t_verde >= 4 s)
             ▼
  [ESTADO_AUTO_AMARILLO] (2 s)
             ▼
  [ESTADO_SEGURIDAD_ALL_RED] (1 s)
             ▼
  [ESTADO_PEATON_VERDE] (5 s)
             ▼
  [ESTADO_PEATON_AVISO_PARPADEO] (2.4 s)
             ▼
  [ESTADO_SEGURIDAD_ALL_RED] (1 s)
             ▼
  (Retorno a ESTADO_REPOSO_AUTO_VERDE)
```

---

## 3. GUÍA PARA REPRODUCIR LA SIMULACIÓN EN WOKWI

Cada carpeta del proyecto incluye su archivo `diagram.json` con la lista de componentes y ruteo de cables virtuales listos para cargar:

1. Ingresa al simulador web [https://wokwi.com/projects/new/arduino-uno](https://wokwi.com/projects/new/arduino-uno).
2. Pega el código del archivo `.ino` correspondiente.
3. Haz clic en la pestaña **diagram.json** del simulador y pega el contenido del archivo `diagram.json` de la carpeta.
4. Presiona el botón verde **Start the simulation**.
5. *(Opcional)* Toma la captura directa de la pantalla de Wokwi si deseas anexarla junto a la foto del montaje real.

---

## 4. CONCLUSIONES

1. **Eficiencia y Protección de Puertos:** Con la resistencia limitadora de $220\ \Omega$ calculada por Ley de Ohm, la corriente por canal se mantiene en $\approx 13.6\text{ mA}$, respetando el límite seguro del fabricante ($< 20\text{ mA}$).
2. **Prioridad Peatonal y Seguridad Vial:** La inclusión de intervalos *All-Red* y destellos de aviso mitiga riesgos de atropello y colisiones en encrucijadas.
3. **Optimización con FSM y millis():** La eliminación de retardos bloqueantes en el Circuito 4 permite monitorear sensores o pulsadores en tiempo real con latencia cero.
4. **Validación Dual:** El proyecto cuenta con soporte simultáneo para simulación en Wokwi y montaje físico real sobre protoboard como se evidencia en la Figura 1.

---

## 5. TRAZABILIDAD Y REPOSITORIO
* **Repositorio GitHub:** [https://github.com/milith0kun/robotica-semaforos](https://github.com/milith0kun/robotica-semaforos)
* **ID de Sesión:** `231cda85-c992-4f6b-8f80-94698b1b354a`
