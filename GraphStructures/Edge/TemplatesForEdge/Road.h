#pragma once
#include <string>

class Road {
private:
    double length;
    double averageTime;
    double price;
    double carsConcentration;

    std::string objectToCompare = "length";

public:
    Road();

    Road(const std::string& objectToCompare);

    Road(double number);

    void setObjectToCompare(const std::string& objectToCompare);

    const std::string& getObjectToCompare() const;

    bool operator<(const Road& other);

    bool operator>(const Road& other);

    Road& operator=(const Road& other);

    Road& operator=(double number);

    Road& operator+=(const Road& other);

    Road& operator+=(double number);
    
    double getValueOfObjectToCompare() const;

    double getLength() const;

    double getAverageTime() const;

    double getPrice() const;

    double getCarsConcentration() const;

    void setLength(double newLength);

    void setAverageTime(double newAverageTime);

    void setPrice(double newPrice);

    void setCarsConcentration(double newCarsConcentration);
};

Road operator+(const Road& firstRoad, const Road& secondRoad);

double operator+(double number, const Road& road);

double operator+(const Road& road, double number);

double& operator+=(double& number, const Road& road);