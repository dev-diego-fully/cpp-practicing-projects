/**
 * @class Light
 * @brief Represents a single light in a semaphore system.
 *
 * This class encapsulates the state and behavior of a single LED light,
 * including its physical pin and the duration it should remain on.
 */
class Light {
 public:
  /**
   * @brief Constructs a new Light object.
   *
   * The constructor initializes the object without setting a specific pin or
   * duration. These are configured later using the `setUp` method.
   */
  Light() {}

  /**
   * @brief Configures the light with a pin and a duration.
   * @param pin The digital pin number connected to the LED.
   * @param duration The time in milliseconds the light should stay on.
   *
   * This method sets the internal pin and duration, and configures the pin as
   * an OUTPUT.
   */
  void setUp(int pin, int duration) {
    this->pin = pin;
    this->duration = duration;
    pinMode(pin, OUTPUT);
  }

  /**
   * @brief Turns the light on for its specified duration.
   *
   * This method sets the pin to HIGH, pauses for the configured duration,
   * and then sets the pin back to LOW. The `const` qualifier indicates that
   * this method does not modify the object's state.
   */
  void takeTime() const {
    digitalWrite(this->pin, HIGH);
    delay(this->duration);
    digitalWrite(this->pin, LOW);
  }

 private:
  int pin;       ///< The digital pin number of the LED.
  int duration;  ///< The duration in milliseconds the light stays on.
};

/**
 * @class Semaphore
 * @brief Manages a sequence of lights.
 * @tparam COUNT The number of lights in the semaphore.
 *
 * This template class holds an array of `Light` objects and steps through them
 * in a predefined sequence.
 */
template <int COUNT>
class Semaphore {
 public:
  /**
   * @brief Sets up a specific light within the semaphore.
   * @param index The 0-based index of the light to configure.
   * @param pin The digital pin for the light.
   * @param duration The duration for the light.
   *
   * This method delegates the configuration to the `setUp` method of a `Light`
   * object at the specified index, ensuring the index is within the valid
   * range.
   */
  void setLight(int index, int pin, int duration) {
    if (index >= 0 && index < COUNT) {
      this->lights[index].setUp(pin, duration);
    }
  }

  /**
   * @brief Advances the semaphore to the next light in the sequence.
   *
   * This method calls `takeTime` on the current light, then updates the
   * `current_light` index to point to the next light in the array, using
   * the modulo operator to cycle back to the beginning.
   */
  void step() {
    this->lights[this->current_light].takeTime();
    this->current_light = (this->current_light + 1) % COUNT;
  }

 private:
  Light lights[COUNT];  ///< An array of Light objects.
  int current_light;    ///< The index of the light currently being processed.
};

/// A global instance of a Semaphore with 3 lights.
Semaphore<3> semaphore;

/**
 * @brief The standard Arduino setup function.
 *
 * This function is called once at the beginning of the program. It initializes
 * the global `semaphore` object by configuring its three lights (Green, Yellow,
 * and Red) with their respective pins and durations.
 */
void setup() {
  constexpr int GREEN = 0, YELLOW = 1, RED = 2;
  constexpr int GREEN_PIN = 13, YELLOW_PIN = 12, RED_PIN = 11;
  constexpr int MAIN_DURATION = 5000, TRANSITION_DURATION = 2000;
  semaphore.setLight(GREEN, GREEN_PIN, MAIN_DURATION);
  semaphore.setLight(YELLOW, YELLOW_PIN, TRANSITION_DURATION);
  semaphore.setLight(RED, RED_PIN, MAIN_DURATION);
}

/**
 * @brief The standard Arduino loop function.
 *
 * This function is called repeatedly after the setup function completes.
 * It drives the semaphore's logic by calling the `step` method, causing the
 * lights to cycle indefinitely.
 */
void loop() { semaphore.step(); }