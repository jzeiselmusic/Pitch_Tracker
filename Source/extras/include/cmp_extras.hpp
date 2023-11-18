#pragma once

#include "../../include/include/cmp_lookandfeel.h"

namespace cmp {
class PlotLookAndFeelTimeline : public PlotLookAndFeel {
 public:
  PlotLookAndFeelTimeline();

  void overridePlotColours() noexcept override;

  juce::Rectangle<int> getGraphBounds(
      const juce::Rectangle<int> bounds,
      const juce::Component *const plot_comp) const noexcept override;

  void drawFrame(juce::Graphics &g [[maybe_unused]],
                 const juce::Rectangle<int> bounds [[maybe_unused]]) override;

  void updateGridLabels(const CommonPlotParameterView common_plot_params,
                        const std::vector<GridLine> &grid_lines,
                        StringVector &x_custom_label_ticks,
                        StringVector &y_custom_label_ticks,
                        LabelVector &x_axis_labels_out,
                        LabelVector &y_axis_labels_out) override;

  bool isXAxisLabelsBelowGraph() const noexcept override;

  void drawGridLabels(juce::Graphics &g, const LabelVector &x_axis_labels,
                      const LabelVector &y_axis_labels) override;

  juce::Font getGridLabelFont() const noexcept override;

  void updateVerticalGridLineTicksAuto(
      const juce::Rectangle<int> &bounds,
      const CommonPlotParameterView &common_plot_parameter_view,
      const GridType grid_type, const std::vector<float> &previous_ticks,
      std::vector<float> &x_ticks) noexcept override;

  void updateHorizontalGridLineTicksAuto(
      const juce::Rectangle<int> &bounds,
      const CommonPlotParameterView &common_plot_parameter_view,
      const GridType grid_type, const std::vector<float> &previous_ticks,
      std::vector<float> &y_ticks) noexcept override;

  std::size_t getMarginSmall() const noexcept override;

  void drawBackground(juce::Graphics &g,
                      const juce::Rectangle<int> &bounds) override;
};
}  // namespace cmp
