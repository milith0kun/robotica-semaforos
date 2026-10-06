/*
 * ==============================================================================
 * PROYECTO 3: Semáforo Sincronizado para Carros y Peatones
 * ==============================================================================
 * Descripción:
 *   Control integral de una intersección con semáforo vehicular (Verde, Amarillo, Rojo)
 *   y semáforo peatonal (Verde, Rojo) trabajando en sincronía con garantías
 *   de seguridad vial.
 *
 * Reglas de Seguridad Crítica:
 *   1. Nunca deben coexistir simultáneamente luz verde vehicular y verde peatonal.
 *   2. Se incluye un intervalo "All-Red" (todo en rojo) para despeje total.
 *   3. La fase peatonal incluye parpadeo de advertencia antes de regresar a rojo.
 *
 * Asignación de Pines:
 *   [Semáforo Vehicular]
 *   - Pin 10 -> LED Verde Vehicular
 *   - Pin 9  -> LED Amarillo Vehicular
 *   - Pin 8  -> LED Rojo Vehicular
 *
 *   [Semáforo Peatonal]
 *   - Pin 7  -> LED Rojo Peatonal (Alto al peatón)
 *   - Pin 6  -> LED Verde Peatonal (Cruce peatonal permitido)
 *
 *   Todos los LEDs usan resistencias limitadoras de 220 Ω conectadas a GND.
 * ==============================================================================
 */

// Pines Semáforo Vehicular
const uint8_t PIN_AUTO_VERDE    = 10;
const uint8_t PIN_AUTO_AMARILLO = 9;
const uint8_t PIN_AUTO_ROJO     = 8;

// Pines Semáforo Peatonal
const uint8_t PIN_PEATON_ROJO   = 7;
const uint8_t PIN_PEATON_VERDE  = 6;

// Tiempos de las Fases (milisegundos)
const unsigned long TIEMPO_AUTO_VERDE       = 5000; // Carros avanzan
const unsigned long TIEMPO_AUTO_AMARILLO    = 2000; // Carros frenan
const unsigned long TIEMPO_SEGURIDAD_ROJO   = 1000; // Despeje intersección
const unsigned long TIEMPO_PEATON_VERDE     = 4000; // Peatones cruzan
const unsigned long TIEMPO_AVISO_PEATON     = 2000; // Parpadeo peatonal
const uint8_t       CANTIDAD_PARPADEOS      = 4;

void setup() {
  Serial.begin(9600);
  Serial.println(F("====================================================="));
  Serial.println(F(" Sistema Iniciado: 03 - Semáforo Vehicular y Peatonal"));
  Serial.println(F("====================================================="));

  // Configuración de salidas
  pinMode(PIN_AUTO_VERDE, OUTPUT);
  pinMode(PIN_AUTO_AMARILLO, OUTPUT);
  pinMode(PIN_AUTO_ROJO, OUTPUT);

  pinMode(PIN_PEATON_ROJO, OUTPUT);
  pinMode(PIN_PEATON_VERDE, OUTPUT);

  // Estado seguro inicial: Vehicular Verde, Peatonal Rojo
  actualizarSemaforos(HIGH, LOW, LOW, HIGH, LOW);
}

void loop() {
  // FASE 1: Paso vehicular (Autos Verde, Peatones Rojo)
  Serial.println(F("[FASE 1] Autos: VERDE | Peatones: ROJO"));
  actualizarSemaforos(HIGH, LOW, LOW, HIGH, LOW);
  delay(TIEMPO_AUTO_VERDE);

  // FASE 2: Advertencia vehicular (Autos Amarillo, Peatones Rojo)
  Serial.println(F("[FASE 2] Autos: AMARILLO (Precaución) | Peatones: ROJO"));
  actualizarSemaforos(LOW, HIGH, LOW, HIGH, LOW);
  delay(TIEMPO_AUTO_AMARILLO);

  // FASE 3: Seguridad / Despeje de intersección (Autos Rojo, Peatones Rojo)
  Serial.println(F("[FASE 3] Autos: ROJO | Peatones: ROJO (Despeje de vía)"));
  actualizarSemaforos(LOW, LOW, HIGH, HIGH, LOW);
  delay(TIEMPO_SEGURIDAD_ROJO);

  // FASE 4: Cruce peatonal seguro (Autos Rojo, Peatones Verde Fijo)
  Serial.println(F("[FASE 4] Autos: ROJO | Peatones: VERDE (Cruce peatonal habilitado)"));
  actualizarSemaforos(LOW, LOW, HIGH, LOW, HIGH);
  delay(TIEMPO_PEATON_VERDE);

  // FASE 5: Advertencia de fin de tiempo peatonal (Verde Peatonal parpadea)
  Serial.println(F("[FASE 5] Peatones: VERDE PARPADEANDO (Finalizando cruce)"));
  parpadearVerdePeatonal(CANTIDAD_PARPADEOS, TIEMPO_AVISO_PEATON);

  // FASE 6: Cierre Peatonal y preparación vehicular (Autos Rojo, Peatones Rojo)
  Serial.println(F("[FASE 6] Autos: ROJO | Peatones: ROJO (Transición final)"));
  actualizarSemaforos(LOW, LOW, HIGH, HIGH, LOW);
  delay(TIEMPO_SEGURIDAD_ROJO);
}

// Función centralizada para actualizar todas las salidas simultáneamente
void actualizarSemaforos(uint8_t aVerde, uint8_t aAmarillo, uint8_t aRojo,
                         uint8_t pRojo, uint8_t pVerde) {
  digitalWrite(PIN_AUTO_VERDE, aVerde);
  digitalWrite(PIN_AUTO_AMARILLO, aAmarillo);
  digitalWrite(PIN_AUTO_ROJO, aRojo);

  digitalWrite(PIN_PEATON_ROJO, pRojo);
  digitalWrite(PIN_PEATON_VERDE, pVerde);
}

// Función para generar destellos de aviso en el LED peatonal
void parpadearVerdePeatonal(uint8_t destellos, unsigned long tiempoTotal) {
  unsigned long retardoMitad = (tiempoTotal / destellos) / 2;
  for (uint8_t i = 0; i < destellos; i++) {
    digitalWrite(PIN_PEATON_VERDE, LOW);
    delay(retardoMitad);
    digitalWrite(PIN_PEATON_VERDE, HIGH);
    delay(retardoMitad);
  }
  digitalWrite(PIN_PEATON_VERDE, LOW);
}
