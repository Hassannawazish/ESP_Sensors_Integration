# ESP_Sensors_Integration
    1 : xTaskCreate(function_name,"Descriptive name for debugging purposes, stack size for the task ig 128, parameter we want to pass to the task,priority of task, task handler)
    
    Essential when you need to run multiple independent tasks concurrently in a FreeRTOS environment.It is infinite loop. We can do the profiling by printing integers within each thread.

    xTaskCreate(
    taskFunction,          // Task function
    "Task1",               // Task name
    128,                   // Stack size (in words)
    NULL,                  // No parameters
    1,                     // Task priority (1 is low, FreeRTOS supports multiple priorities)
    NULL                   // No task handle required
              );

