import { useState, useEffect, ChangeEvent } from "react"
import * as Juce from "juce-framework-frontend";

interface Props {
    identifier: string,
    controlParameterIndexAnnotation: string,
}

const ComboBox = ({ identifier, controlParameterIndexAnnotation }: Props) => {

    const comboBoxState = Juce.getComboBoxState(identifier);

    const [value, setValue] = useState<number>(comboBoxState.getChoiceIndex());
    const [properties, setProperties] = useState<any>(comboBoxState.properties);

    const handleChange = (e: ChangeEvent<HTMLInputElement>) => {
        comboBoxState.setChoiceIndex(e.target.value);
        setValue(Number(e.target.value));
    };

    useEffect(() => {
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
                <span className="label">{choice}</span>
                <input checked={i == Number(value)} onChange={handleChange} type="radio" name="radio-1" className="radio" value={i} />
            </label>
        ))}
    </div>
}

export default ComboBox;