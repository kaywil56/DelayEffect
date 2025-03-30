import { useState, useEffect, ChangeEvent } from "react"
import * as Juce from "juce-framework-frontend";
import { IComboBoxProperties } from "../interfaces/IComboBoxProperties";

interface Props {
    identifier: string,
    controlParameterIndexAnnotation: string,
    value: number,
    setValue: (value: number) => void
}

const ComboBox = ({ identifier, controlParameterIndexAnnotation, value, setValue }: Props) => {

    const comboBoxState = Juce.getComboBoxState(identifier);
    const [properties, setProperties] = useState<IComboBoxProperties>(comboBoxState.properties);

    const handleChange = (e: ChangeEvent<HTMLInputElement>) => {
        comboBoxState.setChoiceIndex(e.target.value);
        setValue(Number(e.target.value));
    };

    useEffect(() => {
        setProperties(comboBoxState.properties)
        setValue(comboBoxState.getChoiceIndex())
        const valueListenerId = comboBoxState.valueChangedEvent.addListener(() => {
            setValue(comboBoxState.getChoiceIndex());
        })
        const propertiesListenerId =
            comboBoxState.propertiesChangedEvent.addListener(() => {
                setProperties(comboBoxState.properties);
            })

        return function cleanup() {
            comboBoxState.valueChangedEvent.removeListener(valueListenerId);
            comboBoxState.propertiesChangedEvent.removeListener(propertiesListenerId);
        }
    }, [])

    return <div className="join"
        {...{
            [controlParameterIndexAnnotation]:
                comboBoxState.properties.parameterIndex,
        }}
    >
        {properties.choices.map((choice: string, i: number) => (
            <label key={i} className="input join-item">
                <span className="label text-primary font-bold">{choice}</span>
                <input checked={i == value} onChange={handleChange} type="radio" name={`radio-${i}`} className="radio" value={i} />
            </label>
        ))}
    </div>
}

export default ComboBox;