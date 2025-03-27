import { useState, useEffect, ChangeEvent, MouseEvent } from "react"
import * as Juce from "juce-framework-frontend";
import { ISliderStateProperties } from "../interfaces/ISliderStateProperties";

interface Props {
  identifier: string,
  controlParameterIndexAnnotation: string,
  displayWithUnit: (value: string) => string
}

const Slider = ({ identifier, controlParameterIndexAnnotation, displayWithUnit }: Props) => {
  const sliderState = Juce.getSliderState(identifier)

  const [value, setValue] = useState<string>(sliderState.getNormalisedValue())
  const [properties, setProperties] = useState<ISliderStateProperties>(sliderState.properties)

  const handleChange = (e: ChangeEvent<HTMLInputElement>) => {
    sliderState.setNormalisedValue(e.target.value);
    setValue(e.target.value);
  };

  useEffect(() => {
    console.log(properties)
  }, [properties])

  const mouseDown = (): void => {
    sliderState.sliderDragStarted();
  };

  const changeCommitted = (e: MouseEvent<HTMLInputElement>): void => {
    sliderState.setNormalisedValue(e.currentTarget.value);
    sliderState.sliderDragEnded();
  };

  useEffect(() => {
    const valueListenerId = sliderState.valueChangedEvent.addListener(() => {
      setValue(sliderState.getNormalisedValue());
    });
    const propertiesListenerId = sliderState.propertiesChangedEvent.addListener(
      () => setProperties(sliderState.properties)
    );

    return function cleanup() {
      sliderState.valueChangedEvent.removeListener(valueListenerId);
      sliderState.propertiesChangedEvent.removeListener(propertiesListenerId);
    };
  }, []);

  return <div
    {...{
      [controlParameterIndexAnnotation]: sliderState?.properties?.paramaterIndex
    }}
    className="flex items-center justify-evenly flex-col gap-2 w-full"
  >
    <label className="input w-full">
      <span className="label">{properties.name}</span>
      <input
        type="range"
        aria-label={sliderState.name}
        min={0}
        max={1}
        step={1 / (properties.numSteps - 1)}
        value={value}
        onChange={handleChange}
        onMouseDown={mouseDown}
        onMouseUp={changeCommitted}
        className="range" />
      <span className="label">{displayWithUnit(sliderState.getScaledValue())}</span>
    </label>
  </div>
}

export default Slider
