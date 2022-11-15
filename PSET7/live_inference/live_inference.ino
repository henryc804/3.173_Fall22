/*
Running live inference
 */

/* Includes ---------------------------------------------------------------- */
//CHANGE THIS LIBRARY TO BE THE NAME OF YOUR DOWNLOADED E.I. LIBRARY
#include <a3173_lecture_test_inferencing.h>

#include <ESP_CapSense.h>

// Capacitive sensor fibers
CapacitiveSensor fiber1 = CapacitiveSensor(4,33);        // resistor between pins 4 (A5) & 33, pin 33 is read pin
CapacitiveSensor fiber2 = CapacitiveSensor(4,15);        // resistor between pins 4 (A5) & 15, pin 15 is read pin

// Constants
const int numReadings = 5;
#define LED                 13      // Built-in LED is on pin 13
#define SAMPLING_TIME       1.5     // in seconds (I found that 1.5 seconds is about the time I do a gesture)
#define SAMPLING_FREQ_HZ    50       // Sampling frequency (Hz)
#define SAMPLING_PERIOD_MS  1000 / SAMPLING_FREQ_HZ   // Sampling period (ms)
#define NUM_SAMPLES         SAMPLING_TIME * SAMPLING_FREQ_HZ

/* Private variables from Edge Impulse */
static uint32_t run_inference_every_ms = 200;
static float buffer[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE] = { 0 };
static float inference_buffer[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE];
// IMPORTANT NOTE: EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE is the size expected, so this is how large the buffer is, the exact number comes from when we trained our network on EI's webportal
ei_impulse_result_t result = { 0 };

/* Forward declaration */
void run_inference_background();

// Arduino setup function
void setup()
{
  fiber1.set_CS_AutocaL_Millis(0xFFFFFFFF);  // turns off autocalibrate
  fiber2.set_CS_AutocaL_Millis(0xFFFFFFFF);  // turns off autocalibrate
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
  Serial.begin(115200);

  Serial.println("Live inferencing demo");
}

void loop()                    
{
  long fiber1Reading;
  long fiber2Reading;
  unsigned long timestamp;
  unsigned long start_timestamp;

  // Record samples in buffer at THE SAME FREQUENCY AS WE INITIALLY TRAINED THE NETWORK ON
  // we can use EI methods to make sure it's always the same, but for now, it's your job to keep it the same
  start_timestamp = millis();
  for (int index = 0; index < NUM_SAMPLES; index++) {
    // taking timestamp to measure out target frequency
    timestamp = millis();

    // roll the buffer -2 points so we can overwrite the last one
    numpy::roll(buffer, EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE, -2);

    // Fiber 1 is left
    fiber1Reading =  fiber1.capacitiveSensor(numReadings);
    // Fiber 2 is right
    fiber2Reading =  fiber2.capacitiveSensor(numReadings);

    // Tossing data into buffer
    // The order here matters! You can inspect the live classifications page to see that they take their raw features where it's a single array, but every pair of samples goes (left, right) (left, right) ...
    buffer[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE - 2] = fiber1Reading;
    buffer[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE - 1] = fiber2Reading;

    run_inference();
    display_results();

    // Wait just long enough for our sampling period
    while (millis() < timestamp + SAMPLING_PERIOD_MS);
  }

}

void run_inference()
{
  // the features are stored into flash, and we don't want to load everything into RAM
  signal_t features_signal;
  features_signal.total_length = sizeof(buffer) / sizeof(buffer[0]);
  features_signal.get_data = &raw_feature_get_data;

  // invoke the impulse
  EI_IMPULSE_ERROR res = run_classifier(&features_signal, &result, false /* debug */);
  Serial.print("run_classifier returned: ");
  Serial.println(res);  // res should be some kind of number, hopefully it is 0 because that means no error

  if (res != 0) return;
}

void display_results()
{
  // print the predictions
  ei_printf("Predictions ");
  ei_printf("(DSP: %d ms., Classification: %d ms., Anomaly: %d ms.)",
      result.timing.dsp, result.timing.classification, result.timing.anomaly);
  ei_printf(": \n");
  ei_printf("[");
  for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++) {
      ei_printf("%.5f", result.classification[ix].value);
#if EI_CLASSIFIER_HAS_ANOMALY == 1
        ei_printf(", ");
#else
        if (ix != EI_CLASSIFIER_LABEL_COUNT - 1) {
            ei_printf(", ");
        }
#endif
    }
#if EI_CLASSIFIER_HAS_ANOMALY == 1
    ei_printf("%.3f", result.anomaly);
#endif
    ei_printf("]\n");

    // human-readable predictions
    for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++) {
        ei_printf("    %s: %.5f\n", result.classification[ix].label, result.classification[ix].value);
    }
#if EI_CLASSIFIER_HAS_ANOMALY == 1
    ei_printf("    anomaly score: %.3f\n", result.anomaly);
#endif
}

/**
 * @brief      Copy raw feature data in out_ptr
 *             Function called by inference library
 *
 * @param[in]  offset   The offset
 * @param[in]  length   The length
 * @param      out_ptr  The out pointer
 *
 * @return     0
 */
int raw_feature_get_data(size_t offset, size_t length, float *out_ptr) {
    memcpy(out_ptr, buffer + offset, length * sizeof(float));
    return 0;
}
