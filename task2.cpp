#include <sciplot/sciplot.hpp>
using namespace sciplot;

int main(int argc, char** argv)
{
    // Create a vector with values from 0 to 5 divived into 200 uniform intervals for the x-axis
    Vec x = linspace(0.0, 5.0, 200);

    // Create 4 different plots
    Plot2D plot0;
    plot0.drawCurve(x, std::sin(x)).label("sin(x)");
    plot0.drawCurve(x, std::cos(x)).label("cos(x)");

    Plot2D plot1;
    plot1.drawCurve(x, std::cos(x)).label("cos(x)");

    Plot2D plot2;
    plot2.drawCurve(x, std::tan(x)).label("tan(x)");

    Plot2D plot3;
    plot3.drawCurve(x, std::sqrt(x)).label("sqrt(x)");

    // Use the previous plots as sub-figures in a larger 2x2 figure.
    Figure fig = {{plot0, plot1},
                  {plot2, plot3}};

    fig.title("Trigonometric Functions");
    fig.palette("dark2");

    // Create canvas to hold figure
    Canvas canvas = {{fig}};
    // Set canvas output size
    canvas.size(600, 600);

    // Show the plot in a pop-up window
    //canvas.show();

    // Save the figure to a PDF file
    canvas.save("example-multiplot.pdf");
}