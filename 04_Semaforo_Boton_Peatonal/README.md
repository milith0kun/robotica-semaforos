# Circuito 4: Semáforo a Demanda con Botón Peatonal

## 1. Análisis de Ingeniería y Máquina de Estados (FSM)
En cruces donde el tráfico peatonal es esporádico, mantener un ciclo rígido causa congestión innecesaria para los vehículos. Este sistema implementa un **semáforo a demanda**:
1. El estado normal o de reposo mantiene el **verde vehicular permanentemente**.
2. Cuando un peatón oprime el pulsador, el sistema registra el evento.
3. Se verifica que haya transcurrido un tiempo mínimo de verde vehicular ($T_{min} = 4\text{ s}$) para evitar frenadas intempestivas en caso de pulsaciones inmediatas sucesivas.
4. Se ejecuta la secuencia segura de transición hacia el cruce peatonal y posterior retorno.

### Diagrama de Estados Finitos (FSM)
```text
 +──────────────────────────+
 │ ESTADO_REPOSO_AUTO_VERDE │ ◄──────────────────────────────────+
 +──────────────────────────+                                    │
               │ (Botón presionado y t >= T_min)                 │
               ▼                                                 │
 +──────────────────────────+                                    │
 │   ESTADO_AUTO_AMARILLO   │                                    │
 +──────────────────────────+                                    │
               │ (t >= 2 s)                                      │
               ▼                                                 │
 +──────────────────────────+                                    │
 │ ESTADO_SEGURIDAD_ALL_RED │                                    │
 +──────────────────────────+                                    │
               │ (t >= 1 s)                                      │
               ▼                                                 │
 +──────────────────────────+                                    │
 │   ESTADO_PEATON_VERDE    │                                    │
 +──────────────────────────+                                    │
               │ (t >= 5 s)                                      │
               ▼                                                 │
 +──────────────────────────+                                    │
 │ ESTADO_PEATON_PARPADEO   │                                    │
 +──────────────────────────+                                    │
               │ (t >= 2.4 s)                                    │
               ▼                                                 │
 +──────────────────────────+                                    │
 │ ESTADO_SEGURIDAD_ALL_RED │ ───────────────────────────────────+
 +──────────────────────────+
```

---

## 2. Configuración Eléctrica y Antirrebote (Debounce)
- **Botón Peatonal**: Conectado entre el pin digital **D2** y **GND**.
- Se utiliza el modo `pinMode(PIN_BOTON, INPUT_PULLUP)`. Esto activa la resistencia de polarización pull-up integrada de ~20-50 kΩ en el chip ATmega328P.
- **Lógica Inversa**: Al no presionarse, el pin lee `HIGH` (5V); al presionarse, se deriva a tierra y lee `LOW` (0V).
- **Antirrebote**: Por software, se descartan cambios en un periodo de $200\text{ ms}$ para filtrar vibraciones mecánicas del contacto metálico.

---

## 3. Simulación en Wokwi
Abre `04_Semaforo_Boton_Peatonal.ino` y `diagram.json` en [Wokwi](https://wokwi.com/projects/new/arduino-uno). Haz clic sobre el botón "Cruzar" azul para ver cómo responde el semáforo.
