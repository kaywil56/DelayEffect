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

  const convertValueToPercentage = (value: number): string => {
    return `${(Math.round(value * 100))}%`;
  }

  const convertValueToSeconds = (value: number): string => {
    return `${(parseFloat(String(value)).toFixed(2))} Sec`;
  }

  const convertValueToFraction = (value: number): string => {
    const fractionValues: string[] = ["1", "2", "4", "8", "16"]
    let suffix: string = ""
    if (comboBoxValue == 2) {
      suffix = "t"
    }
    else if (comboBoxValue == 3) {
      suffix = "d"
    }
    return `1/${fractionValues[value]}${suffix}`
  }

  return (
    <main className="h-screen w-screen flex items-center justify-center flex-col">
      <div className="divider"></div>
      <div className="flex justify-center items-center w-full">
        <div style={{
          writingMode: "vertical-rl",
          fontSize: "9vw"
        }} className="text-primary-content grid place-content-center font-black">
          DELAY
        </div>
        <div className="h-fit flex items-center justify-evenly gap-4 flex-col">
          <Slider
            identifier="dryWet"
            controlParameterIndexAnnotation={controlParameterIndexAnnotation}
            displayWithUnit={convertValueToPercentage}
            key="dryWet"
          />
          <Slider
            identifier="feedback"
            controlParameterIndexAnnotation={controlParameterIndexAnnotation}
            displayWithUnit={convertValueToPercentage}
            key="feedback"
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
      </div>
      <div className="divider"></div>
      Made By Kaylem
    </main>
  )
}

export default App
