/*
 * ==============================================================================
 * PROYECTO 2: Simulación de un Semáforo Americano Estándar (MUTCD)
 * ==============================================================================
 * Descripción:
 *   Implementación del ciclo de un semáforo vehicular según el estándar
 *   estadounidense (Manual on Uniform Traffic Control Devices - MUTCD).
 *   Secuencia cíclica: VERDE -> AMARILLO (precaución) -> ROJO (alto) -> VERDE.
 *   (A diferencia del modelo británico/europeo, no existe fase Rojo+Amarillo simultánea).
 *
 * Análisis Circuital:
 *   - Vcc = 5.0 V
 *   - LED Rojo:     Vf ≈ 2.0 V  ->  R = (5.0 - 2.0) / 0.015 A ≈ 200 Ω -> Usamos 220 Ω
 *   - LED Amarillo: Vf ≈ 2.1 V  ->  R = (5.0 - 2.1) / 0.015 A ≈ 193 Ω -> Usamos 220 Ω
 *   - LED Verde:    Vf ≈ 3.0 V  ->  R = (5.0 - 3.0) / 0.010 A ≈ 200 Ω -> Usamos 220 Ω
 *   - Corriente total máxima simultánea: Solo un LED encendido a la vez (~10 - 15 mA),
 *     lo cual está muy por debajo del límite de 200 mA del regulador del ATmega328P.
 *
 * Asignación de Pines:
 *   - Pin Digital 10 -> Ánodo LED Verde (Salida)
 *   - Pin Digital 9  -> Ánodo LED Amarillo (Salida)
 *   - Pin Digital 8  -> Ánodo LED Rojo (Salida)
 *   - Cátodos de los 3 LEDs conectados a resistencias de 220 Ω y luego a GND común.
 * ==============================================================================
 */

// Definición de pines
const uint8_t PIN_VERDE    = 10;
const uint8_t PIN_AMARILLO = 9;
const uint8_t PIN_ROJO     = 8;

// Tiempos de cada fase en milisegundos (Escala adaptada para simulación cómoda)
const unsigned long TIEMPO_VERDE    = 5000; // 5 segundos en verde
const unsigned long TIEMPO_AMARILLO = 2000; // 2 segundos en amarillo (precaución)
const unsigned long TIEMPO_ROJO     = 5000; // 5 segundos en rojo (alto)

void setup() {
  Serial.begin(9600);
  Serial.println(F("============================================="));
  Serial.println(F(" Sistema Iniciado: 02 - Semáforo Americano  "));
  Serial.println(F(" Secuencia: VERDE -> AMARILLO -> ROJO       "));
  Serial.println(F("============================================="));

  // Configuración de los pines como salidas
  pinMode(PIN_VERDE, OUTPUT);
  pinMode(PIN_AMARILLO, OUTPUT);
  pinMode(PIN_ROJO, OUTPUT);

  // Estado inicial: Todos apagados
  apagarTodos();
}

void loop() {
  // FASE 1: LUZ VERDE (Paso vehicular permitido)
  establecerLuces(HIGH, LOW, LOW);
  Serial.println(F("[FASE] VERDE: Tráfico vehicular fluyendo..."));
  delay(TIEMPO_VERDE);

  // FASE 2: LUZ AMARILLA (Advertencia de cambio a rojo / despeje)
  establecerLuces(LOW, HIGH, LOW);
  Serial.println(F("[FASE] AMARILLO: Precaución, despeje de intersección..."));
  delay(TIEMPO_AMARILLO);

  // FASE 3: LUZ ROJA (Detención obligatoria)
  establecerLuces(LOW, LOW, HIGH);
  Serial.println(F("[FASE] ROJO: Alto total vehicular."));
  delay(TIEMPO_ROJO);
}

// Función auxiliar para fijar los estados de los tres LEDs de forma atómica y clara
void establecerLuces(uint8_t verde, uint8_t amarillo, uint8_t rojo) {
  digitalWrite(PIN_VERDE, verde);
  digitalWrite(PIN_AMARILLO, amarillo);
  digitalWrite(PIN_ROJO, rojo);
}

// Función para apagar todas las luces
void apagarTodos() {
  establecerLuces(LOW, LOW, LOW);
}
