import Slider from "./components/Slider"
import ComboBox from "./components/ComboBox";
import * as Juce from "juce-framework-frontend"
import { useState, useEffect } from "react";

const controlParameterIndexAnnotation = "controlparameterindex";

function App() {
  const [comboBoxValue, setComboBoxValue] = useState<number>(0);
  const controlParameterIndexUpdater = new Juce.ControlParameterIndexUpdater(
    controlParameterIndexAnnotation
  );

  useEffect(() => {
    document.addEventListener("mousemove", (event) => {
      controlParameterIndexUpdater.handleMouseMove(event);
    });
  }, [])

  const convertValueToPercentage = (value: string): string => {
    return `${(Math.round(Number(value) * 100))}%`;
  }

  const convertValueToSeconds = (value: string): string => {
    return `${(parseFloat(value).toFixed(2))} Sec`;
  }

  const convertValueToFraction = (value: string): string => {
    const fractionValues: string[] = ["1", "2", "4", "8", "16"]
    let suffix: string = ""
    if (comboBoxValue == 2) {
      suffix = "t"
    }
    else if(comboBoxValue == 3){
      suffix = "d"
    }
    return `1/${fractionValues[Number(value)]}${suffix}`
  }

  return (
    <main className="h-screen w-screen flex items-center justify-center flex-col">
      <div className="h-fit flex items-center justify-evenly gap-4 flex-col">
        <Slider
          identifier="dryWet"
          controlParameterIndexAnnotation={controlParameterIndexAnnotation}
          displayWithUnit={convertValueToPercentage}
        />
        <Slider
          identifier="feedback"
          controlParameterIndexAnnotation={controlParameterIndexAnnotation}
          displayWithUnit={convertValueToPercentage}
        />
        {comboBoxValue == 0 ? (<Slider
          identifier="delayTime"
          controlParameterIndexAnnotation={controlParameterIndexAnnotation}
          displayWithUnit={convertValueToSeconds}
        />) : (
          <Slider
            identifier="delayTimeFraction"
            controlParameterIndexAnnotation={controlParameterIndexAnnotation}
            displayWithUnit={convertValueToFraction}
          />
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
