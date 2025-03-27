import Slider from "./components/Slider"
import ComboBox from "./components/ComboBox";
import * as Juce from "juce-framework-frontend"
import { useState } from "react";

const controlParameterIndexAnnotation = "controlparameterindex";

function App() {
  const [comboBoxValue, setComboBoxValue] = useState<number>(0);
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
        {comboBoxValue == 0 ? (<Slider
          identifier="delayTime"
          controlParameterIndexAnnotation={controlParameterIndexAnnotation} />) : (
          <Slider
            identifier="delayTimeFraction"
            controlParameterIndexAnnotation={controlParameterIndexAnnotation} />
        )}
        <ComboBox
          value={comboBoxValue}
          setValue={setComboBoxValue}
          identifier="delayTimeType"
          controlParameterIndexAnnotation={controlParameterIndexAnnotation} />
      </div>
      <div className="divider"></div>
      Made By Kaylem
    </main>
  )
}

export default App
