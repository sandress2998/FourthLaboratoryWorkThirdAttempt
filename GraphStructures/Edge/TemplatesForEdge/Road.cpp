#include "../GraphStructures/Edge/TemplatesForEdge/Road.h"
#include <string>
#include <stdexcept>

Road::Road() = default;

Road::Road(const std::string& objectToCompare) {
    if (objectToCompare == "length" || objectToCompare == "averageTime" ||
        objectToCompare == "price" || objectToCompare == "carsConcentration") {
        this->objectToCompare = objectToCompare;
        return;
    }
    throw std::invalid_argument("The wrong name of object to compare.");
}

Road::Road(double number) {
    this->length = number;
}

void Road::setObjectToCompare(const std::string& objectToCompare) {
    if (objectToCompare == "length" || objectToCompare == "averageTime" ||
        objectToCompare == "price" || objectToCompare == "carsConcentration") {
        this->objectToCompare = objectToCompare;
        return;
    }
    throw std::invalid_argument("The wrong name of object to compare.");
}

const std::string& Road::getObjectToCompare() const {
    return objectToCompare;
}

bool Road::operator<(const Road& other) {
    if (objectToCompare == "length") {
        return this->length < other.length;
    } else if (objectToCompare == "averageTime") {
        return this->averageTime < other.averageTime;
    } else if (objectToCompare == "price") {
        return this->price < other.price;
    } else {
        return this->carsConcentration < other.carsConcentration;
    }
}

bool Road::operator>(const Road& other) {
    if (objectToCompare == "length") {
        return this->length > other.length;
    } else if (objectToCompare == "averageTime") {
        return this->averageTime > other.averageTime;
    } else if (objectToCompare == "price") {
        return this->price > other.price;
    } else {
        return this->carsConcentration > other.carsConcentration;
    }
}

Road& Road::operator=(const Road& other) {
    this->length = other.length;
    this->averageTime = other.averageTime;
    this->price = other.price;
    this->carsConcentration = other.carsConcentration;
    this->objectToCompare = other.objectToCompare;
    return *this;
}

Road& Road::operator+=(const Road& other) {
    this->length += other.length;
    this->averageTime += other.averageTime;
    this->price += other.price;
    this->carsConcentration += other.carsConcentration;
    this->objectToCompare += other.objectToCompare;
    return *this;
}

Road& Road::operator+=(double number) {
    if (getObjectToCompare() == "length") {
        setLength(getLength() + number);
    } else if (getObjectToCompare() == "averageTime") {
        setAverageTime(getAverageTime() + number);
    } else if (getObjectToCompare() == "price") {
        setPrice(getPrice() + number);
    } else {
        setCarsConcentration(getCarsConcentration() + number);
    }
    return *this;
}

Road& Road::operator=(double number) {
    if (objectToCompare == "length") {
        this->length = number;
    } else if (objectToCompare == "averageTime") {
        this->averageTime = number;
    } else if (objectToCompare == "price") {
        this->price = number;
    } else {
        this->carsConcentration = number;
    }
    return *this;
}

double Road::getValueOfObjectToCompare() const {
    if (objectToCompare == "length") {
        return this->length;
    } else if (objectToCompare == "averageTime") {
        return this->averageTime;
    } else if (objectToCompare == "price") {
        return this->price;
    } else {
        return this->carsConcentration;
    }
}

double Road::getLength() const {
    return length;
}

double Road::getAverageTime() const {
    return averageTime;
}

double Road::getPrice() const {
    return price;
}

double Road::getCarsConcentration() const {
    return carsConcentration;
}

void Road::setLength(double newLength) {
    this->length = newLength;
}

void Road::setAverageTime(double newAverageTime) {
    this->averageTime = newAverageTime;
}

void Road::setPrice(double newPrice) {
    this->price = newPrice;
}

void Road::setCarsConcentration(double newCarsConcentration) {
    this->carsConcentration = newCarsConcentration;
}

Road operator+(const Road& firstRoad, const Road& secondRoad) {
    Road road(firstRoad.getObjectToCompare());
    road.setLength(firstRoad.getLength() + secondRoad.getLength());
    road.setAverageTime(firstRoad.getAverageTime() + secondRoad.getAverageTime());
    road.setPrice(firstRoad.getPrice() + secondRoad.getPrice());
    road.setCarsConcentration(firstRoad.getCarsConcentration() + secondRoad.getCarsConcentration());
    return road;
}

double operator+(double number, const Road& road) {
    double result = number;
    if (road.getObjectToCompare() == "length") {
        return result += road.getLength();
    } else if (road.getObjectToCompare() == "averageTime") {
        return result += road.getAverageTime();
    } else if (road.getObjectToCompare() == "price") {
        return result += road.getPrice();
    } else {
        return result += road.getCarsConcentration();
    }
}

double operator+(const Road& road, double number) {
    return number + road;
}

double& operator+=(double& number, const Road& road) {
    number += road.getValueOfObjectToCompare();
    return number;
}