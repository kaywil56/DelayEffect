import { useState, useEffect, ChangeEvent, MouseEvent } from "react"
import * as Juce from "juce-framework-frontend";

interface Props {
  identifier: string,
  controlParameterIndexAnnotation: string,
}

const Slider = ({ identifier, controlParameterIndexAnnotation }: Props) => {
  const sliderState = Juce.getSliderState(identifier)

  const [value, setValue] = useState<string>(sliderState?.getNormalisedValue())
  const [properties, setProperties] = useState<string>(sliderState?.properties)

  const handleChange = (e: ChangeEvent<HTMLInputElement>) => {
    sliderState.setNormalisedValue(e.target.value);
    setValue(e.target.value);
  };

  const mouseDown = (): void => {
    sliderState.sliderDragStarted();
  };

  const changeCommitted = (e: MouseEvent<HTMLInputElement>): void => {
    sliderState.setNormalisedValue(e.currentTarget.value);
    sliderState.sliderDragEnded();
  };

  useEffect(() => {
    const valueListenerId = sliderState.valueChangedEvent.addListener(() => {
      setValue(sliderState?.getNormalisedValue());
    });
    const propertiesListenerId = sliderState.propertiesChangedEvent.addListener(
      () => setProperties(sliderState?.properties)
    );

    return function cleanup() {
      sliderState.valueChangedEvent.removeListener(valueListenerId);
      sliderState.propertiesChangedEvent.removeListener(propertiesListenerId);
    };
  }, []);

  return <div
    {...{
      [controlParameterIndexAnnotation]: sliderState.properties.paramaterIndex
    }}
    className="flex items-center justify-evenly flex-col gap-2 w-full"
  >
    <label className="input w-full">
      <span className="label">{properties?.name}</span>
      <input
        type="range"
        aria-label={properties?.name}
        min={0}
        max={1}
        step={1 / (properties?.numSteps - 1)}
        value={value}
        onChange={handleChange}
        onMouseDown={mouseDown}
        onMouseUp={changeCommitted}
        className="range" />
      <span className="label">{parseFloat(sliderState.getScaledValue()).toFixed(2)}</span>
    </label>
  </div>
}

export default Slider
