#define STACK_SIZE 1024

int change_me = 3;
static SemaphoreHandle_t mutex;
int real_num = 5;

void to_thread(void* num){
  while(true){
    Serial.println("thread here, changing");
    xSemaphoreTake(mutex, portMAX_DELAY);
    change_me = real_num;
    xSemaphoreGive(mutex);
    delay(10000);
  }
}

void setup() {
  Serial.begin(115200);
  delay(5000);
  Serial.println("begin with setup");
  TaskHandle_t xHandle = NULL;
  static uint8_t ucParameterToPass = 5;
  mutex = xSemaphoreCreateMutex();
  xTaskCreate(to_thread, "changing", STACK_SIZE, NULL, tskIDLE_PRIORITY, &xHandle);
  configASSERT(xHandle);
  Serial.println("done with setup");
}

void loop() {
   int tmp_change_me;
   xSemaphoreTake(mutex, portMAX_DELAY);
   tmp_change_me = change_me;
   xSemaphoreGive(mutex);
   Serial.printf("change_me: %d\n", tmp_change_me);
   if(tmp_change_me != 3){
    Serial.println("changing back");
    xSemaphoreTake(mutex, portMAX_DELAY);
    change_me = 3;
    xSemaphoreGive(mutex);
   }
   delay(5000);
}
