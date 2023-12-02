# Pitch Tracking Plugin

This is an application of the following paper from the DAFX 2023 Conference:
A Frequency Tracker Based on a Kalman Filter Update of a Single Parameter Adaptive Notch Filter. (2023). In R. Ali \& T. Van Waterschoot (Eds.), DAFx23 (Vol. 26)

It implements a Kalman filter in tandem with an adaptive notch filter to do fast and efficient frequency tracking. This JUCE-based project implements the algorithm and utilizes it for tuning applications.

To run this application, follow these steps:

  1. git clone https://github.com/jzeiselmusic/Pitch_Tracker.git
  
  2. Download JUCE and launch Projucer: https://juce.com/download/. JUCE will ask you to build Projucer before you can use it

  3. Open the projucer file in this repo, and from there open the project in XCode.
  
  4. Click on the appropriate build target and hit the build/run button in XCode to compile the application.

## Usage ##

Use the sliders at the top of the window to pick different hyperparameter values for the Kalman filter. These values will have a large effect on the accuracy of the pitch detection algorithm. 

The drop-down boxes affect only the "note" determination and note the frequency determination. In other words, the frequency is quantized to whatever note is closest in the chosen key and scale. That quantized note is shown to the right of the frequency. 

Choose your audio input and output device on the righthand side of the screen. 


