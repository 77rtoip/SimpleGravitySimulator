#include <graphics.h>
#include <windows.h>
#include <iostream>
#include <conio.h>
#include <cmath>
#include <complex>
#define MAX_NUMBER_OF_BODIES 10

// Gravity simulator

class GravBody {
public:
    std::string name;
    double X, Y, VX, VY, mass;

    GravBody() {
        X = 0; Y = 0; VX = 0; VY = 0; mass = 1;
    }

    GravBody(std::string nn, double xx, double yy, double vvx, double vvy, double mm) {
        name = nn; X = xx; Y = yy; VX = vvx; VY = vvy; mass = mm;
    }

    void PrintStartingInfo() {
        std::cout << "Position: " << X << ", " << Y << "\n";
        std::cout << "Velocity: " << VX << ", " << VY << "\n";
        std::cout << "Mass: " << mass << "\n";
    }

    void PrintInfo() {
        std::cout << "Position: " << X << ", " << Y << "\n";
        std::cout << "Velocity: " << VX << ", " << VY << "\n";
    }
};

void CopyArray(GravBody arr1[], GravBody arr2[], int n) {
    for (int i = 0; i < n; ++i) {
        arr2[i] = arr1[i];
    }
}

class SystemOfBodies {
public:
    std::string name;
    const double gravConst = 1;
    const double deltaT = 0.1;
    double time;
    int number;
    GravBody body[MAX_NUMBER_OF_BODIES];
    SystemOfBodies(std::string nn, GravBody b) {
        name = nn;
        number = 1;
        time = 0;
        body[0] = b;
    }
    void Add(GravBody b) {
        if (number < MAX_NUMBER_OF_BODIES) body[number++] = b;
    }
    double DistSq(GravBody b1, GravBody b2) {
        return (b2.X-b1.X)*(b2.X-b1.X) + (b2.Y-b1.Y)*(b2.Y-b1.Y);
    }
    double Dist(GravBody b1, GravBody b2) {
        return sqrt(DistSq(b1, b2));
    }

    void PrintStartingInfo() {
        std::cout << name << "\n";
        std::cout << "Gravitational constant: " << gravConst << "\n";
        std::cout << "Delta t: " << deltaT << "\n";
        std::cout << "Number of bodies: " << number << "\n";
        for (int i = 0; i < number; ++i) {
            std::cout << "Body no " << i+1 << ": " << body[i].name << "\n";
            body[i].PrintStartingInfo();
            std::cout << "----------------------------------\n";
        }
    }

    void PrintInfo() {
        std::cout << "Time: " << time << "\n";
        for (int i = 0; i < number; ++i) {
            std::cout << "Body no " << i+1 << ": " << body[i].name << " (" << body[i].mass << ")\n";
            body[i].PrintInfo();
            std::cout << "-----------------------------------\n";
        }
        std::cout << "-----------------------------------\n";
    }

    void CalculateMovement() {
        GravBody temp[MAX_NUMBER_OF_BODIES];
        CopyArray(body, temp, number);
        for (int i = 0; i < number; ++i) {
            double dvX = 0, dvY = 0;
            for (int j = 0; j < number; ++j) {
                if (i!=j) {
                    dvX += gravConst * deltaT * body[j].mass * (body[j].X - body[i].X) / (Dist(body[i], body[j]) * DistSq(body[i], body[j]));
                    dvY += gravConst * deltaT * body[j].mass * (body[j].Y - body[i].Y) / (Dist(body[i], body[j]) * DistSq(body[i], body[j]));
                }
            }
            temp[i].VX += dvX;
            temp[i].VY += dvY;
            temp[i].X += body[i].VX * deltaT;
            temp[i].Y += body[i].VY * deltaT;
        }
        CopyArray(temp, body, number);
        time += deltaT;
    }
};

int main()
{
    unsigned long screenWidth = GetSystemMetrics(SM_CXSCREEN);
    unsigned long screenHeight = GetSystemMetrics(SM_CYSCREEN);

    int gdriver = VGA, gmode =  VGAMAX, errorcode;
    struct palettetype pal;

    initgraph(&gdriver, &gmode, "");

    //initwindow(screenWidth, screenHeight, "", -2, -3);
    errorcode = graphresult();
    if (errorcode != grOk)
    {
        printf("Graphics error: %s\n", grapherrormsg(errorcode));
        printf("Press any key to halt:");
        getch();
        exit(1);
    }

/*    getpalette(&pal);
    for (int i = 0; i < pal.size; ++i)
        setrgbpalette(pal.colors[i], i * 16, i * 16, i * 16);*/

    int const xmax_screen = getmaxx();
    int const ymax_screen = getmaxy();

// Main code

    GravBody earth("Earth", 0, 50, 5, 0, 1);
    GravBody sun("Sun", 0, 0, 0, 0, 1000);
    GravBody moon("Moon", 0, 55, 5.1, 0, 0.1);
    SystemOfBodies s1("Solar System", earth);
    s1.Add(sun);
    s1.Add(moon);
    s1.PrintStartingInfo();
    do {
        for (int i = 0; i < s1.number; ++i) {
            putpixel(s1.body[i].X + xmax_screen / 2, ymax_screen / 2 - s1.body[i].Y, WHITE);
        }
        s1.PrintInfo();
        s1.CalculateMovement();
    } while (s1.time < 1000);

    getch();
    cleardevice();
    closegraph();

    return 0;
}
