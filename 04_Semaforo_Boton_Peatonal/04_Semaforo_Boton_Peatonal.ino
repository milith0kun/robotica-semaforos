/*
 * ==============================================================================
 * PROYECTO 4: Semáforo Inteligente a Demanda con Botón Peatonal (FSM no bloqueante)
 * ==============================================================================
 * Descripción:
 *   Sistema de semaforización a demanda operado mediante máquina de estados
 *   finitos (FSM) y temporización con millis() y antirrebote (debouncing).
 *   
 * Lógica Operativa:
 *   1. ESTADO_REPOSO: Autos en VERDE permanente, peatones en ROJO.
 *   2. Al pulsar el botón (Pin 2 con INPUT_PULLUP), se registra la solicitud.
 *   3. Se respeta un tiempo mínimo de verde vehicular (TIEMPO_MINIMO_AUTO_VERDE)
 *      para evitar congestión y frenadas bruscas.
 *   4. Secuencia de Cruce:
 *      - Autos AMARILLO (2 s)
 *      - Despeje ALL-RED (1 s)
 *      - Peatones VERDE fijo (5 s)
 *      - Peatones VERDE parpadeante (2.5 s)
 *      - Peatones ROJO (1 s)
 *      - Retorno a ESTADO_REPOSO con autos en VERDE.
 *
 * Conexiones de Hardware:
 *   [Semáforo Vehicular]
 *   - Pin 10 -> LED Verde Vehicular (R = 220 Ω a GND)
 *   - Pin 9  -> LED Amarillo Vehicular (R = 220 Ω a GND)
 *   - Pin 8  -> LED Rojo Vehicular (R = 220 Ω a GND)
 *
 *   [Semáforo Peatonal]
 *   - Pin 7  -> LED Rojo Peatonal (R = 220 Ω a GND)
 *   - Pin 6  -> LED Verde Peatonal (R = 220 Ω a GND)
 *
 *   [Botón Peatonal]
 *   - Pin 2  -> Terminal 1 del pulsador
 *   - GND    -> Terminal 2 del pulsador
 *   (Se utiliza la resistencia pull-up interna del microcontrolador: INPUT_PULLUP)
 * ==============================================================================
 */

// Asignación de Pines
const uint8_t PIN_AUTO_VERDE    = 10;
const uint8_t PIN_AUTO_AMARILLO = 9;
const uint8_t PIN_AUTO_ROJO     = 8;
const uint8_t PIN_PEATON_ROJO   = 7;
const uint8_t PIN_PEATON_VERDE  = 6;
const uint8_t PIN_BOTON         = 2; // Pin de interrupción externa INT0

// Parámetros Temporales (milisegundos)
const unsigned long TIEMPO_MIN_VERDE_AUTO = 4000; // Mínimo de tiempo verde vehicular
const unsigned long TIEMPO_AUTO_AMARILLO  = 2000; // Duración amarillo vehicular
const unsigned long TIEMPO_ALL_RED        = 1000; // Despeje total de seguridad
const unsigned long TIEMPO_PEATON_VERDE   = 5000; // Paso peatonal libre
const unsigned long TIEMPO_AVISO_PEATON   = 2400; // Parpadeo verde peatonal
const unsigned long TIEMPO_DEBOUNCE       = 200;  // Tiempo antirrebote mecánico

// Definición de Estados de la FSM
enum EstadoSemaforo {
  ESTADO_REPOSO_AUTO_VERDE,
  ESTADO_AUTO_AMARILLO,
  ESTADO_SEGURIDAD_ALL_RED_1,
  ESTADO_PEATON_VERDE,
  ESTADO_PEATON_AVISO_PARPADEO,
  ESTADO_SEGURIDAD_ALL_RED_2
};

// Variables de Control
EstadoSemaforo estadoActual = ESTADO_REPOSO_AUTO_VERDE;
unsigned long tiempoInicioEstado = 0;
unsigned long tiempoUltimoCambioVerdeAuto = 0;
unsigned long ultimoTiempoBoton = 0;
volatile bool peticionPeatonal = false;

// Variables para parpadeo no bloqueante
unsigned long ultimoParpadeo = 0;
bool estadoParpadeo = false;

void setup() {
  Serial.begin(9600);
  Serial.println(F("=========================================================="));
  Serial.println(F(" Sistema Iniciado: 04 - Semáforo con Botón Peatonal"));
  Serial.println(F(" Presione el botón en Pin 2 para solicitar cruce."));
  Serial.println(F("=========================================================="));

  // Configuración de salidas
  pinMode(PIN_AUTO_VERDE, OUTPUT);
  pinMode(PIN_AUTO_AMARILLO, OUTPUT);
  pinMode(PIN_AUTO_ROJO, OUTPUT);
  pinMode(PIN_PEATON_ROJO, OUTPUT);
  pinMode(PIN_PEATON_VERDE, OUTPUT);

  // Configuración de entrada con Pull-up interna (Presionado = LOW)
  pinMode(PIN_BOTON, INPUT_PULLUP);

  // Estado inicial por defecto
  cambiarEstado(ESTADO_REPOSO_AUTO_VERDE);
}

void loop() {
  // 1. Detección no bloqueante del botón con antirrebote
  verificarBoton();

  // 2. Máquina de Estados Finitos (FSM)
  unsigned long ahora = millis();

  switch (estadoActual) {

    case ESTADO_REPOSO_AUTO_VERDE:
      // Si hay una petición y ya pasó el tiempo mínimo garantizado para autos
      if (peticionPeatonal && (ahora - tiempoUltimoCambioVerdeAuto >= TIEMPO_MIN_VERDE_AUTO)) {
        Serial.println(F("[FSM] Atendiendo petición peatonal. Iniciando transición..."));
        cambiarEstado(ESTADO_AUTO_AMARILLO);
      }
      break;

    case ESTADO_AUTO_AMARILLO:
      if (ahora - tiempoInicioEstado >= TIEMPO_AUTO_AMARILLO) {
        cambiarEstado(ESTADO_SEGURIDAD_ALL_RED_1);
      }
      break;

    case ESTADO_SEGURIDAD_ALL_RED_1:
      if (ahora - tiempoInicioEstado >= TIEMPO_ALL_RED) {
        cambiarEstado(ESTADO_PEATON_VERDE);
      }
      break;

    case ESTADO_PEATON_VERDE:
      if (ahora - tiempoInicioEstado >= TIEMPO_PEATON_VERDE) {
        cambiarEstado(ESTADO_PEATON_AVISO_PARPADEO);
      }
      break;

    case ESTADO_PEATON_AVISO_PARPADEO:
      // Manejo no bloqueante del parpadeo del verde peatonal cada 300 ms
      if (ahora - ultimoParpadeo >= 300) {
        ultimoParpadeo = ahora;
        estadoParpadeo = !estadoParpadeo;
        digitalWrite(PIN_PEATON_VERDE, estadoParpadeo ? HIGH : LOW);
      }

      if (ahora - tiempoInicioEstado >= TIEMPO_AVISO_PEATON) {
        cambiarEstado(ESTADO_SEGURIDAD_ALL_RED_2);
      }
      break;

    case ESTADO_SEGURIDAD_ALL_RED_2:
      if (ahora - tiempoInicioEstado >= TIEMPO_ALL_RED) {
        // Fin de la atención a la petición
        peticionPeatonal = false;
        Serial.println(F("[FSM] Ciclo peatonal finalizado. Volviendo a flujo vehicular."));
        cambiarEstado(ESTADO_REPOSO_AUTO_VERDE);
      }
      break;
  }
}

// Función para registrar la pulsación del botón evitando rebotes mecánicos
void verificarBoton() {
  if (digitalRead(PIN_BOTON) == LOW) { // Pulsador presionado a GND
    unsigned long ahora = millis();
    if (ahora - ultimoTiempoBoton >= TIEMPO_DEBOUNCE) {
      if (!peticionPeatonal && estadoActual == ESTADO_REPOSO_AUTO_VERDE) {
        peticionPeatonal = true;
        Serial.println(F("[EVENTO] ¡Botón peatonal presionado! Solicitud registrada."));
      }
      ultimoTiempoBoton = ahora;
    }
  }
}

// Transición segura entre estados y ajuste de salidas digitales
void cambiarEstado(EstadoSemaforo nuevoEstado) {
  estadoActual = nuevoEstado;
  tiempoInicioEstado = millis();

  switch (nuevoEstado) {
    case ESTADO_REPOSO_AUTO_VERDE:
      // Autos en Verde, Peatones en Rojo
      setSalidas(HIGH, LOW, LOW, HIGH, LOW);
      tiempoUltimoCambioVerdeAuto = millis();
      Serial.println(F("[ESTADO] Reposo: Autos VERDE | Peatones ROJO"));
      break;

    case ESTADO_AUTO_AMARILLO:
      // Autos en Amarillo, Peatones en Rojo
      setSalidas(LOW, HIGH, LOW, HIGH, LOW);
      Serial.println(F("[ESTADO] Precaución: Autos AMARILLO | Peatones ROJO"));
      break;

    case ESTADO_SEGURIDAD_ALL_RED_1:
    case ESTADO_SEGURIDAD_ALL_RED_2:
      // Todos en Rojo por seguridad
      setSalidas(LOW, LOW, HIGH, HIGH, LOW);
      Serial.println(F("[ESTADO] Seguridad All-Red: Autos ROJO | Peatones ROJO"));
      break;

    case ESTADO_PEATON_VERDE:
      // Autos en Rojo, Peatones en Verde
      setSalidas(LOW, LOW, HIGH, LOW, HIGH);
      Serial.println(F("[ESTADO] Cruce: Autos ROJO | Peatones VERDE"));
      break;

    case ESTADO_PEATON_AVISO_PARPADEO:
      // Autos en Rojo, Verde Peatonal iniciará parpadeo
      setSalidas(LOW, LOW, HIGH, LOW, HIGH);
      estadoParpadeo = true;
      ultimoParpadeo = millis();
      Serial.println(F("[ESTADO] Aviso Peatonal: Verde parpadeando"));
      break;
  }
}

// Asignación atómica de pines de salida
void setSalidas(uint8_t aV, uint8_t aA, uint8_t aR, uint8_t pR, uint8_t pV) {
  digitalWrite(PIN_AUTO_VERDE, aV);
  digitalWrite(PIN_AUTO_AMARILLO, aA);
  digitalWrite(PIN_AUTO_ROJO, aR);
  digitalWrite(PIN_PEATON_ROJO, pR);
  digitalWrite(PIN_PEATON_VERDE, pV);
}
