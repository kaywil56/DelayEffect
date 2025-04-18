import Slider from "./components/Slider"
import ComboBox from "./components/ComboBox";
import ToggleButton from "./components/ToggleButton";
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
    <main className="h-screen w-screen flex items-center justify-start flex-col">
      <div className="divider"></div>
      <div className="flex justify-evenly items-center w-full">
        <div style={{
          writingMode: "vertical-rl"
        }} className="text-primary-content grid place-content-center font-black text-7xl">
          DELAY
        </div>
        <div className="h-fit flex items-center justify-evenly gap-4 flex-col">
          <div className="flex items-center justify-end w-full">
            <ToggleButton identifier="isPingPong" controlParameterIndexAnnotation={controlParameterIndexAnnotation} />
          </div>
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
      <div className="font-bold text-primary grid place-items-center">
        Made by Company.
      </div>
    </main>
  )
}

export default App
