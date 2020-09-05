#pragma once

struct SensorData {
    union {
        float v[3];
        struct {
            float x;
            float y;
            float z;
        };
    };

    SensorData() : x(0), y(0), z(0) { }
};