import { useState, useEffect, ChangeEvent } from "react";
import * as Juce from "juce-framework-frontend";
import { IToggleButtonProperties } from "../interfaces/IToggleButtonProperties";

interface Props {
    identifier: string,
    controlParameterIndexAnnotation: string
}

const ToggleButton = ({ identifier, controlParameterIndexAnnotation }: Props) => {
    const toggleButtonState = Juce.getToggleState(identifier);

    const [value, setValue] = useState<boolean>(toggleButtonState.getValue());
    const [properties, setProperties] = useState<IToggleButtonProperties>(toggleButtonState.properties);

    const handleChange = (e: ChangeEvent<HTMLInputElement>) => {
        toggleButtonState.setValue(e.target.checked);
        setValue(e.target.checked);
    };

    useEffect(() => {
        setProperties(toggleButtonState.properties)
        setValue(toggleButtonState.getValue())
        const valueListenerId = toggleButtonState.valueChangedEvent.addListener(() => {
            setValue(toggleButtonState.getValue());
        });
        const propertiesListenerId =
            toggleButtonState.propertiesChangedEvent.addListener(() =>
                setProperties(toggleButtonState.properties)
            );

        return function cleanup() {
            toggleButtonState.valueChangedEvent.removeListener(valueListenerId);
            toggleButtonState.propertiesChangedEvent.removeListener(propertiesListenerId);
        };
    }, []);

    return (
        <div
            {...{
                [controlParameterIndexAnnotation]:
                    toggleButtonState.properties.parameterIndex,
            }} className="flex justify-center items-center">
            <span className="text-primary font-bold px-2">
                {properties.name}</span>
            <input type="checkbox" checked={value} className="toggle" onChange={handleChange} />
        </div>
    );
}

export default ToggleButton