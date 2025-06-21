// FixedIncomeYieldCalculator.cpp
// A CLI-based tool to compute Yield to Maturity (YTM) for fixed income bonds
// Author: Vaibhavee Thorat

#include <iostream>
#include <cmath>
#include <iomanip>

// Function to compute bond price given yield
// Price = Σ (C / (1 + y)^t) + F / (1 + y)^n
// Using for estimation in Newton-Raphson

double bondPrice(double faceValue, double couponRate, double years, double yield) {
    double price = 0.0;
    for (int t = 1; t <= years; ++t) {
        price += (faceValue * couponRate) / std::pow(1 + yield, t);
    }
    price += faceValue / std::pow(1 + yield, years);
    return price;
}

// Derivative of bond price with respect to yield
double bondPriceDerivative(double faceValue, double couponRate, double years, double yield) {
    double derivative = 0.0;
    for (int t = 1; t <= years; ++t) {
        derivative -= t * (faceValue * couponRate) / std::pow(1 + yield, t + 1);
    }
    derivative -= years * faceValue / std::pow(1 + yield, years + 1);
    return derivative;
}

// Newton-Raphson method to calculate YTM
double calculateYTM(double faceValue, double couponRate, double years, double marketPrice) {
    double ytm = 0.05; // Initial guess
    double tolerance = 1e-6;
    int maxIterations = 100;

    for (int i = 0; i < maxIterations; ++i) {
        double f = bondPrice(faceValue, couponRate, years, ytm) - marketPrice;
        double f_prime = bondPriceDerivative(faceValue, couponRate, years, ytm);

        if (std::abs(f_prime) < 1e-10) break; // avoid division by very small number

        double newYTM = ytm - f / f_prime;

        if (std::abs(newYTM - ytm) < tolerance) return newYTM;

        ytm = newYTM;
    }
    return ytm;
}

int main() {
    double faceValue, couponRate, years, marketPrice;

    std::cout << "\nFixed Income Yield to Maturity Calculator\n";
    std::cout << "----------------------------------------\n";
    std::cout << "Enter Face Value of Bond: ";
    std::cin >> faceValue;
    std::cout << "Enter Annual Coupon Rate (in %): ";
    std::cin >> couponRate;
    couponRate /= 100.0; // convert to decimal
    std::cout << "Enter Years to Maturity: ";
    std::cin >> years;
    std::cout << "Enter Market Price of Bond: ";
    std::cin >> marketPrice;

    double ytm = calculateYTM(faceValue, couponRate, years, marketPrice);

    std::cout << std::fixed << std::setprecision(4);
    std::cout << "\nEstimated Yield to Maturity (YTM): " << ytm * 100 << "%\n";

    return 0;
}
