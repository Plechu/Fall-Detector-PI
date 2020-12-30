#include <ADXL345.h>

#define impactTh 2
#define angleTh 65

struct Vector{
    float X;
    float Y;
    float Z;

    void update(float x, float y, float z); // aktualizacja wspolrzednych

    void makeVersor(); // tworzenie wektora jednostkowego
};

class Accelerometer{

    ADXL345 accel;
    Vector actual;
    Vector reference;
    
public:
    bool getStatus(); // inicjalizacja akcelerometru oraz pobranie wartosci referencyjnego wektora przyspieszenia

    bool setActualVector(); // pobranie wartosci aktualnego wektora przyspieszenia

    bool isFall(); // sprawdzenie wystapienia upadku
};