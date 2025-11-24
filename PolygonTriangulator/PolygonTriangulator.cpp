#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <cstdlib>
#include "Polygon.h"
#include "Triangle.h"

void RunUnitTests();   // <-- forward declaration

static void PrintUsage(const char* exeName)
{
    std::cout <<
        "Usage:\n"
        "  " << exeName << " [input_csv] [plot]\n"
        "  " << exeName << " --test\n\n"
        "Options:\n"
        "  input_csv    Path to a CSV file containing polygon vertices (x,y pairs).\n"
        "               If omitted, a default sample file will be used.\n\n"
        "  plot         Optional. If the second argument is the keyword 'plot',\n"
        "               the program will invoke the Python visualization script.\n\n"
        "  --test       Run internal unit tests and exit.\n\n"
        "Examples:\n"
        "  " << exeName << "\n"
        "      Runs triangulation on the default polygon and prints the area.\n\n"
        "  " << exeName << " ../PolygonTriangulator/Input/simple_concave_poly.csv\n"
        "      Triangulates the specified polygon. No Python plot.\n\n"
        "  " << exeName << " ../PolygonTriangulator/Input/simple_concave_poly.csv plot\n"
        "      Triangulates and visualizes the polygon.\n\n"
        "  " << exeName << " --test\n"
        "      Runs internal tests only.\n"
        << std::endl;
}


void WriteTrianglesCsv(const std::string& tri_file,
    const std::vector<Triangle>& triangles) {
    std::ofstream out(tri_file);
    if (!out) {
        std::cerr << "Warning: could not open " << tri_file
                  << " for writing; visualization will be skipped.\n";
        return;
    }

    for (const auto& t : triangles) {
        out << t.a.id << "," << t.b.id << "," << t.c.id << "\n";
    }
}

int main(int argc, char* argv[])
{
    try {
        std::string csv_file_name;
        std::ifstream test;
        bool visualize = false;

        // Argument 1: input CSV
        if (argc >= 2 && std::string(argv[1]) == "--test") {
            RunUnitTests();
            return 0;
        }
        // If user asks for help explicitly
        if (argc >= 2 && (std::string(argv[1]) == "--help" ||
            std::string(argv[1]) == "-h")) {
            PrintUsage(argv[0]);
            return 0;
        }
        else if (argc >= 2) {
            csv_file_name = argv[1];
        }
        else {
            csv_file_name = "PolygonTriangulator/Input/concave_poly.csv";

            test.open(csv_file_name.c_str());
            if (!test) {
                // if running from bin/, try one level up
                csv_file_name = "../PolygonTriangulator/Input/concave_poly.csv";
            }
            std::cout << "No input file provided.\nUsing default polygon file:\n  "
                      << csv_file_name << "\n\n";
        }

        // Argument 2: if present and equal to "plot", enable visualization
        if (argc >= 3) {
            std::string arg2 = argv[2];
            if (arg2 == "plot" || arg2 == "--plot") {
                visualize = true;
            }
        }

        // Load polygon
        Polygon poly = Polygon::fromCSV(csv_file_name);

        // Triangulate
        std::vector<Triangle> triangles_optimized = poly.triangulate();

        // Compute cumulative area
        double area = 0.0;
        for (const auto& t : triangles_optimized) {
            area += t.area();
        }

        std::cout << std::fixed << std::setprecision(13);
        std::cout << "Ear-Clipping Triangulation Algorithm produced "
            << triangles_optimized.size()
            << " triangles with cumulative area = "
            << area
            << std::endl;

        // Optional visualization
        if (visualize) {
            const std::string tri_file = "triangles.csv";
            WriteTrianglesCsv(tri_file, triangles_optimized);

            // Build Python command: python visualize_triangulation.py <poly> <tris>
            std::string cmd = "python \"../PolygonTriangulator/visualize_triangulation.py\" \"" + csv_file_name + "\" \"" + tri_file + "\"";


            int result = std::system(cmd.c_str());
            if (result != 0) {
                std::cerr << "Warning: visualization command failed with code "
                    << result << "\n";
            }
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
