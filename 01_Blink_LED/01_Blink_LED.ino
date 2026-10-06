/*
 * ==============================================================================
 * PROYECTO 1: Control de Encendido y Apagado de un LED (Blink)
 * ==============================================================================
 * Descripción:
 *   Programa básico para alternar el estado de un LED conectado al pin digital 8
 *   de Arduino UNO cada 1000 milisegundos (1 segundo).
 *
 * Análisis Circuital:
 *   - Voltaje de alimentación digital (Arduino UNO): Vcc = 5.0 V
 *   - Voltaje directo típico del LED rojo: Vf ≈ 2.0 V
 *   - Corriente de trabajo recomendada: If ≈ 13.6 mA
 *   - Resistencia limitadora: R = (Vcc - Vf) / If = (5.0V - 2.0V) / 0.0136A = 220 Ω
 *   - Potencia disipada en R: P = I^2 * R = (0.0136)^2 * 220 ≈ 0.041 W (1/4 W sobra)
 *
 * Conexiones de Hardware:
 *   - Pin Digital 8 Arduino -> Ánodo (+) del LED (pata larga)
 *   - Cátodo (-) del LED (pata corta) -> Terminal 1 de Resistencia (220 Ω)
 *   - Terminal 2 de Resistencia -> GND de Arduino
 * ==============================================================================
 */

// Definición de pines mediante constantes para optimizar memoria y legibilidad
const uint8_t PIN_LED = 8;

// Configuración de tiempos (milisegundos)
const unsigned long TIEMPO_ENCENDIDO = 1000;
const unsigned long TIEMPO_APAGADO   = 1000;

void setup() {
  // Inicialización de la comunicación serial para monitoreo / depuración
  Serial.begin(9600);
  Serial.println(F("========================================"));
  Serial.println(F("  Sistema Iniciado: 01 - Blink LED      "));
  Serial.println(F("========================================"));

  // Configuración del pin del LED como salida digital
  pinMode(PIN_LED, OUTPUT);

  // Aseguramos que el LED inicie apagado
  digitalWrite(PIN_LED, LOW);
}

void loop() {
  // 1. Encender el LED
  digitalWrite(PIN_LED, HIGH);
  Serial.println(F("[ESTADO] LED ENCENDIDO (HIGH)"));
  delay(TIEMPO_ENCENDIDO);

  // 2. Apagar el LED
  digitalWrite(PIN_LED, LOW);
  Serial.println(F("[ESTADO] LED APAGADO (LOW)"));
  delay(TIEMPO_APAGADO);
}
