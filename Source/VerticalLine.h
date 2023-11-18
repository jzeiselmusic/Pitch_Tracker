#include <JuceHeader.h>

using namespace juce;

class VerticalLineComponent : public Component
{
public:
    VerticalLineComponent()
    {
    }
    
    void setYAndHeight(float start_y, float height) {
        startY = start_y;
        heightY = height;
    }

    void paint(Graphics& g) override
    {
        g.setColour(Colours::red); // Set the color of the vertical line
        g.drawLine(lineX, startY, lineX, heightY); // Replace lineX with your x-coordinate
    }

    void bringToFront()
    {
        toFront(true); // Bring this component to the front within its parent
    }
    
    void setX(float x) {
        lineX = x;
    }

private:
    float lineX = 100.0f; // Replace with your specific x-value for the line
    float startY;
    float heightY;
};
