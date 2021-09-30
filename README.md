# G.O.A.T: Granular Omnipotent Audio Transformer
A granular delay effect using randomization of input parameters and pitch detection to alter the grain size.

## How To Use

Move your soundfile (.wav) in the same folder as the PureData file.

Type the name of your soundfile in the message box or use the three examples.

There is small variety of presets given. Each one in a seperate textfile (.txt).
In the upper left corner you can choose them, the first slot resets all values to default.

There are the parameters of the granular engine (orange) and three kinds of modulators, 4 LFOs (green), 3 random number generators with normal distribution and pitch detector.
Every parameter with a slider can be controlled and modulated. 
For modulation click first on the button next to the modulator name and second on the vertical slider next to parameter which shall be modulated.
The modulator is now attached to the parameter. The amount of modulation can be set with the vertical slider.
The resulting value for the parameter is given by:

```
value = parameter + modulation * amount
```

One modulator can be attached to several parameters and even the parameters of the modulators can be modulated.
To detach the modulation from a parameter click first on the detach button and second on the amount slider of the respective parameter. 
Click the post_parameter button to post the current values of the parameters to the PureData console.

Use the wet/dry slider to mix the altered signal with the original sound.
 
### Parameters and Modulators

#### Granular Engine
Parameter | Description
--- | ---
grainsize | length of the grain in seconds
graindistance | distance between the grains relative to the grainsize
graindelay | delay of the grains relative to the original signal in seconds  
grainpitch | added pitch in semitones  
use relative pitch | uses the pitch detector to keep the grain at a constant frequency  
grain envelope | choose between four envelopes to shape the grains  
attack and release time | in seconds, can be altered for trapezoidal and cosine bell envelopes  

#### LFO
Parameter | Description
| --- | --- |
frequency | rate of the LFO  
curve | choose between sine, triangle, square and two different saw waves  

#### Random number generator
Parameter | Description
| --- | --- |
frequency | rate of newly generated random numbers  
value | expectation value of the normal distribution  
variation | standard deviation of the normal distribution  

#### Pitch Detector
Returns the current frequency multiplied by the factor which can be set with the slider.
Use negative values for inverted modulation. 
