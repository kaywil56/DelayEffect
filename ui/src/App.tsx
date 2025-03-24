import Slider from "./components/Slider"
import * as Juce from "juce-framework-frontend"

const controlParameterIndexAnnotation = "controlparameterindex";

function App() {
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
          title="Dry/Wet"
          controlParameterIndexAnnotation={controlParameterIndexAnnotation} min={0} max={1} />
        <Slider
          identifier="feedback"
          title="Feedback"
          controlParameterIndexAnnotation={controlParameterIndexAnnotation}
          min={0}
          max={1} />
        <Slider
          identifier="delayTime"
          title="Delay Time"
          controlParameterIndexAnnotation={controlParameterIndexAnnotation}
          min={0} max={2} />
        <div className="join">
        <label className="input join-item">
            <span className="label">Time</span>
            <input type="radio" name="radio-1" className="radio" defaultChecked />
          </label>
          <label className="input join-item">
            <span className="label">Beat</span>
            <input type="radio" name="radio-1" className="radio" />
          </label>
          <label className="input join-item">
            <span className="label">Dotted</span>
            <input type="radio" name="radio-1" className="radio"/>
          </label>
          <label className="input join-item">
            <span className="label">Triplet</span>
            <input type="radio" name="radio-1" className="radio"/>
          </label>
        </div>
      </div>
      <div className="divider"></div>
      Made By Kaylem
    </main>
  )
}

export default App
