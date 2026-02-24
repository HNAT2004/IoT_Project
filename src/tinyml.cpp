#include "tinyml.h"

// Globals, for the convenience of one-shot setup.
namespace
{
    tflite::ErrorReporter *error_reporter = nullptr;
    const tflite::Model *model = nullptr;
    tflite::MicroInterpreter *interpreter = nullptr;
    TfLiteTensor *input = nullptr;
    TfLiteTensor *output = nullptr;
    constexpr int kTensorArenaSize = 8 * 1024; 
    uint8_t tensor_arena[kTensorArenaSize];
} // namespace

void setupTinyML()
{
    Serial.println("TensorFlow Lite Init....");
    static tflite::MicroErrorReporter micro_error_reporter;
    error_reporter = &micro_error_reporter;

    model = tflite::GetModel(dht_anomaly_model_tflite); 
    if (model->version() != TFLITE_SCHEMA_VERSION)
    {
        error_reporter->Report("Model schema mismatch!");
        return;
    }

    static tflite::AllOpsResolver resolver;
    static tflite::MicroInterpreter static_interpreter(
        model, resolver, tensor_arena, kTensorArenaSize, error_reporter);
    interpreter = &static_interpreter;

    TfLiteStatus allocate_status = interpreter->AllocateTensors();
    if (allocate_status != kTfLiteOk)
    {
        error_reporter->Report("AllocateTensors() failed");
        return;
    }

    input = interpreter->input(0);
    output = interpreter->output(0);

    Serial.println("TensorFlow Lite Micro initialized on ESP32.");
}

void tiny_ml_task(void *pvParameters)
{
    setupTinyML();

    // Định nghĩa ngưỡng cảnh báo (Threshold) dựa trên kết quả log của bạn
    const float ANOMALY_THRESHOLD = 0.70f;

    while (1)
    {
        float temperature = 0;
        float humidity = 0;

        // Lấy dữ liệu từ Queue (đảm bảo lấy dữ liệu mới nhất)
        bool has_temp = xQueueReceive(xQueueTemperature, &temperature, 0) == pdTRUE;
        bool has_hum = xQueueReceive(xQueueHumidity, &humidity, 0) == pdTRUE;

        // Nếu không có dữ liệu trong Queue, thử đọc trực tiếp từ cảm biến
        if (!has_temp) temperature = dht20.getTemperature();
        if (!has_hum) humidity = dht20.getHumidity();

        // Gán dữ liệu vào Input Tensor
        input->data.f[0] = temperature;
        input->data.f[1] = humidity;

        // Chạy suy luận (Inference)
        TfLiteStatus invoke_status = interpreter->Invoke();
        if (invoke_status != kTfLiteOk)
        {
            error_reporter->Report("Invoke failed");
        }
        else
        {
            // Lấy kết quả đầu ra
            float result = output->data.f[0];
            Serial.print("AI Result: "); Serial.print(result, 4);

            if (result > ANOMALY_THRESHOLD)
            {
                Serial.println(" -> WARNING: ANOMALY DETECTION !!!");
            }
            else
            {
                Serial.println(" -> NORMAL DATA");
            }
        }

        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}