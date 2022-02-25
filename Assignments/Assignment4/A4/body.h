typedef struct coordinate
{
    double x;
    double y;
} coordinate_t;

typedef struct body
{
    coordinate_t pos;
    double mass;
    coordinate_t vel;
    double brightness;
} body_t;