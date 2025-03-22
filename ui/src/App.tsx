import Slider from "./components/Slider"
import * as Juce from "juce-framework-frontend"

const controlParameterIndexAnnotation = "controlparameterindex";

function App() {
  const controlParameterIndexUpdater = new Juce.ControlParameterIndexUpdater(
    controlParameterIndexAnnotation
  );

  document.addEventListener("mousemove", (event) => {
    controlParameterIndexUpdater.handleMouseMove(event);
  });

  return (
    <div>
      <Slider identifier="dryWet" title="Dry/Wet" controlParameterIndexAnnotation = "controlparameterindex"/>
      <h1>Text</h1>
    </div>
  )
}

export default App
