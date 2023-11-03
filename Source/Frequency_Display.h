/*
  ==============================================================================

    Frequency_Display.h
    Created: 2 Nov 2023 12:28:51pm
    Author:  Jacob Zeisel

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
//#include "maps.h"

using namespace juce;

namespace Gui
{
    class Frequency_Display : public Component
    {
    public:
        void paint(Graphics& g) override
        {
            auto text = std::to_string((int)frequency_val);

            // Get the bounds of the component
            juce::Rectangle<int> bounds = getLocalBounds();

            // Set the font and color for the text
            g.setColour(juce::Colours::white);
            g.setFont(35.0f);

            // Draw the text at the center of the component
            g.drawText(text, 0, 0, bounds.getWidth(), bounds.getHeight(),
                       juce::Justification::centred, true);
            
            // Calculate the position to center the text
            int textBoundsWidth = g.getCurrentFont().getStringWidth(text);
            int textBoundsHeight = g.getCurrentFont().getHeight();
            int x = (bounds.getWidth() - textBoundsWidth) / 2;
            int y = (bounds.getHeight() - textBoundsHeight) / 2;
            g.drawRoundedRectangle(x - rect_padding, y - rect_padding, textBoundsWidth + (2*rect_padding), textBoundsHeight + (2*rect_padding), 5.0, 4.0);
        }
        
        void setVal(float freq_val)
        {
            this->frequency_val = freq_val;
        }
    private:
        float frequency_val = 0.0;
        float rect_padding = 20.0;
    };
}
