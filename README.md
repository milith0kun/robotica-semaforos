# Laboratorio de Robótica: Análisis, Diseño y Simulación de Circuitos Semafóricos

Este repositorio contiene el análisis teórico, diseño electrónico, esquemas de conexión y código fuente en C++/Arduino para la simulación de 4 sistemas de control con Arduino UNO y el simulador Wokwi.

---

## 📁 Estructura del Repositorio

```text
Robotica/
├── 01_Blink_LED/                      # Circuito 1: Prender y apagar un LED
│   ├── 01_Blink_LED.ino               # Código fuente Arduino comentado
│   ├── diagram.json                   # Esquema de conexión para Wokwi
│   ├── wokwi-project.txt              # Configuración de Wokwi
│   └── README.md                      # Explicación y cálculos
├── 02_Semaforo_Americano/             # Circuito 2: Semáforo vehicular americano
│   ├── 02_Semaforo_Americano.ino      # Código fuente con secuencia MUTCD
│   ├── diagram.json                   # Esquema de conexión para Wokwi
│   ├── wokwi-project.txt              # Configuración de Wokwi
│   └── README.md                      # Explicación y tabla de estados
├── 03_Semaforo_Vehicular_Peatonal/    # Circuito 3: Semáforo sincronizado autos + peatones
│   ├── 03_Semaforo_Vehicular_Peatonal.ino # Código fuente con interbloqueo seguro
│   ├── diagram.json                   # Esquema con 5 LEDs y resistencias
│   ├── wokwi-project.txt              # Configuración de Wokwi
│   └── README.md                      # Matriz de estados y seguridad
├── 04_Semaforo_Boton_Peatonal/        # Circuito 4: Semáforo inteligente a demanda con botón
│   ├── 04_Semaforo_Boton_Peatonal.ino # FSM no bloqueante con millis() y debouncing
│   ├── diagram.json                   # Esquema con pulsador e INPUT_PULLUP
│   ├── wokwi-project.txt              # Configuración de Wokwi
│   └── README.md                      # Diagrama de estados FSM
├── docs/
│   └── analisis_y_diseno.md           # Memoria técnica completa de ingeniería
├── .gitignore                         # Filtro de archivos no deseados
└── README.md                          # Este archivo principal
```

---

## 🔬 Resumen de Circuitos Implementados

### 1. Prender y apagar un LED (`01_Blink_LED`)
- **Objetivo**: Control digital básico mediante GPIO.
- **Pin**: Pin Digital 8 a Ánodo LED Rojo.
- **Resistencia limitadora**: $220\ \Omega$ calculada por Ley de Ohm ($V_f = 2.0\text{V}$, $I_f \approx 13.6\text{mA}$).

### 2. Semáforo Americano (`02_Semaforo_Americano`)
- **Objetivo**: Simulación de la secuencia estándar de Estados Unidos (MUTCD).
- **Secuencia**: **Verde** (5 s) $\to$ **Amarillo** (2 s) $\to$ **Rojo** (5 s) $\to$ **Verde** (cíclico).
- **Pines**: Verde (D10), Amarillo (D9), Rojo (D8).

### 3. Semáforo para Carros y Personas (`03_Semaforo_Vehicular_Peatonal`)
- **Objetivo**: Coordinación vehicular y peatonal con interbloqueo y periodo de seguridad *All-Red*.
- **Pines Vehiculares**: Verde (D10), Amarillo (D9), Rojo (D8).
- **Pines Peatonales**: Rojo (D7), Verde (D6).
- **Características de seguridad**: 
  - Nunca coinciden verdes vehiculares y peatonales.
  - Intervalo de 1 segundo de *All-Red* (ambos en rojo) para despeje de intersección.
  - Parpadeo de advertencia en el verde peatonal antes de regresar a rojo.

### 4. Semáforo con Botón Peatonal (`04_Semaforo_Boton_Peatonal`)
- **Objetivo**: Sistema a demanda operado por pulsador mediante **Máquina de Estados Finitos (FSM) no bloqueante**.
- **Entrada**: Botón en **Pin D2** configurado con resistencia interna `INPUT_PULLUP`.
- **Filtro Antirrebote**: Ventana de 200 ms con `millis()`.
- **Tiempo mínimo garantizado**: 4 segundos de verde vehicular continuo antes de conceder paso al peatón.

---

## 🌐 Simulación en Wokwi (Paso a Paso)

Cada circuito está listo para simularse en [Wokwi Online](https://wokwi.com/):

1. Ingresa a [Wokwi Arduino Uno Simulator](https://wokwi.com/projects/new/arduino-uno).
2. Abre la carpeta del circuito deseado (por ejemplo, `04_Semaforo_Boton_Peatonal/`).
3. Copia el contenido del archivo `.ino` y pégalo en la pestaña `sketch.ino`.
4. Ve a la pestaña `diagram.json` en Wokwi y reemplaza su contenido con el `diagram.json` del proyecto.
5. Haz clic en **Start the simulation** (ícono de Play verde).
6. Abre el **Serial Monitor** dentro de Wokwi (9600 baud) para observar los mensajes de diagnóstico en tiempo real.

---

## 🚀 Publicación del Código en GitHub

El repositorio local ya está inicializado con Git. Para publicarlo en tu cuenta de GitHub, sigue estos pasos:

1. Crea un nuevo repositorio vacío en [GitHub](https://github.com/new) (por ejemplo llamado `robotica-semaforos`). No marques la opción de inicializar con README ni `.gitignore` (ya están creados localmente).
2. En tu terminal (PowerShell), ubicado en la carpeta del proyecto (`D:\Proyectos\Robotica`), ejecuta:

```powershell
# 1. Agregar y confirmar todos los archivos locales
git add .
git commit -m "feat: Implementacion completa de los 4 circuitos semaforicos con esquemas Wokwi y documentacion"

# 2. Renombrar la rama a main
git branch -M main

# 3. Vincular con tu repositorio remoto de GitHub (sustituye TU_USUARIO y TU_REPOSITORIO)
git remote add origin https://github.com/TU_USUARIO/TU_REPOSITORIO.git

# 4. Subir todos los archivos a GitHub
git push -u origin main
```

---

## 🔗 Enlace de las Conversaciones y Trazabilidad

Para dar cumplimiento al punto **6.- link de las conversaciones**:
- **ID de Sesión de Desarrollo**: `231cda85-c992-4f6b-8f80-94698b1b354a`
- **Registro de Trazabilidad Local**: La sesión completa de análisis, diseño iterativo, generación de diagramas y código está documentada en los registros del entorno de desarrollo.
- **Enlace de Compartir**: Si requieres compartir la transcripción o enlace web de la sesión, puedes utilizar la función **Share Chat / Compartir conversación** de tu asistente o consultar el informe consolidado en [`docs/analisis_y_diseno.md`](docs/analisis_y_diseno.md).
