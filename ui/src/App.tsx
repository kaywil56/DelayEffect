import Slider from "./components/Slider"
import ComboBox from "./components/ComboBox";
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
    <main className="h-screen w-screen flex items-center justify-center flex-col">
      <div className="h-fit flex items-center justify-evenly gap-4 flex-col">
        <Slider
          identifier="dryWet"
          controlParameterIndexAnnotation={controlParameterIndexAnnotation} />
        <Slider
          identifier="feedback"
          controlParameterIndexAnnotation={controlParameterIndexAnnotation} />
        <Slider
          identifier="delayTime"
          controlParameterIndexAnnotation={controlParameterIndexAnnotation} />
        <ComboBox
          identifier="delayTimeType"
          controlParameterIndexAnnotation={controlParameterIndexAnnotation} />
      </div>
      <div className="divider"></div>
      Made By Kaylem
    </main>
  )
}

export default App
