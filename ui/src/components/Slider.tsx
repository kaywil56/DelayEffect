import { useState, useEffect, ChangeEvent } from "react"
import * as Juce from "juce-framework-frontend";

interface Props {
  identifier: string,
  title: string,
  controlParameterIndexAnnotation: string
}

const Slider = ({ identifier, title, controlParameterIndexAnnotation } : Props) => {
  const sliderState = Juce.getSliderState(identifier)

  const [value, setValue] = useState<any>(sliderState.getNormalisedValue())
  const [properties, setProperties] = useState<any>(sliderState.properties)

  const handleChange = (e: ChangeEvent<HTMLInputElement>) => {
    sliderState.setNormalisedValue(parseFloat(e.target.value).toFixed(2));
    setValue(parseFloat(e.target.value).toFixed(2));
  };

  const mouseDown = () => {
    sliderState.sliderDragStarted();
  };

  const changeCommitted = () => {
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
  });

  return <div
  {...{
    [controlParameterIndexAnnotation]: sliderState.properties.paramaterIndex
  }}
  >
    <span className="label">{title}</span>
    <input 
    type="range"
    aria-label={title} 
    min={0.0} 
    max={1.01} 
    step={1 / (properties.numSteps - 1)}
    value={value} 
    onChange={handleChange}
    onMouseDown={mouseDown}
    onMouseUp={changeCommitted}
    className="range" />
    <span className="label">{value}</span>
  </div>
}

export default Slider
