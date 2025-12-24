enum Color { NONE = 0, RED = 1, YELLOW = 2, GREEN = 3 };

class Semaphore {
 public:
  Semaphore(int greenLightPin, int redLightPin, int yellowLight = -1);
  void setColor(Color color);

 private:
  int redLight;
  int greenLight;
  int yellowLight;

  void setToGreen();
  void setToYellow();
  void setToRed();
  void turnLightsOff();
  void turnLightOn(int lightsPin);
  void startPins();
};

class SemaphoreCoordinator {
 public:
  SemaphoreCoordinator(Semaphore *vehicles, Semaphore *pedestrians);
  void setColors(Color vehicles, Color pedestrians);

 private:
  Semaphore *vehicles;
  Semaphore *pedestrians;
};

struct ColorConfigs {
  int timeStamp;
  Color vehiclesColor;
  Color pedestriansColor;
};

int deltaTimeMs();

constexpr int VEHICLE_GREEN_PIN = 11, VEHICLE_RED_PIN = 13,
              VEHICLE_YELLOW_PIN = 12;
constexpr int PEDESTRIAN_GREEN_PIN = 9, PEDESTRIAN_RED_PIN = 10;

constexpr int RESET_TIME = 32000;
constexpr int COLOR_CONFIG_COUNT = 6;

int lastMs;
int semaphoreClock;

const ColorConfigs LIGHTS_CONFIGS[] = {
    {10000, GREEN, RED}, {12000, YELLOW, RED}, {22000, RED, GREEN},
    {30500, RED, NONE},  {31000, RED, GREEN},  {31500, RED, NONE},
};

Semaphore vehiclesLights(VEHICLE_GREEN_PIN, VEHICLE_RED_PIN,
                         VEHICLE_YELLOW_PIN);
Semaphore pedestriansLights(PEDESTRIAN_GREEN_PIN, PEDESTRIAN_RED_PIN);

SemaphoreCoordinator trafficLights(&vehiclesLights, &pedestriansLights);

void setup() {
  lastMs = 0;
  semaphoreClock = 0;
  trafficLights.setColors(NONE, NONE);
}

void loop() {
  const int deltaT = deltaTimeMs();
  semaphoreClock += deltaT;

  if (semaphoreClock >= RESET_TIME) {
    semaphoreClock -= RESET_TIME;
  }

  for (int i = 0; i < COLOR_CONFIG_COUNT; i++) {
    const ColorConfigs &currentConfig = LIGHTS_CONFIGS[i];

    if (semaphoreClock < currentConfig.timeStamp) {
      trafficLights.setColors(currentConfig.vehiclesColor,
                              currentConfig.pedestriansColor);
      return;
    }
  }
}

int deltaTimeMs() {
  const int currentMs = millis();
  const int deltaMs = currentMs - lastMs;
  lastMs = currentMs;

  return deltaMs;
}

Semaphore::Semaphore(int greenLightPin, int redLightPin, int yellowLight) {
  this->redLight = redLightPin;
  this->greenLight = greenLightPin;
  this->yellowLight = yellowLight;

  this->startPins();
}

void Semaphore::setColor(Color color) {
  switch (color) {
    case RED:
      this->setToRed();
      break;
    case GREEN:
      this->setToGreen();
      break;
    case YELLOW:
      this->setToYellow();
      break;
    default:
      this->turnLightsOff();
      break;
  }
}

void Semaphore::turnLightsOff() {
  digitalWrite(this->greenLight, LOW);
  digitalWrite(this->redLight, LOW);

  if (this->yellowLight >= 0) {
    digitalWrite(this->yellowLight, LOW);
  }
}

void Semaphore::startPins() {
  pinMode(this->greenLight, OUTPUT);
  pinMode(this->redLight, OUTPUT);

  if (this->yellowLight >= 0) {
    pinMode(this->yellowLight, OUTPUT);
  }
}

void Semaphore::turnLightOn(int lightsPin) { digitalWrite(lightsPin, HIGH); }

void Semaphore::setToGreen() {
  this->turnLightsOff();
  this->turnLightOn(this->greenLight);
}

void Semaphore::setToRed() {
  this->turnLightsOff();
  this->turnLightOn(this->redLight);
}

void Semaphore::setToYellow() {
  this->turnLightsOff();
  this->turnLightOn(this->yellowLight);
}

SemaphoreCoordinator::SemaphoreCoordinator(Semaphore *vehicles,
                                           Semaphore *pedestrians) {
  this->vehicles = vehicles;
  this->pedestrians = pedestrians;
}

void SemaphoreCoordinator::setColors(Color vehicles, Color pedestrians) {
  this->vehicles->setColor(vehicles);
  this->pedestrians->setColor(pedestrians);
}